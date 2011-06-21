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
                    //�}�C�i�X�̒l�Ȃ�������w�肵�Ȃ��̂�-1���Z�b�g
                    angle = ANGLE_DISABLED;

                    //�`�F�b�N�{�b�N�X�𐧌�
                    chkDisable.Checked = true;
                }
                else
                {
                    //360�x�ȏ�͖��Ӗ��Ȃ̂�mod�ł܂�߂�
                    angle = value % 360;

                    //�`�F�b�N�{�b�N�X�𐧌�
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
            //�}�E�X�ʒu����p�x�����߂�

            //���S���W
            Point center = new Point(
                canvas.Width / 2,
                canvas.Height / 2);
            
            //�O�p�`
            int x, y;
            x = mousePos.X - center.X;
            y = -(mousePos.Y - center.Y); //Windows�̌��_�͍���AGps�͍����Ȃ̂�Y�𔽓]

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

        //�p�x��`�悵�Ȃ���
        private void redrawAngle()
        {
            Debug.WriteLine(chkDisable.Checked);
            if (chkDisable.Checked)
            {
                g.Clear(SystemColors.ButtonFace);

                g.DrawString(angle + "��", DefaultFont, SystemBrushes.ButtonHighlight, new Point(1, 1));
                g.DrawString(angle + "��", DefaultFont, SystemBrushes.ButtonShadow, new Point(0, 0));
            }
            else
            {
                Pen pen = SystemPens.InfoText;
                Brush brush = SystemBrushes.InfoText;

                Point from = new Point(), to = new Point();
                int radius;

                //�`���(�}�[�W������������)rect
                Rectangle drawRect = new Rectangle(new Point(2, 2),
                    new Size(canvas.Width - 4, canvas.Height - 4));

                //GPS�̍��W�n�ɂ��킹�Ċp�x�␳
                //(�^�E��GPS��0�x�A�����v���ő���)
                int drawAngle = angle + 90;

                //���S���W�v�Z
                from.X = canvas.Width / 2;
                from.Y = canvas.Height / 2;

                //���a�v�Z
                radius = drawRect.Width / 2;

                //���W�v�Z
                to.X = (int)(radius * Math.Sin(degreeToRadian(drawAngle)));
                to.Y = (int)(radius * Math.Cos(degreeToRadian(drawAngle)));
                to.Offset(from);

                //�w�i���N���A
                g.Clear(SystemColors.Info);
                //�~��`��
                g.DrawEllipse(pen, drawRect);
                g.FillEllipse(brush, from.X - 3, from.Y - 3, 7, 7);
                //�p�x��`��
                g.DrawLine(pen, from, to);

                //�p�x�������`��
                g.DrawString(angle + "��", DefaultFont, brush, new Point(0, 0));
            }

            picAngleCircle.Refresh();
        }

        /**
         * �f�O���[���烉�W�A���ɕϊ�����B
         */
        private double degreeToRadian(double deg)
        {
            return (deg / (180 / Math.PI));
        }

        /**
         * ���W�A������f�O���[�ɕϊ�����B
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
