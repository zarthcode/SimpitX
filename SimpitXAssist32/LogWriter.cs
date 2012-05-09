using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace SimpitXAssist32
{
	public class LogWriter : TextWriter
	{
		
		private StringBuilder content = new StringBuilder();
		private FileStream stream;

		public void SetWriter(FileStream fileStream)
		{
			this.stream = fileStream;
		}

		public override void Write(char value)
		{
			base.Write(value);
			content.Append(value);

			Byte[] b = BitConverter.GetBytes(value);
			stream.WriteByte(b[0]);
			stream.Flush();
	
		}

		public override Encoding Encoding
		{
			get { return System.Text.Encoding.UTF8; }
		}
		
	}
}
