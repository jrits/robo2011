using System;
using System.Collections.Generic;
using System.Text;

namespace GpsVisualizer
{
	class UserCanceledException : Exception
	{
		public UserCanceledException() { }
		public UserCanceledException(string message) : base(message) { }
		public UserCanceledException(string message, Exception inner) : base(message) { }
	}
}
