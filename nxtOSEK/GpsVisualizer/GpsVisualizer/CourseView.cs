using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.Drawing.Drawing2D;
using System.Collections;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.IO;

namespace GpsVisualizer
{
	enum Insert{Before, After};

	public partial class CourseView : UserControl
	{
		//定数
		//---------------------------------------------------

		//コースに引かれるグリッド線の間隔と色
		private int GRID_PITCH; //mm単位
		private Color GRID_LINE_COLOR;
		private Color GRID_TEXT_COLOR;
		private Color GRID_TEXT_SHADOW_COLOR;

        //制御線の色と太さ
        private Color CONTROL_LINE_COLOR;
        private int CONTROL_LINE_WIDTH;

        //軌跡の色と太さ
		private Color TRACK_LINE_COLOR;
		private int TRACK_LINE_WIDTH;
		//軌跡を最大何点まで描画するか
		private int TRACK_COUNT;

		//コース画像のファイル名
		private const string COURSE_IMAGE = "GpsVisualizer.image.course.png";

        //ファイル出力するときのTRUEとFALSEを示す文字列
        private const string BOOL_TRUE_STRING = "TRUE";
        private const string BOOL_FALSE_STRING = "FALSE";


		//メンバ変数
		//---------------------------------------------------

		//制御点のリスト
		private List<ControlPoint> controlPointList = new List<ControlPoint>();

		//コースのスクロールフラグ
		private bool bScrolling = false;
		//スクロールの基準座標
		private Point scrollOrigin;

		//コース画像
		private Bitmap courseImage = null;
		//最終レンダリングターゲット(バックバッファ)
		private Bitmap renderTarget = null;

		//読み込んだログファイル名(未読込みならnull)
		private string logFilePath = null;
		//読み込んだログファイルの内容
		private NxtTrackLog nxtTrackLog = null;

		//ログファイルの総再生時間(ms)
		private int duration = 0;

		//ログファイルの現在の再生位置(配列のインデックス)
		private int playPosition = 0;

		//再生状態
		PlayStatus playStatus = PlayStatus.Close;

        //コールバック用
        private UpdateCallback playInfoUpdater = null;
        private MouseCallback mouseInfoUpdater = null;
        private ControlPointSelectedCallback activeControlPointNotifier = null;

        //現在選択されているコントロールポイント
        ControlPoint activeControlPoint = null;

		//デリゲート
		//---------------------------------------------------

        public delegate void UpdateCallback(PlayInfo info);
        public delegate void MouseCallback(Point point);
        public delegate void ControlPointSelectedCallback(ControlPoint cp);


		//プロパティ
		//---------------------------------------------------

		/**
		 * ログファイルのパスを取得する。
		 * 例外：InvalidNxtLogFileException, IOException
		 */
		public string LogFilePath
		{
			get {
				return logFilePath;
			}
		}

		/**
		 * ログファイルの総再生時間を取得する。
		 */
		public int Duration{
			get 
			{
				return duration;
			}
		}

		/**
		 * ログファイルの現在の再生位置(ms)を取得する。
		 */
		public int PlayTime{
			get
			{
				return PositionToTime(playPosition);
			}
		}

		//関数
		//---------------------------------------------------

		/**
		 * ログファイルを開く
		 */
		public void OpenLogFile(string filePath) { 
			//いったん閉じる
			CloseLogFile();

			//ログファイルを読み込む
			logFilePath = filePath;
			nxtTrackLog = new NxtTrackLog(filePath);
			playPosition = 0;
			playStatus = PlayStatus.Stop;
			UpdateView();
			NoticeUpdate();
		}
	
		/**
		 * ログファイルを閉じる
		 */
		public void CloseLogFile() { 
			//ファイルを閉じる
			nxtTrackLog = null;
			logFilePath = null;
			//アップデート
			playPosition = 0;
			playStatus = PlayStatus.Close;
			UpdateView();
			NoticeUpdate();
		}
		


		/**
		 * ログファイルを現在の位置から再生する。
		 */
		public void Play() { }

		/**
		 * ログファイルの再生を一時停止する。
		 */
		public void Pause() { }

		/**
		 * ログファイルの再生を一時停止した後、一番最初へシークする。
		 */
		public void Stop() { }
		
		/**
		 * 再生位置を変更する。
		 */
		public void Seek(int position) {
			//ファイルが開かれていなければ(nxtTrackLog == null)
			if (playStatus == PlayStatus.Close)
			{
				throw new NxtLogFileNotOpenedException("ログファイルが開かれていません。");
			}
			playPosition = position;
			UpdateView();
			NoticeUpdate();
		}

		/**
		 * コンストラクタ
		 */
		public CourseView()
		{
			InitializeComponent();
            InitializeSettings();
            InitializeImages();
			UpdateView();
		}

        /**
         * 線の色や太さなどの設定を読み込む
         */
        private void InitializeSettings() 
        {
            CONTROL_LINE_WIDTH = Properties.Settings.Default.CONTROL_LINE_WIDTH;

            GRID_PITCH = Properties.Settings.Default.GRID_PITCH;
            GRID_LINE_COLOR = Properties.Settings.Default.GRID_LINE_COLOR;
            GRID_TEXT_COLOR = Properties.Settings.Default.GRID_TEXT_COLOR;
            GRID_TEXT_SHADOW_COLOR = Properties.Settings.Default.GRID_TEXT_SHADOW_COLOR;

            CONTROL_LINE_COLOR = Properties.Settings.Default.CONTROL_LINE_COLOR;
            CONTROL_LINE_WIDTH = Properties.Settings.Default.CONTROL_LINE_WIDTH;

            TRACK_LINE_COLOR = Properties.Settings.Default.TRACK_LINE_COLOR;
            TRACK_LINE_WIDTH = Properties.Settings.Default.TRACK_LINE_WIDTH;
            TRACK_COUNT = Properties.Settings.Default.TRACK_COUNT;
        }

		/**
		 * パラメータに変化があった場合に呼び出されるメソッドを登録する
		 */
		public void SetUpdateCallback(UpdateCallback callback)
		{
			playInfoUpdater += callback;
			NoticeUpdate();
		}

		/**
		 * パラメータに変化があった場合に呼び出されるメソッドを登録する
		 */
		public void SetMouseCallback(MouseCallback callback)
		{
			mouseInfoUpdater += callback;
		}

		/**
		 * コントロールポイントが選択された場合に呼び出されるメソッドを登録する
		 */
        public void SetControlPointSelectedCallback(ControlPointSelectedCallback callback)
		{
            activeControlPointNotifier += callback;
		}

		/**
		 * パラメータの変化を通知する
		 */
		public void NoticeUpdate()
		{
			//通知先が登録されていなければ何もしない
			if (playInfoUpdater == null)
			{
				return;
			}

			//ファイルが開かれていなければclose通知以外はゴミデータ
			if (playStatus == PlayStatus.Close)
			{
				playInfoUpdater(new PlayInfo(playStatus, 0, null));
			}
			else
			{
				playInfoUpdater(new PlayInfo(playStatus, nxtTrackLog.Length, nxtTrackLog.getLog(playPosition)));
			}
		}

		/**
		 * マウス位置の変化を通知する(単位：mm)
		 */
		public void NoticeMousePos(Point point)
		{
			//通知先が登録されていなければ何もしない
			if (mouseInfoUpdater == null)
			{
				return;
			}

			mouseInfoUpdater(point);
		}

        //選択されているコントロールポイントが変化したことを通知する
        public void ControlPointSelected(ControlPoint cp)
        {
            //選択が変化したか確認(選択済のコントロールポイントが再クリックされた可能性がある)
            if (cp != activeControlPoint)
            {
                //いままで選択されていたコントロールポイントにデアクティブを通知
                if (activeControlPoint != null)
                {
                    activeControlPoint.ControlPointDeactivated();
                }

                //現在の選択を更新
                activeControlPoint = cp;

                //クリックされたコントロールポイントにアクティブを通知
				if (cp != null)
				{
					cp.ControlPointActivated();
				}

                //選択が変化したことを通知
                if (activeControlPointNotifier != null)
                {
                    activeControlPointNotifier(cp);
                }
            }

            return;
        }


		/**
		 * コース画像を読み込んでリサイズし保持する。
		 * 読込みに失敗した場合は同サイズの白い画像を作成して保持する。
		 */
		private void InitializeImages()
		{
			Size courseImageSize = EtCourseScaler.GetPixelCourseSize();
			try
			{
				Assembly assembly = Assembly.GetExecutingAssembly();
				courseImage = new Bitmap(new Bitmap(assembly.GetManifestResourceStream(COURSE_IMAGE)), courseImageSize.Width, courseImageSize.Height);
			}
			catch (Exception)
			{
				courseImage = new Bitmap(courseImageSize.Width, courseImageSize.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				Graphics g = Graphics.FromImage(courseImage);
				g.Clear(Color.White);
				g.DrawString("コース画像を読み込めませんでした。起動は続行されますが、コース画像は表示されません。",DefaultFont ,new SolidBrush(GRID_TEXT_COLOR), new Point(0, 0));
			}

			DrawGrid(courseImage, EtCourseScaler.NativeToPixel(GRID_PITCH));
			
			//renderTargetを同サイズに
			renderTarget = new Bitmap(courseImageSize.Width, courseImageSize.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
		}

		/**
		 * グリッドを描画する。
		 */
		private void DrawGrid(Bitmap image, int pitch)
		{
			Graphics g = Graphics.FromImage(image);
			int linePos = 0;
			Pen linePen = new Pen(GRID_LINE_COLOR, 1);
			linePen.DashStyle = DashStyle.Dash;
			SolidBrush textBrush = new SolidBrush(GRID_TEXT_COLOR);
			SolidBrush textShadowBrush = new SolidBrush(GRID_TEXT_SHADOW_COLOR);

			//横線
			linePos = 0;
			for (int i = 0; i < image.Height; i += pitch)
			{
				g.DrawLine(linePen, 0, i, image.Width, i);
				g.DrawString(linePos + "mm", DefaultFont, textShadowBrush, new Point(0 + 1, i + 1));
				g.DrawString(linePos + "mm", DefaultFont, textBrush, new Point(0, i));
				linePos += GRID_PITCH;
			}

			//縦線
			linePos = 0;
			for (int i = 0; i < image.Width; i += pitch)
			{
				g.DrawLine(linePen, i, 0, i, image.Height);
				g.DrawString(linePos + "mm", DefaultFont, textShadowBrush, new Point(i + 1, 0 + 1));
				g.DrawString(linePos + "mm", DefaultFont, textBrush, new Point(i, 0));
				linePos += GRID_PITCH;
			}
		}

		/**
		 * コースビューの表示を更新する。
		 */
		public void UpdateView()
		{
            Debug.WriteLine("UpdateView()");

			Graphics g = Graphics.FromImage(renderTarget);

			//ファイルが閉じている
			if (playStatus == PlayStatus.Close)
			{
				//ベース(コース)のみ描画
				g.DrawImage(courseImage, 0, 0);
			}
			else
			{

				//ベース(コース)描画
				g.DrawImage(courseImage, 0, 0);

				//軌跡を描画-------------------------------------------------------
				Point startPoint = EtCourseScaler.NativeToPixelInvY(nxtTrackLog.getLog(playPosition).point);

				//最初の一点は必ず描画するため、開始座標を+1する
				startPoint.Offset(new Point(1, 1));

				Point endPoint;

				int trackCount = 0; //何ログ分まで描いたかをカウント
				for (int i = playPosition; i >= 0; i--)
				{
					//規定数分描いたらやめる
					if (trackCount >= TRACK_COUNT)
					{
						break;
					}

					endPoint = EtCourseScaler.NativeToPixelInvY(nxtTrackLog.getLog(i).point);

					//色作成
					Pen gpsLinePen = new Pen(Color.FromArgb((int)(255 - (trackCount * (255 / (double)TRACK_COUNT))), TRACK_LINE_COLOR), TRACK_LINE_WIDTH);
					//描画
					g.DrawLine(gpsLinePen, startPoint, endPoint);

					startPoint = endPoint;
					trackCount++;
				}
			}

			//制御線を描画----------------------------------------------------------
			int pointCount = controlPointList.Count;
            Pen controlLinePen = new Pen(CONTROL_LINE_COLOR, CONTROL_LINE_WIDTH);

			//制御点が一つ以上あれば開始点・終了点に丸をつける
			if (pointCount > 0)
			{
				//終了点
				g.FillEllipse(new SolidBrush(Color.White), controlPointList[pointCount - 1].ScaledLocation.X - 11, controlPointList[pointCount - 1].ScaledLocation.Y - 11, 20, 20);
				g.DrawEllipse(new Pen(Color.Red, 2), controlPointList[pointCount - 1].ScaledLocation.X - 11, controlPointList[pointCount - 1].ScaledLocation.Y - 11, 20, 20);

				//開始点
				g.FillEllipse(new SolidBrush(Color.White), controlPointList[0].ScaledLocation.X - 9, controlPointList[0].ScaledLocation.Y - 9, 16, 16);
				g.DrawEllipse(new Pen(Color.Blue, 2), controlPointList[0].ScaledLocation.X - 9, controlPointList[0].ScaledLocation.Y - 9, 16, 16);
			}

			//制御点が2個以上なら線を描く
			if (pointCount > 1)
			{
				//二制御点間に線を引く
				for (int i = 0; i < (pointCount - 1); i++)
				{
					g.DrawLine(
						controlLinePen,
						controlPointList[i].ScaledLocation,
						controlPointList[i + 1].ScaledLocation);
				}
			}

			//表示更新
			if(coursePicture.Image == null)
			{
				coursePicture.Image = renderTarget;
			}
			coursePicture.Refresh();
		}

		/**
		 * すべての軌跡を描画し、画像として保存する。
		 */
		public void SaveAllTrack(string fileName)
		{
			//ファイルが閉じている
			if (playStatus == PlayStatus.Close)
			{
				throw new NxtLogFileNotOpenedException("ログファイルが開かれていません。");
			}

			//ログの最後
			int last = nxtTrackLog.Length - 1;

			//保存対象のBitmapを生成
			Size courseImageSize = EtCourseScaler.GetPixelCourseSize();
			Bitmap trackImage = new Bitmap(courseImageSize.Width, courseImageSize.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

			Graphics g = Graphics.FromImage(trackImage);

			//ベース(コース)描画
			g.DrawImage(courseImage, 0, 0);

			//軌跡描画---------
			Point startPoint = EtCourseScaler.NativeToPixelInvY(nxtTrackLog.getLog(last).point);
			//最初の一点は必ず描画するため、開始座標を+1する
			startPoint.Offset(new Point(1, 1));

			Point endPoint;

			//色作成
			Pen linePen = new Pen(TRACK_LINE_COLOR, TRACK_LINE_WIDTH);
			for (int i = last; i >= 0; i--)
			{
				endPoint = EtCourseScaler.NativeToPixelInvY(nxtTrackLog.getLog(i).point);

				//描画
				g.DrawLine(linePen, startPoint, endPoint);

				startPoint = endPoint;
			}

			//保存
			trackImage.Save(fileName, ImageFormat.Png);
		}


		/**
		 * プレイ位置(ログ配列のインデックス)からプレイ時間(ms)に変換する
		 * 変換できない場合は例外を返す
		 */
		private int PositionToTime(int playPosition)
		{
			//ファイルが開かれていなければ(nxtTrackLog == null)
			if (playStatus == PlayStatus.Close)
			{
				throw new NxtLogFileNotOpenedException("ログファイルが開かれていません。");
			}

			XYAngle log = nxtTrackLog.getLog(playPosition);
			return log.time;
		}


		/**
		 * コースのドラッグスクロールを開始する。
		 */
		private void coursePicture_MouseDown(object sender, MouseEventArgs e)
		{
			//左クリックならスクロール開始
			if (e.Button == MouseButtons.Left)
			{
				int offsetX = -this.AutoScrollPosition.X;
				int offsetY = -this.AutoScrollPosition.Y;

				scrollOrigin = coursePicture.PointToScreen(new Point(e.X, e.Y));

				//現在のスクロール位置分オフセットを取る
				scrollOrigin.Offset(new Point(offsetX, offsetY));
				bScrolling = true;
			}
		}

		/**
		 * コースのドラッグスクロール中？
		 */
		private void coursePicture_MouseMove(object sender, MouseEventArgs e)
		{
			if (bScrolling)
			{ //ドラッグスクロール中
				Point scrollPoint = coursePicture.PointToScreen(new Point(e.X, e.Y));
				int x = scrollPoint.X - scrollOrigin.X;
				int y = scrollPoint.Y - scrollOrigin.Y;
				this.AutoScrollPosition = new Point(x * -1, y * -1);
			}
			else
			{
				//座標を通知
				NoticeMousePos(EtCourseScaler.PixelToNativeInvY(new Point(e.X, e.Y)));
			}
		}

		/**
		 * コースのドラッグスクロールを終了する。
		 */
		private void coursePicture_MouseUp(object sender, MouseEventArgs e)
		{
			//左クリックならスクロール終了
			if (e.Button == MouseButtons.Left)
			{
				bScrolling = false;
			}
		}

		//イベント ハンドラ
		//制御点を追加
		private void addControlPointMenu_Click(object sender, EventArgs e)
		{
			//制御点を追加する座標を算出して作成
			Point addLocation = EtCourseScaler.PixelToNativeInvY(coursePicture.PointToClient(new Point(mapMenu.Left, mapMenu.Top)));

			ControlPoint point = AddControlPoint(addLocation);

			//選択する
			this.ControlPointSelected(point);

			//線を再描画
			UpdateView();
		}

		//イベント ハンドラ
		//前に制御点を挿入
		private void addControlPointBeforeMenu_Click(object sender, EventArgs e)
		{
			//コンテキストメニューの呼び出し元を取得
			Object source = controlPointMenu.SourceControl;
			Debug.Assert(source is ControlPoint);

			ControlPoint point = InsertControlPoint((ControlPoint)source, Insert.Before);

			//選択する
			this.ControlPointSelected(point);

			//線を再描画
			UpdateView();
		}

		//イベント ハンドラ
		//後に制御点を挿入
		private void addControlPointAfterMenu_Click(object sender, EventArgs e)
		{
			//コンテキストメニューの呼び出し元を取得
			Object source = controlPointMenu.SourceControl;
			Debug.Assert(source is ControlPoint);

			ControlPoint point = InsertControlPoint((ControlPoint)source, Insert.After);

			//選択する
			this.ControlPointSelected(point);

			//線を再描画
			UpdateView();
		}

		//イベント ハンドラ
		//制御点を削除
		private void removeControlPointMenu_Click(object sender, EventArgs e)
		{
			//コンテキストメニューの呼び出し元を取得
			Object source = controlPointMenu.SourceControl;
			Debug.Assert(source is ControlPoint);

			RemoveControlPoint((ControlPoint)source);

			//線を再描画
			UpdateView();
		}

		//制御点を削除
		private void RemoveControlPoint(ControlPoint controlPoint)
		{
			//リストから制御点を削除
			controlPointList.Remove(controlPoint);

			//画面から制御点を削除
			this.containerPanel.Controls.Remove(controlPoint);

			//選択されていたら選択解除
			if (this.activeControlPoint == controlPoint)
			{
				this.ControlPointSelected(null);
			}

			controlPoint.Dispose();
		}

        //制御点をすべて削除
        private void RemoveAllControlPoint()
        {
			while(controlPointList.Count > 0)
			{
				this.RemoveControlPoint(controlPointList[0]);
			}
        }

		//指定位置に制御点を追加
		private ControlPoint AddControlPoint(Point addLocation)
		{
			ControlPoint newControlPoint = new ControlPoint(addLocation.X, addLocation.Y, this);

			//リストの最後に制御点を追加
			controlPointList.Add(newControlPoint);

			//コンテキストメニューを設定
			newControlPoint.ContextMenuStrip = controlPointMenu;

			//制御点を画面に反映
			this.containerPanel.Controls.Add(newControlPoint);
			newControlPoint.BringToFront();
            newControlPoint.EnableCourseDraw = true;

			return newControlPoint;
		}

		//指定制御点の前後どちらかに制御点を挿入
		private ControlPoint InsertControlPoint(ControlPoint point, Insert position)
		{
			Debug.Assert(point != null);

			//挿入位置の制御点は配列の何番目か検索
			int index = controlPointList.IndexOf(point);
			Debug.Assert(index != -1);

			ControlPoint newControlPoint = null;

			//リストに制御点を挿入(前or後)
			switch (position)
			{
			case Insert.Before:
				//制御点を追加する座標を算出して作成(挿入元から30mm左上の位置にハードコード)
				newControlPoint = new ControlPoint(point.NativeX - 30, point.NativeY + 30, this);
				controlPointList.Insert(index, newControlPoint);
				break;

			case Insert.After:
				//制御点を追加する座標を算出して作成(挿入元から30mm右下の位置にハードコード)
				newControlPoint = new ControlPoint(point.NativeX + 30, point.NativeY - 30, this);
				controlPointList.Insert(index + 1, newControlPoint);
				break;

			default:
				Debug.Assert(false);
				break;
			}

			//コンテキストメニューを設定
			newControlPoint.ContextMenuStrip = controlPointMenu;

			//制御点を画面に反映
			this.containerPanel.Controls.Add(newControlPoint);
			newControlPoint.BringToFront();

			return newControlPoint;
		}


        //ファイルから制御点をロード
        public void LoadVirtualLineFile(string filePath)
        {
            //現在の点は全部消す
            RemoveAllControlPoint();

            //            String data = "1515, -501\n1251, -477\n1098, -363\n1116, -150\n1320, -96\n1509, -192\n1494, -384\n1179, -603\n144, -597\n1023, -1287\n669, -273\n369, -1281\n1176, -603\n1470, -2346";
            String data = File.ReadAllText(filePath, System.Text.Encoding.GetEncoding("Shift_JIS"));

            //空白、タブを取り除く。ひどいコード！
            while (data.Contains(" "))
            {
                data = data.Replace(" ", "");
            }
            while (data.Contains("\t"))
            {
                data = data.Replace("\t", "");
            }

            //改行で分割
            string[] points = data.Split(new char[] { '\r', '\n' });
            int line = -1;

            foreach (string point in points)
            {
                line++;

                //カンマで分割
                string[] pointLocation = point.Split(',');

                try
                {
                    //カラム数が足りなくても、とにかくいけるところまでは読む(上位互換のため)
                    Point location = new Point();
                    location.X = int.Parse(pointLocation[0]);
                    location.Y = int.Parse(pointLocation[1]);

                    //ロケーションさえ読めれば点を作る
                    ControlPoint cp = AddControlPoint(location);

                    //他の値はオプション(カラムが無かったり不正値だった場合はcatchに飛ぶのでデフォルト値のまま)
                    int angle = int.Parse(pointLocation[2]);
                    cp.NativeAngle = angle;

                    int forward = int.Parse(pointLocation[3]);
                    cp.Forward = forward;

                    int allowableError = int.Parse(pointLocation[4]);
                    cp.AllowableError = allowableError;

                    bool slowdown = (pointLocation[5] == BOOL_TRUE_STRING)?true:false;
                    cp.Slowdown = slowdown;
                }
                catch (Exception e)
                {
                    //エラー処理はひどい
//					MessageBox.Show("エラーが発生しました。これ以降正常動作は期待できません。\r\n" + e.ToString());
                }
            }

            //線を再描画
            UpdateView();

        }

        //ファイルに制御点をセーブ
        public void SaveVirtualLineFile(string filePath)
        {
            int pointCount = controlPointList.Count;

            using (StreamWriter writer = new StreamWriter(filePath))
            {
                if (pointCount > 0)
                {
                    //制御点を吐く
                    for (int i = 0; i < pointCount; i++)
                    {
						writer.Write(controlPointList[i].NativeX);
						writer.Write(", ");
						writer.Write(controlPointList[i].NativeY);
                        writer.Write(", ");
                        writer.Write(controlPointList[i].NativeAngle);
                        writer.Write(", ");
                        writer.Write(controlPointList[i].Forward);
                        writer.Write(", ");
                        writer.Write(controlPointList[i].AllowableError);
                        writer.Write(", ");
                        writer.Write(controlPointList[i].Slowdown?BOOL_TRUE_STRING:BOOL_FALSE_STRING); //bool値はTRUE or FALSEで出力
                        writer.WriteLine();
                    }
                }
            }

        }










		public void ExportVirtualLine(MainWindow owner)
        {
            StringBuilder pointArrayCode = new StringBuilder();
            int pointCount = controlPointList.Count;

            //構造体とか定義部分
            pointArrayCode.AppendLine("typedef struct _TAGCONTROLPOINT{");
            pointArrayCode.AppendLine("\tfloat X; //目標X座標(mm)。");
            pointArrayCode.AppendLine("\tfloat Y; //目標Y座標(mm)。");
            pointArrayCode.AppendLine("\tfloat direction; //目標向き。方向転換しない場合はFP_NANを指定。");
            pointArrayCode.AppendLine("\tfloat forward; //座標指定走行用。フォワード値。");
            pointArrayCode.AppendLine("\tfloat allowableError; //共用。許容誤差(mm)。");
            pointArrayCode.AppendLine("\tfloat slowdown; //座標指定走行用。SlowdownSkill を使用するかどうか。");
            pointArrayCode.AppendLine("} ControlPoint;");
            pointArrayCode.AppendLine();
            pointArrayCode.AppendLine("int NUM_TEST_LINE = " + pointCount + ";");
            pointArrayCode.AppendLine();
            pointArrayCode.Append("ControlPoint VIRTUAL_LINE[]={");

            //何制御点ごとに改行するか
            const int RETURN_COUNT = 1;
            if (pointCount > 0)
            {
                pointArrayCode.AppendLine();

                //制御点を吐く
                for (int i = 0; i < pointCount; i++)
                {
                    if ((i % RETURN_COUNT) == 0)
                        pointArrayCode.Append("\t");

                    pointArrayCode.Append("{");
                    pointArrayCode.Append(controlPointList[i].NativeX);
                    pointArrayCode.Append(", ");
                    pointArrayCode.Append(controlPointList[i].NativeY);
                    pointArrayCode.Append(", ");
                    pointArrayCode.Append((controlPointList[i].NativeAngle < 0)?"FP_NAN":controlPointList[i].NativeAngle.ToString());
                    pointArrayCode.Append(", ");
                    pointArrayCode.Append(controlPointList[i].Forward);
                    pointArrayCode.Append(", ");
                    pointArrayCode.Append(controlPointList[i].AllowableError);
                    pointArrayCode.Append(", ");
                    pointArrayCode.Append(controlPointList[i].Slowdown?BOOL_TRUE_STRING:BOOL_FALSE_STRING);
                    pointArrayCode.Append("}");

                    //次の要素があるならカンマ・改行を打つ
                    if (pointCount != i + 1)
                    {
                        pointArrayCode.Append(", ");

                        //一定出力ごとに改行
                        if ((i % RETURN_COUNT) == RETURN_COUNT - 1)
                            pointArrayCode.AppendLine();
                    }
                }
            }
            pointArrayCode.Append("\r\n};");

			owner.putDebugText(pointArrayCode.ToString());
		}

        private void removeAllControlPointMenu_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("制御点をすべて削除しますか？", "", MessageBoxButtons.YesNo) == DialogResult.Yes) {
                RemoveAllControlPoint();

                //線を再描画
                UpdateView();
            }

        }

	}
}
