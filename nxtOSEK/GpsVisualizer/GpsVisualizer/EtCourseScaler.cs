using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GpsVisualizer
{
	class EtCourseScaler
	{
		//現実のコースサイズ(ミリ指定) 5460mm×3640mm
		private const int COURSE_WIDTH = 5460;
		private const int COURSE_HEIGHT = 3640;

		//コース画像のスケール倍率(1で1px=1mm, 1/3で1px=3mm)
		private const double SCALE = 1.0 / 3.0;

		//コースのスケーリング済サイズをピクセル単位で返す
		public static Size GetPixelCourseSize()
		{
			return new Size((int)(COURSE_WIDTH * SCALE), (int)(COURSE_HEIGHT * SCALE));
		}

		//コースの実際のサイズをmm単位で返す
		public static Size GetNativeCourseSize()
		{
			return new Size(COURSE_WIDTH, COURSE_HEIGHT);
		}

		//実際のサイズ(mm)からピクセルに変換
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

		//実際のサイズ(mm)からピクセルに変換(Y座標反転付き)
		public static Point NativeToPixelInvY(Point native)
		{
			native.X = NativeToPixel(native.X);
			native.Y = NativeToPixel(-native.Y);
			return native;
		}

		//ピクセルから実際のサイズ(mm)に変換
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

		//ピクセルから実際のサイズ(mm)に変換(Y座標反転付き)
		public static Point PixelToNativeInvY(Point pixel)
		{
			pixel.X = PixelToNative(pixel.X);
			pixel.Y = PixelToNative(-pixel.Y);
			return pixel;
		}
	}
}
