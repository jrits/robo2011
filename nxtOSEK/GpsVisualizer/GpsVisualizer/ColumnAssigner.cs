using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GpsVisualizer
{
	public partial class ColumnAssigner : Form
	{
		public ColumnAssigner(string[] column)
		{
			InitializeComponent();
			InitializeGUI(column); 
		}

		public int xColumnIndex
		{
			get
			{
				return paramXCombo.SelectedIndex;
			}
		}

		public int yColumnIndex
		{
			get
			{
				return paramYCombo.SelectedIndex;
			}
		}

		public int angleColumnIndex
		{
			get
			{
				return paramAngleCombo.SelectedIndex;
			}
		}

		private void InitializeGUI(string[] column) {
			//ドロップダウンリストに選択肢を追加
			paramXCombo.Items.AddRange(column);
			paramYCombo.Items.AddRange(column);
			paramAngleCombo.Items.AddRange(column);

			//ドロップダウンリストの1つ目の要素を選択しておく
			paramXCombo.SelectedIndex = 0;
			paramYCombo.SelectedIndex = 0;
			paramAngleCombo.SelectedIndex = 0;

		}

		private void ParamAssigner_Load(object sender, EventArgs e)
		{

		}
	}
}
