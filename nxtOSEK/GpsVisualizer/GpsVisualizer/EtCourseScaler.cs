using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GpsVisualizer
{
	class EtCourseScaler
	{
		//�����̃R�[�X�T�C�Y(�~���w��) 5460mm�~3640mm
		private const int COURSE_WIDTH = 5460;
		private const int COURSE_HEIGHT = 3640;

		//�R�[�X�摜�̃X�P�[���{��(1��1px=1mm, 1/3��1px=3mm)
		private const double SCALE = 1.0 / 3.0;

		//�R�[�X�̃X�P�[�����O�σT�C�Y���s�N�Z���P�ʂŕԂ�
		public static Size GetPixelCourseSize()
		{
			return new Size((int)(COURSE_WIDTH * SCALE), (int)(COURSE_HEIGHT * SCALE));
		}

		//�R�[�X�̎��ۂ̃T�C�Y��mm�P�ʂŕԂ�
		public static Size GetNativeCourseSize()
		{
			return new Size(COURSE_WIDTH, COURSE_HEIGHT);
		}

		//���ۂ̃T�C�Y(mm)����s�N�Z���ɕϊ�
		public static int NativeToPixel(int native)
		{
			return (int)(native * SCALE);
		}
		public static Point NativeToPixel(Point native)
		{
			native.X = NativeToPixel(native.X);
			native.Y = NativeToPixel(native.Y);
			return native;
		}

		//���ۂ̃T�C�Y(mm)����s�N�Z���ɕϊ�(Y���W���]�t��)
		public static Point NativeToPixelInvY(Point native)
		{
			native.X = NativeToPixel(native.X);
			native.Y = NativeToPixel(-native.Y);
			return native;
		}

		//�s�N�Z��������ۂ̃T�C�Y(mm)�ɕϊ�
		public static int PixelToNative(int pixel)
		{
			return (int)(pixel / SCALE);
		}
		public static Point PixelToNative(Point pixel)
		{
			pixel.X = PixelToNative(pixel.X);
			pixel.Y = PixelToNative(pixel.Y);
			return pixel;
		}

		//�s�N�Z��������ۂ̃T�C�Y(mm)�ɕϊ�(Y���W���]�t��)
		public static Point PixelToNativeInvY(Point pixel)
		{
			pixel.X = PixelToNative(pixel.X);
			pixel.Y = PixelToNative(-pixel.Y);
			return pixel;
		}
	}
}
