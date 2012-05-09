using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace Monitor_Application
{
	public class LogWriter : TextWriter
	{
		
		private ListBox list;
		private StringBuilder content = new StringBuilder();
		private FileStream file;
		public void ListBoxWriter(ListBox list)
		{
			this.list = list;
		}

		public void FileWriter(FileStream fileStream)
		{
			this.file = fileStream;
		}

		public override void Write(char value)
		{
			base.Write(value);
			content.Append(value);

			if (file != null)
			{
				byte[] ba = BitConverter.GetBytes(value);
				file.Write(ba, 0, 1);

				file.Flush();
			}

			if (value == '\n')
			{

				MainDialog.InvokeControlAction<ListBox>
					(list, ls => ls.Items.Add(content.ToString()));
//					(lastSessionErrors, lse => lse.Text = program["Info", "Program Name"] + " closed.");
	
				content = new StringBuilder();
			}
		}

		public override Encoding Encoding
		{
			get { return System.Text.Encoding.UTF8; }
		}
		
	}
}
