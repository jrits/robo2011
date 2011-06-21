using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace GpsVisualizer
{
	public partial class AngleWindow : Form
	{
		//定数
		//---------------------------------------------------
		//角度を表す線分の太さ
		private const int LINE_WIDTH = 1;
		//角度を表す線分の色
		private Color LINE_COLOR = Color.Black;
		//線分の終端
		private CustomLineCap END_CAP = new AdjustableArrowCap(3, 3);

		//メンバ変数
		//---------------------------------------------------
		//角度画像
		private Bitmap angleImage = null;
		//角度画像のサイズ(X,Y同値)
		private int imageSize;

		public AngleWindow(int windowSize)
		{
			this.imageSize = windowSize;
			InitializeComponent();
			InitializeGUI();
		}

		/**
		 * AngleWindowのGUIを初期化する。
		 */
		private void InitializeGUI()
		{
			anglePicture.Size = new Size(imageSize, imageSize);
			this.ClientSize = new Size(imageSize, imageSize + opacityTrack.Height);
			opacityTrack.Left = 0;
			opacityTrack.Top = imageSize;
			opacityTrack.Width = imageSize;
			opacityTrack.Value = 100;

			setAngle(0);
		}

		/**
		 * ウィンドウの透明度を変化させる。
		 */
		private void opacityTrack_ValueChanged(object sender, EventArgs e)
		{
			this.Opacity = opacityTrack.Value / 100.0;
		}

		/**
		 * 角度をセットする。
		 * セットされた角度に基づき、表示を更新する。
		 * @param angle 角度(degree)
		 */
		public void setAngle(double angle){
			//半径
			int radius = imageSize / 2;
			//中心点
			Point from = new Point(radius, radius);
			//角度を表す線分の端
			Point to = new Point();
			//実際に描く線分の角度
			double drawAngle;

			//角度表示用Bitmapが未作成なら作成する
			if (angleImage == null)
            {
				angleImage = new Bitmap(anglePicture.Width, anglePicture.Height);
				anglePicture.Image = angleImage;
			}

			//GPSの座標系にあわせて角度補正
			//(真右がGPSの0度、反時計回りで増加)
			drawAngle = angle + 270;

			//座標計算
			to.X = (int)(radius * Math.Sin(degreeToRadian(drawAngle)));
			to.Y = (int)(radius * Math.Cos(degreeToRadian(drawAngle)));
			to.Offset(from);

			//角度線描画準備
			Graphics g = Graphics.FromImage(anglePicture.Image);
			Pen pen = new Pen(LINE_COLOR, LINE_WIDTH);
			pen.CustomEndCap = END_CAP;

			//背景をクリアして線分描画
			g.Clear(Color.White);
			g.DrawLine(pen, from, to);

			//角度文字も描画
			g.DrawString(angle + "°", DefaultFont, new SolidBrush(LINE_COLOR), new Point(0, 0));

			//再表示
			anglePicture.Refresh();
		}

		/**
		 * デグリーからラジアンに変換する。
		 */
		private double degreeToRadian(double deg) {
			return (deg / (180 / Math.PI));
		} 
	}
}
