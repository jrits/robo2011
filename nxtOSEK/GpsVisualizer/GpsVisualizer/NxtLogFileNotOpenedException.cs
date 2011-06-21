using System;
using System.Collections.Generic;
using System.Text;

namespace GpsVisualizer
{
	class NxtLogFileNotOpenedException : Exception
	{
		public NxtLogFileNotOpenedException() { }
		public NxtLogFileNotOpenedException(string message) : base(message) { }
		public NxtLogFileNotOpenedException(string message, Exception inner) : base(message) { }
	}
}
