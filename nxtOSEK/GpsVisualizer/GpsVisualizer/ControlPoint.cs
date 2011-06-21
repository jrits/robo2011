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

		//�e�̃��[�U�[�R���g���[��
		CourseView owner;

		//����_�̃h���b�O�ړ��̂��߂̕ϐ�
		private bool isDraggable = false;
		private Point dragBasePoint = new Point();

		//�l�C�e�B�u���W<->�X�P�[�����O�σs�N�Z�����W�̕ϊ��̂��߂̕ϐ�
		private Point pixelLocationOffset = new Point();

        //����_���(�G�N�X�|�[�g�p)
        private Point nativeLocation; //�l�C�e�B�u�̃��P�[�V����(mm)
        private int nativeAngle = NO_ANGLE; //�l�C�e�B�u�̊p�x
        private int forward = FORWARD_DEFAULT; //���{�q�̈ړ����x
        private int allowableError = ALLOWABLE_ERROR_DEFAULT; //���e�덷
        private bool slowdown = SLOWDOWN_DEFAULT; //�X���[�_�E���X�L�����g�p���邩

        //�R�[�X���ĕ`�悷�邩(����_�쐬���͍쐬���̐ӔC�ŌĂ΂�邽��false)
        private bool enableCourseDraw = false;

        [Category("���{�b�g����")]
        [DescriptionAttribute("���ۂ̃R�[�X�ɂ�����X���W��mm�P�ʂŎw�肵�܂��B")]
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

        [Category("���{�b�g����")]
        [DescriptionAttribute("���ۂ̃R�[�X�ɂ�����Y���W��mm�P�ʂŎw�肵�܂��B")]
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

        [Category("���{�b�g����"),
        DescriptionAttribute("���̐���_�ɓ��B��A���{�b�g�������������w�肵�܂��B�������w�肹���A���̂܂܎��̐���_��ڎw���đ��s������ꍇ�́A-1���w�肵�܂��B"),
        DefaultValue(NO_ANGLE),
        Editor(typeof(AngleEditor), typeof(UITypeEditor))]
        public int NativeAngle
        {
            set
            {
                if (value < 0)
                {
                    //�}�C�i�X�̒l�Ȃ�������w�肵�Ȃ��̂�-1���Z�b�g
                    nativeAngle = NO_ANGLE;
                }
                else
                {
                    //360�x�ȏ�͖��Ӗ��Ȃ̂�mod�ł܂�߂�
                    nativeAngle = value % 360;
                }
            }
            get
            {
                return nativeAngle;
            }
        }

        [Category("���{�b�g����"),
         DescriptionAttribute("�ړ����x��0-100�͈̔͂Ŏw�肵�܂��B"),
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

        [Category("���{�b�g����"),
         DescriptionAttribute("����_�ɓ��B�����Ƃ݂Ȃ��덷��mm�P�ʂŎw�肵�܂��B"),
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

        [Category("���{�b�g����"),
         DescriptionAttribute("���̐���_���玟�̐���_�Ɍ������Ƃ��ASlowdown Skill���g�p���邩���w�肵�܂��B"),
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

		//�s�N�Z���P�ʂ̐���_���W��Ԃ�
		public Point ScaledLocation
		{
			get
			{
				return EtCourseScaler.NativeToPixelInvY(nativeLocation);
			}
		}


		//�R���X�g���N�^
		//�X�P�[�����O�̂�����Ȃ��l�C�e�B�u��X��Y���W(�P�ʁFmm)�A�X�P�[�����O�䗦��n��
		public ControlPoint(int nativeX, int nativeY, CourseView Owner)
		{
			this.owner = Owner;
            this.nativeLocation.X = nativeX;
            this.nativeLocation.Y = nativeY;
            this.NativeAngle = -1;

			//�C�x���g�n���h���̐ݒ�
			this.Resize += new System.EventHandler(OnResize);
			this.Move += new System.EventHandler(OnMove);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(OnMouseMove);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(OnMouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(OnMouseUp);
			
			//�����X�^�C���̐ݒ�
			this.BackColor = Color.White;
			this.BorderStyle = BorderStyle.FixedSingle;
			this.Size = new Size(DEFAULT_POINT_SIZE, DEFAULT_POINT_SIZE);
			this.Text = "";

			//�X�P�[������W�̐ݒ�
			UpdateControlLocation();
		}

		//�I�t�Z�b�g���Čv�Z����
		private void UpdatePixelOffset()
		{
			pixelLocationOffset.X = (this.Width / 2);
			pixelLocationOffset.Y = (this.Height / 2);
		}

		//�C�x���g �n���h��
		//�R���g���[���T�C�Y�ύX���͍��W�I�t�Z�b�g���Čv�Z
		private void OnResize(object sender, EventArgs e)
		{
			UpdatePixelOffset();
		}

		//�C�x���g �n���h��
		//�R���g���[���ړ����ɂ̓l�C�e�B�u��X��Y���W���Čv�Z
		private void OnMove(object sender, EventArgs e)
		{
			//�X�P�[���O�̐���_���W�����߂�
			RecalculateNativePosition();
		}

		//�C�x���g �n���h��
		//�R���g���[���̃h���b�O&�h���b�v������
		private void OnMouseDown(object sender, MouseEventArgs e)
		{
			//�I�����ꂽ���Ƃɂ���
			owner.ControlPointSelected(this);

			//���N���b�N��
			if (e.Button == MouseButtons.Left)
			{
                //����_�̃h���b�O�J�n
				isDraggable = true;
				Debug.WriteLine("�h���b�O�J�n");

                //�R�[�X�̕`���}��
                this.EnableCourseDraw = false;

				//�h���b�O�J�n���̃J�[�\���ʒu��ێ�
				dragBasePoint.X = e.X;
				dragBasePoint.Y = e.Y;
			}

		}
		private void OnMouseMove(object sender, MouseEventArgs e)
		{
			//���N���b�N��
			if (e.Button == MouseButtons.Left)
			{
				//�h���b�O�\��Ԃ�
				if (!isDraggable) return;

				this.Left += e.X - dragBasePoint.X;
				this.Top += e.Y - dragBasePoint.Y;
			}
		}
		private void OnMouseUp(object sender, MouseEventArgs e)
		{
			//���N���b�N��
			if (e.Button == MouseButtons.Left)
			{
                //����_�h���b�O�I��
				isDraggable = false;

                //���W�ݒ�
				RecalculateNativePosition();
                UpdateControlLocation();

                //�R�[�X�̕`��}��������
                this.EnableCourseDraw = true;

                //�`��OK�Ȃ�(OK�m�肾��)
                if (this.EnableCourseDraw)
                {
                    //�R�[�X���ĕ`��
                    owner.UpdateView();
                }

				Debug.WriteLine("�h���b�O�I��");
			}
        }

        public void ControlPointActivated()
        {
            Debug.WriteLine("active");
            //���납����
            this.BackColor = Color.Red;
        }

        public void ControlPointDeactivated()
        {
            Debug.WriteLine("deactive");

            //���납����
            this.BackColor = Color.White;
        }

		//����_���R�[�X�O�ɔ�яo�Ă�����ǂ�
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

		//�R���g���[���ʒu����l�C�e�B�u�ʒu���v�Z
		private void RecalculateNativePosition()
		{
			Point pixelLocation = this.Location;
			pixelLocation.Offset(pixelLocationOffset.X, pixelLocationOffset.Y);

            this.NativeX = EtCourseScaler.PixelToNativeInvY(pixelLocation).X;
            this.NativeY = EtCourseScaler.PixelToNativeInvY(pixelLocation).Y;
        }

		//�l�C�e�B�u�ʒu����R���g���[���ʒu���Čv�Z���Đݒ�
		private void UpdateControlLocation()
		{
			Point pixelLocation = EtCourseScaler.NativeToPixelInvY(nativeLocation);
			pixelLocation.Offset(-pixelLocationOffset.X, -pixelLocationOffset.Y);
            this.Location = pixelLocation;
		}
	}

    //�v���p�e�B�O���b�h�ɕ\������v���p�e�B�����肷��
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

    //�p�x�v���p�e�B�G�f�B�^
    internal class AngleEditor : UITypeEditor
    {
        AngleEditorControl ui = null;

        public override bool GetPaintValueSupported(ITypeDescriptorContext context)
        {
            //�v���p�e�B�̕`��̓u���E�U���ɂ���Ă��炤
            //���O�ł͏����Ȃ�
            return false;
        }

        //�v���p�e�B�ҏW���h���b�v�_�E���X�^�C���ɐݒ�
        public override UITypeEditorEditStyle GetEditStyle(
         ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.DropDown;
        }

        //�v���p�e�B�ҏW�J�n
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider sp, object value)
        {
            Debug.WriteLine("a");

            //�G�f�B�^�T�[�r�X���擾
            IWindowsFormsEditorService edSvc =
                (IWindowsFormsEditorService)sp.GetService(typeof(IWindowsFormsEditorService));

            //�T�[�r�X���擾�ł��Ȃ������牽�����Ȃ��ł��̂܂ܖ߂�
            if (edSvc == null)
                return value;

            //(���쐬�Ȃ�)�G�f�B�^�R���g���[�����쐬
            if (ui == null)
            {
                ui = new AngleEditorControl();
            }

            //ui���v���p�e�B�ɂ��킹�ď�����
            if (value is int)
            {
                ui.Angle = (int)value;
            }
            else
            {
                return value;
            }

            //ui���h���b�v�_�E���Ƃ��ĕ\��
            edSvc.DropDownControl(ui);

            // �X�V���ꂽ�l��Ԃ�
            return ui.Angle;
        }

    }



}
