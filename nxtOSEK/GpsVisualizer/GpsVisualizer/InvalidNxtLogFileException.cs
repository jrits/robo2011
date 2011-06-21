using System;
using System.Collections.Generic;
using System.Text;

namespace GpsVisualizer
{
	class InvalidNxtLogFileException : Exception
	{
		public InvalidNxtLogFileException() {}
		public InvalidNxtLogFileException(string message) : base(message) {}
		public InvalidNxtLogFileException(string message, Exception inner) : base(message) {}
	}
}
