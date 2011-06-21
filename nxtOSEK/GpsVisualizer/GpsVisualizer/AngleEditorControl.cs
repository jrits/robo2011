using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace GpsVisualizer
{
    public partial class AngleEditorControl : UserControl
    {
        private int angle = -1;
        private const int ANGLE_DISABLED = -1;

        private Bitmap canvas = null;
        private Graphics g = null;
        
        private bool isDrag;

        public AngleEditorControl()
        {
            InitializeComponent();
            canvas = new Bitmap(picAngleCircle.ClientSize.Width, picAngleCircle.ClientSize.Height);
            picAngleCircle.Image = canvas;
                
            g = Graphics.FromImage(picAngleCircle.Image);
            
            redrawAngle();
        }

        public int Angle
        {
            set
            {
                if (value < 0)
                {
                    //マイナスの値なら向きを指定しないので-1をセット
                    angle = ANGLE_DISABLED;

                    //チェックボックスを制御
                    chkDisable.Checked = true;
                }
                else
                {
                    //360度以上は無意味なのでmodでまるめる
                    angle = value % 360;

                    //チェックボックスを制御
                    chkDisable.Checked = false;
                }

                redrawAngle();
            }
            get
            {
                if (chkDisable.Checked)
                {
                    return ANGLE_DISABLED;
                }
                else
                {
                    return angle;
                }
            }
        }

        private void updateAngle(Point mousePos)
        {
            //マウス位置から角度を求める

            //中心座標
            Point center = new Point(
                canvas.Width / 2,
                canvas.Height / 2);
            
            //三角形
            int x, y;
            x = mousePos.X - center.X;
            y = -(mousePos.Y - center.Y); //Windowsの原点は左上、Gpsは左下なのでYを反転

            angle = (int)radianToDegree(Math.Atan2(y, x));

            while (angle < 0) angle += 360;

            /*
            Debug.WriteLine("updateAngle---");
            Debug.WriteLine("x: " + x);
            Debug.WriteLine("y: " + y);
            Debug.WriteLine("angle: " + angle);
            Debug.WriteLine("--------------");
            */
        }

        //角度を描画しなおす
        private void redrawAngle()
        {
            Debug.WriteLine(chkDisable.Checked);
            if (chkDisable.Checked)
            {
                g.Clear(SystemColors.ButtonFace);

                g.DrawString(angle + "°", DefaultFont, SystemBrushes.ButtonHighlight, new Point(1, 1));
                g.DrawString(angle + "°", DefaultFont, SystemBrushes.ButtonShadow, new Point(0, 0));
            }
            else
            {
                Pen pen = SystemPens.InfoText;
                Brush brush = SystemBrushes.InfoText;

                Point from = new Point(), to = new Point();
                int radius;

                //描画先(マージン差し引き後)rect
                Rectangle drawRect = new Rectangle(new Point(2, 2),
                    new Size(canvas.Width - 4, canvas.Height - 4));

                //GPSの座標系にあわせて角度補正
                //(真右がGPSの0度、反時計回りで増加)
                int drawAngle = angle + 90;

                //中心座標計算
                from.X = canvas.Width / 2;
                from.Y = canvas.Height / 2;

                //半径計算
                radius = drawRect.Width / 2;

                //座標計算
                to.X = (int)(radius * Math.Sin(degreeToRadian(drawAngle)));
                to.Y = (int)(radius * Math.Cos(degreeToRadian(drawAngle)));
                to.Offset(from);

                //背景をクリア
                g.Clear(SystemColors.Info);
                //円を描く
                g.DrawEllipse(pen, drawRect);
                g.FillEllipse(brush, from.X - 3, from.Y - 3, 7, 7);
                //角度を描く
                g.DrawLine(pen, from, to);

                //角度文字も描画
                g.DrawString(angle + "°", DefaultFont, brush, new Point(0, 0));
            }

            picAngleCircle.Refresh();
        }

        /**
         * デグリーからラジアンに変換する。
         */
        private double degreeToRadian(double deg)
        {
            return (deg / (180 / Math.PI));
        }

        /**
         * ラジアンからデグリーに変換する。
         */
        private double radianToDegree(double rad)
        {
            return (rad / (Math.PI / 180));
        } 

        private void picAngleCircle_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDrag)
            {
                updateAngle(new Point(e.X, e.Y));
                redrawAngle();
            }
        }

        private void picAngleCircle_MouseDown(object sender, MouseEventArgs e)
        {
            if(e.Button == MouseButtons.Left)
                isDrag = true;
        }

        private void picAngleCircle_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                isDrag = false;
        } 

        private void chkDisable_CheckedChanged(object sender, EventArgs e)
        {
            picAngleCircle.Enabled = !chkDisable.Checked;

            redrawAngle();
        }

    }
}
