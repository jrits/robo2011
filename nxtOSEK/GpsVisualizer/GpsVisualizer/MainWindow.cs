using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace GpsVisualizer
{
	public partial class MainWindow : Form
	{
		//定数
		//---------------------------------------------------

		//車体角度表示サイズ
		private const int ANGLE_IMAGE_SIZE = 127;


		//メンバ変数
		//---------------------------------------------------

		//角度表示ウィンドウ
		AngleWindow angleWindow = null;

		//ひみつのデバッグ窓
		DebugOut debugWindow;


		//関数
		//---------------------------------------------------

		/**
		 * コンストラクタ
		 */
		public MainWindow()
		{
			InitializeComponent();
			InitializeGUI();
			courseView.SetUpdateCallback(new CourseView.UpdateCallback(Update));
			courseView.SetMouseCallback(new CourseView.MouseCallback(showMousePos));
            courseView.SetControlPointSelectedCallback
                (new CourseView.ControlPointSelectedCallback(UpdateSelectedControlPoint)
                );
		}

		/**
		 * GUIを初期化する。
		 */
		private void InitializeGUI() {
			InitializeListViewHeader();
			InitializeAngleWindow();

		}

		/*
		 * 情報リストのカラムヘッダ部分を構築する
		 */
		private void InitializeListViewHeader()
		{
			//ハードコーディング
			infoList.View = View.Details;
			infoList.GridLines = true;

			ColumnHeader[] columnHeader = {new ColumnHeader(), new ColumnHeader()};
			columnHeader[0].Text = "Param";
			columnHeader[0].Width = 60;
			columnHeader[1].Text = "Value";
			columnHeader[1].Width = 140;
			infoList.Columns.AddRange(columnHeader);
		}

		/*
		 * AngleWindowを作成して表示する。
		 */
		private void InitializeAngleWindow() {
			angleWindow = new AngleWindow(ANGLE_IMAGE_SIZE);
			angleWindow.Owner = this;
			angleWindow.Show();
		}


		/**
		 * アプリケーションを終了する。
		 */
		private void exitMenuItem_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		/**
		 * ログファイルを開く。
		 */
		private void OpenMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog openDlg = new OpenFileDialog();

			openDlg.FileName = "*.csv";
			openDlg.Filter = "csvファイル(*.csv)|*.csv";
			openDlg.Title = "走行ログを開く";

			if (openDlg.ShowDialog() == DialogResult.OK) {
				//ファイルが選択された
				try
				{
					courseView.OpenLogFile(openDlg.FileName);
				}
				catch (UserCanceledException) {
					//ユーザーキャンセルなのでなにもしない
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
			
		}

		/**
		 * シークバーがシークされたときに呼び出される。
		 */
		private void seekBar_Scroll(object sender, EventArgs e)
		{
			try
			{
				courseView.Seek(seekBar.Value);
			}
			catch (NxtLogFileNotOpenedException ex) {
				MessageBox.Show(ex.Message);
			}
		}

		/**
		 * すべてのパラメータ表示を更新する。
		 */
		private void Update(PlayInfo info) {
			//ファイルが閉じているか
			if (info.status == PlayStatus.Close)
			{
			//閉じている
				//サイドバー
				updateInfoList(null);

				//ステータスバー
				showMousePos(new Point(0, 0));
				RefreshPlayTimeText(0);

				//メニュー
				saveMenuItem.Enabled = false;

				//角度
				angleWindow.setAngle(0);

				//シークバー
				seekBar.Value = 0;
				seekBar.Maximum = 0;

				//操作パネル
				seekPanel.Enabled = false;
			}
			else
			{
			//開いている
				//サイドバー
				updateInfoList(info.param);

				//ステータスバー
				RefreshPlayTimeText(info.param.time);

				//メニュー
				saveMenuItem.Enabled = Enabled;

				//角度
				angleWindow.setAngle(info.param.angle);

				//シークバー
				if (seekBar.Maximum != info.logLength - 1)
				{
					seekBar.Value = 0;
					seekBar.Maximum = info.logLength - 1;
				}

				//操作パネル
				seekPanel.Enabled = true;
			}

		}

		/**
		 * マウス座標を更新する。
		 */
		private void showMousePos(Point point)
		{
			CursorPosStatLabel.Text = string.Format("{0}mm, {1}mm", point.X, point.Y);
		}

        /*
         * 選択されている制御点を更新する
         */
        private void UpdateSelectedControlPoint(ControlPoint cp)
        {
            controlPointProperty.SelectedObject = cp;
        }


		/**
		 * サイドバーのパラメータリストを更新する。
		 * 引数にnullを与えると、クリアのみ行う。
		 * @param param 更新対象のXYAngle
		 */
		private int updateInfoList(XYAngle param)
		{
			infoList.Items.Clear();
			if (param != null)
			{
				string[] item2 = { "X", param.point.X.ToString() };
				infoList.Items.Add(new ListViewItem(item2));
				string[] item3 = { "Y", param.point.Y.ToString() };
				infoList.Items.Add(new ListViewItem(item3));
				string[] item4 = { "Angle", param.angle.ToString() };
				infoList.Items.Add(new ListViewItem(item4));
			}

			infoList.Refresh();

			return 0;
		}

		/**
		 * プレイ時間表示を更新する。
		 * @param time プレイ時間(ミリ秒)
		 */
		private void RefreshPlayTimeText(int time){
			string hhmmss = string.Format("{0:D2}:{1:D2}:{2:D2}:{3:D2}",
				(time / 60 / 60 / 60), //時間
				((time % (60 * 60 * 60)) / 60 / 60), //分
				((time % (60 * 60)) / 60), //秒
				(time % 60)); //ミリ病

			//表示更新
			playTimeStatLabel.Text = hhmmss;
		}

		private void saveMenuItem_Click(object sender, EventArgs e)
		{
			SaveFileDialog saveDlg = new SaveFileDialog();

			saveDlg.FileName = "*.png";
			saveDlg.Filter = "pngファイル(*.png)|*.png";
			saveDlg.Title = "可視化結果を保存";

			if (saveDlg.ShowDialog() == DialogResult.OK)
			{
				//ファイルが選択された
				try
				{
					courseView.SaveAllTrack(saveDlg.FileName);
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
		}





		private void debugButton_Click(object sender, EventArgs e)
		{
            courseView.ExportVirtualLine(this);
		}

		public void putDebugText(String text)
        {
			if (debugWindow == null)
			{
                debugWindow = new DebugOut();
                debugWindow.Owner = this;
			}
			debugWindow.putText(text);
			debugWindow.Show();
		}

        /**
         * バーチャルラインファイルを開く
         */
        private void OpenVirtualLineMenuItem_Click(object sender, EventArgs e)
        {
			OpenFileDialog openDlg = new OpenFileDialog();

			openDlg.FileName = "*.vl";
			openDlg.Filter = "仮想ライン ファイル(*.vl)|*.vl|すべてのファイル(*.*)|*.*";
			openDlg.Title = "仮想ラインファイルを開く";

			if (openDlg.ShowDialog() == DialogResult.OK) {
				//ファイルが選択された
				try
				{
					courseView.LoadVirtualLineFile(openDlg.FileName);
				}
				catch (UserCanceledException) {
					//ユーザーキャンセルなのでなにもしない
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
        }

        private void SaveVirtualLineMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveDlg = new SaveFileDialog();

            saveDlg.FileName = "*.vl";
            saveDlg.Filter = "仮想ライン ファイル(*.vl)|*.vl|すべてのファイル(*.*)|*.*";
            saveDlg.Title = "仮想ラインファイルを保存";

            if (saveDlg.ShowDialog() == DialogResult.OK)
            {
                //ファイルが選択された
                try
                {
                    courseView.SaveVirtualLineFile(saveDlg.FileName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void ExportVirtualLine_Click(object sender, EventArgs e)
        {
            courseView.ExportVirtualLine(this);
        }

        private void controlPointProperty_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            courseView.UpdateView();
        }



	}
}
