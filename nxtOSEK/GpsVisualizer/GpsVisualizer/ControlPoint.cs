using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;

namespace GpsVisualizer
{
    [TypeConverter(typeof(ControlPointConverter))]
	public class ControlPoint : Label
    {
        private const int DEFAULT_POINT_SIZE = 9;
        private const int NO_ANGLE = -1;
        private const int FORWARD_DEFAULT = 100;
        private const int ALLOWABLE_ERROR_DEFAULT = 100;
        private const bool SLOWDOWN_DEFAULT = false;

		//親のユーザーコントロール
		CourseView owner;

		//制御点のドラッグ移動のための変数
		private bool isDraggable = false;
		private Point dragBasePoint = new Point();

		//ネイティブ座標<->スケーリング済ピクセル座標の変換のための変数
		private Point pixelLocationOffset = new Point();

        //制御点情報(エクスポート用)
        private Point nativeLocation; //ネイティブのロケーション(mm)
        private int nativeAngle = NO_ANGLE; //ネイティブの角度
        private int forward = FORWARD_DEFAULT; //ロボ子の移動速度
        private int allowableError = ALLOWABLE_ERROR_DEFAULT; //許容誤差
        private bool slowdown = SLOWDOWN_DEFAULT; //スローダウンスキルを使用するか

        //コースを再描画するか(制御点作成時は作成側の責任で呼ばれるためfalse)
        private bool enableCourseDraw = false;

        [Category("ロボット制御")]
        [DescriptionAttribute("実際のコースにおけるX座標をmm単位で指定します。")]
		public int NativeX
		{
			set
			{
				nativeLocation.X = value;
                CheckRegion();
                UpdateControlLocation();
			}
			get
			{
				return nativeLocation.X;
			}
        }

        [Category("ロボット制御")]
        [DescriptionAttribute("実際のコースにおけるY座標をmm単位で指定します。")]
        public int NativeY
        {
            set
            {
                nativeLocation.Y = value;
                CheckRegion();
                UpdateControlLocation();
            }
            get
            {
                return nativeLocation.Y;
            }
        }

        [Category("ロボット制御"),
        DescriptionAttribute("この制御点に到達後、ロボットが向く方向を指定します。向きを指定せず、そのまま次の制御点を目指して走行させる場合は、-1を指定します。"),
        DefaultValue(NO_ANGLE),
        Editor(typeof(AngleEditor), typeof(UITypeEditor))]
        public int NativeAngle
        {
            set
            {
                if (value < 0)
                {
                    //マイナスの値なら向きを指定しないので-1をセット
                    nativeAngle = NO_ANGLE;
                }
                else
                {
                    //360度以上は無意味なのでmodでまるめる
                    nativeAngle = value % 360;
                }
            }
            get
            {
                return nativeAngle;
            }
        }

        [Category("ロボット制御"),
         DescriptionAttribute("移動速度を0-100の範囲で指定します。"),
         DefaultValue(FORWARD_DEFAULT)]
        public int Forward
        {
            set
            {
                forward = value;
            }
            get
            {
                return forward;
            }
        }

        [Category("ロボット制御"),
         DescriptionAttribute("制御点に到達したとみなす誤差をmm単位で指定します。"),
         DefaultValue(ALLOWABLE_ERROR_DEFAULT)]
        public int AllowableError
        {
            set
            {
                allowableError = value;
            }
            get
            {
                return allowableError;
            }
        }

        [Category("ロボット制御"),
         DescriptionAttribute("この制御点から次の制御点に向かうとき、Slowdown Skillを使用するかを指定します。"),
         DefaultValue(SLOWDOWN_DEFAULT)]
        public bool Slowdown
        {
            set
            {
                slowdown = value;
            }
            get
            {
                return slowdown;
            }
        }

        public bool EnableCourseDraw
        {
            set
            {
                enableCourseDraw = value;
            }
            get
            {
                return enableCourseDraw;
            }
        }

		//ピクセル単位の制御点座標を返す
		public Point ScaledLocation
		{
			get
			{
				return EtCourseScaler.NativeToPixelInvY(nativeLocation);
			}
		}


		//コンストラクタ
		//スケーリングのかからないネイティブのXとY座標(単位：mm)、スケーリング比率を渡す
		public ControlPoint(int nativeX, int nativeY, CourseView Owner)
		{
			this.owner = Owner;
            this.nativeLocation.X = nativeX;
            this.nativeLocation.Y = nativeY;
            this.NativeAngle = -1;

			//イベントハンドラの設定
			this.Resize += new System.EventHandler(OnResize);
			this.Move += new System.EventHandler(OnMove);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(OnMouseMove);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(OnMouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(OnMouseUp);
			
			//初期スタイルの設定
			this.BackColor = Color.White;
			this.BorderStyle = BorderStyle.FixedSingle;
			this.Size = new Size(DEFAULT_POINT_SIZE, DEFAULT_POINT_SIZE);
			this.Text = "";

			//スケール後座標の設定
			UpdateControlLocation();
		}

		//オフセットを再計算する
		private void UpdatePixelOffset()
		{
			pixelLocationOffset.X = (this.Width / 2);
			pixelLocationOffset.Y = (this.Height / 2);
		}

		//イベント ハンドラ
		//コントロールサイズ変更時は座標オフセットを再計算
		private void OnResize(object sender, EventArgs e)
		{
			UpdatePixelOffset();
		}

		//イベント ハンドラ
		//コントロール移動時にはネイティブのXとY座標を再計算
		private void OnMove(object sender, EventArgs e)
		{
			//スケール前の制御点座標を求める
			RecalculateNativePosition();
		}

		//イベント ハンドラ
		//コントロールのドラッグ&ドロップを実装
		private void OnMouseDown(object sender, MouseEventArgs e)
		{
			//選択されたことにする
			owner.ControlPointSelected(this);

			//左クリックか
			if (e.Button == MouseButtons.Left)
			{
                //制御点のドラッグ開始
				isDraggable = true;
				Debug.WriteLine("ドラッグ開始");

                //コースの描画を抑制
                this.EnableCourseDraw = false;

				//ドラッグ開始時のカーソル位置を保持
				dragBasePoint.X = e.X;
				dragBasePoint.Y = e.Y;
			}

		}
		private void OnMouseMove(object sender, MouseEventArgs e)
		{
			//左クリックか
			if (e.Button == MouseButtons.Left)
			{
				//ドラッグ可能状態か
				if (!isDraggable) return;

				this.Left += e.X - dragBasePoint.X;
				this.Top += e.Y - dragBasePoint.Y;
			}
		}
		private void OnMouseUp(object sender, MouseEventArgs e)
		{
			//左クリックか
			if (e.Button == MouseButtons.Left)
			{
                //制御点ドラッグ終了
				isDraggable = false;

                //座標設定
				RecalculateNativePosition();
                UpdateControlLocation();

                //コースの描画抑制を解除
                this.EnableCourseDraw = true;

                //描画OKなら(OK確定だが)
                if (this.EnableCourseDraw)
                {
                    //コースを再描画
                    owner.UpdateView();
                }

				Debug.WriteLine("ドラッグ終了");
			}
        }

        public void ControlPointActivated()
        {
            Debug.WriteLine("active");
            //いろかえる
            this.BackColor = Color.Red;
        }

        public void ControlPointDeactivated()
        {
            Debug.WriteLine("deactive");

            //いろかえる
            this.BackColor = Color.White;
        }

		//制御点がコース外に飛び出てたらもどす
		private void CheckRegion()
		{
			if (this.nativeLocation.X < 0)
				this.nativeLocation.X = 0;
			if (-this.nativeLocation.Y < 0)
				this.nativeLocation.Y = 0;
			if (this.nativeLocation.X > EtCourseScaler.GetNativeCourseSize().Width)
				this.nativeLocation.X = EtCourseScaler.GetNativeCourseSize().Width;
			if (-this.nativeLocation.Y > EtCourseScaler.GetNativeCourseSize().Height)
				this.nativeLocation.Y = -EtCourseScaler.GetNativeCourseSize().Height;
		}

		//コントロール位置からネイティブ位置を計算
		private void RecalculateNativePosition()
		{
			Point pixelLocation = this.Location;
			pixelLocation.Offset(pixelLocationOffset.X, pixelLocationOffset.Y);

            this.NativeX = EtCourseScaler.PixelToNativeInvY(pixelLocation).X;
            this.NativeY = EtCourseScaler.PixelToNativeInvY(pixelLocation).Y;
        }

		//ネイティブ位置からコントロール位置を再計算して設定
		private void UpdateControlLocation()
		{
			Point pixelLocation = EtCourseScaler.NativeToPixelInvY(nativeLocation);
			pixelLocation.Offset(-pixelLocationOffset.X, -pixelLocationOffset.Y);
            this.Location = pixelLocation;
		}
	}

    //プロパティグリッドに表示するプロパティを限定する
    internal class ControlPointConverter : TypeConverter
    {
        public override bool GetPropertiesSupported(ITypeDescriptorContext context)
        {
            return true;
        }
        public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
        {
            List<PropertyDescriptor> descriptors = new List<PropertyDescriptor>();
            PropertyDescriptorCollection source = TypeDescriptor.GetProperties(value, attributes);
            foreach (PropertyDescriptor desc in source)
            {
                if (
                    desc.Name == ("NativeX") ||
                    desc.Name == ("NativeY") ||
                    desc.Name == ("NativeAngle") ||
                    desc.Name == ("AllowableError") ||
                    desc.Name == ("Slowdown") ||
                    desc.Name == ("Forward")
                    )
                {
                    descriptors.Add(desc);
                }
            }
            return new PropertyDescriptorCollection(descriptors.ToArray());
        }
    }

    //角度プロパティエディタ
    internal class AngleEditor : UITypeEditor
    {
        AngleEditorControl ui = null;

        public override bool GetPaintValueSupported(ITypeDescriptorContext context)
        {
            //プロパティの描画はブラウザ側にやってもらう
            //自前では書かない
            return false;
        }

        //プロパティ編集をドロップダウンスタイルに設定
        public override UITypeEditorEditStyle GetEditStyle(
         ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.DropDown;
        }

        //プロパティ編集開始
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider sp, object value)
        {
            Debug.WriteLine("a");

            //エディタサービスを取得
            IWindowsFormsEditorService edSvc =
                (IWindowsFormsEditorService)sp.GetService(typeof(IWindowsFormsEditorService));

            //サービスが取得できなかったら何もしないでそのまま戻る
            if (edSvc == null)
                return value;

            //(未作成なら)エディタコントロールを作成
            if (ui == null)
            {
                ui = new AngleEditorControl();
            }

            //uiをプロパティにあわせて初期化
            if (value is int)
            {
                ui.Angle = (int)value;
            }
            else
            {
                return value;
            }

            //uiをドロップダウンとして表示
            edSvc.DropDownControl(ui);

            // 更新された値を返す
            return ui.Angle;
        }

    }



}
