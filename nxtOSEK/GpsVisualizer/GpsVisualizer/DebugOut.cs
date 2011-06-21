using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GpsVisualizer
{
	public partial class DebugOut : Form
	{
		public DebugOut()
		{
			InitializeComponent();
		}

		public DebugOut(String text) : this()
		{
			putText(text);
		}

		public void putText(String text)
		{
			debugText.Text = text;
		}

		private void DebugOut_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (e.CloseReason == CloseReason.UserClosing)
			{
				this.Hide();
				e.Cancel = true;
			}
        }

        protected override bool ProcessDialogKey(Keys keyData)
        {
            switch (keyData)
            {
                case Keys.A | Keys.Control:
                    Control activeCtl = this.ActiveControl;
                    while (activeCtl is ContainerControl)
                    {
                        activeCtl = ((ContainerControl)activeCtl).ActiveControl;
                    }

                    if (activeCtl is TextBox)
                    {
                        TextBox txt = (TextBox)activeCtl;
                        txt.SelectionStart = 0;
                        txt.SelectionLength = txt.Text.Length;
                        return true;
                    }
                    break;
            }
            return base.ProcessDialogKey(keyData);
        }

        private void btnCopy_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(debugText.Text);
        }
	}
}