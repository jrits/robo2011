using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GpsVisualizer
{
	public class XYAngle
	{
		private int _time;
		private Point _point;
		private int _angle;

		public XYAngle(int Time, int X, int Y, int Angle) { _time = Time; _point = new Point(X, Y); _angle = Angle; }
		public int time {
			set{
				_time = value;
			}
			get {
				return _time;
			}
		}
		public Point point {
			set{
				_point = value;
			}
			get {
				return _point;
			}
		}
		public int angle {
			set{
				_angle = value;
			}
			get {
				return _angle;
			}
		}
	}
}
