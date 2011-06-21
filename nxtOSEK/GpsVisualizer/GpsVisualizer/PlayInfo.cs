using System;
using System.Collections.Generic;
using System.Text;

namespace GpsVisualizer
{
	public enum PlayStatus
	{
		Close,
		Play,
		Stop,
	}

	public struct PlayInfo
	{
		public PlayInfo(PlayStatus Status, int length, XYAngle Param) : this() { logLength = length; status = Status; param = Param; }
		public PlayStatus status;
		public XYAngle param;
		public int logLength;
	}
}
