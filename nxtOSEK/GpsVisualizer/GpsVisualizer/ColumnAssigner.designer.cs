namespace GpsVisualizer
{
	partial class ColumnAssigner
	{
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label5 = new System.Windows.Forms.Label();
			this.paramAngleCombo = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.paramYCombo = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.paramXCombo = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.cancelButton = new System.Windows.Forms.Button();
			this.okButton = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.label5);
			this.groupBox1.Controls.Add(this.paramAngleCombo);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.paramYCombo);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.paramXCombo);
			this.groupBox1.Location = new System.Drawing.Point(8, 48);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(368, 72);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "パラメータの対応(&P)";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(248, 24);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(56, 12);
			this.label5.TabIndex = 5;
			this.label5.Text = "Angle(&A)：";
			// 
			// paramAngleCombo
			// 
			this.paramAngleCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.paramAngleCombo.FormattingEnabled = true;
			this.paramAngleCombo.Location = new System.Drawing.Point(248, 40);
			this.paramAngleCombo.Name = "paramAngleCombo";
			this.paramAngleCombo.Size = new System.Drawing.Size(112, 20);
			this.paramAngleCombo.TabIndex = 4;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(120, 24);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(33, 12);
			this.label3.TabIndex = 3;
			this.label3.Text = "Y(&Y)：";
			// 
			// paramYCombo
			// 
			this.paramYCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.paramYCombo.FormattingEnabled = true;
			this.paramYCombo.Location = new System.Drawing.Point(128, 40);
			this.paramYCombo.Name = "paramYCombo";
			this.paramYCombo.Size = new System.Drawing.Size(112, 20);
			this.paramYCombo.TabIndex = 2;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(8, 24);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(33, 12);
			this.label2.TabIndex = 1;
			this.label2.Text = "X(&X)：";
			// 
			// paramXCombo
			// 
			this.paramXCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.paramXCombo.FormattingEnabled = true;
			this.paramXCombo.Location = new System.Drawing.Point(8, 40);
			this.paramXCombo.Name = "paramXCombo";
			this.paramXCombo.Size = new System.Drawing.Size(112, 20);
			this.paramXCombo.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.AutoEllipsis = true;
			this.label1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(368, 32);
			this.label1.TabIndex = 2;
			this.label1.Text = "各パラメータを、対応するcsvの列に割り当ててください。(X,Yの単位はmm、Angleの単位は度です。)";
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(288, 128);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(88, 21);
			this.cancelButton.TabIndex = 3;
			this.cancelButton.Text = "キャンセル";
			this.cancelButton.UseVisualStyleBackColor = true;
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(192, 128);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(88, 21);
			this.okButton.TabIndex = 4;
			this.okButton.Text = "OK";
			this.okButton.UseVisualStyleBackColor = true;
			// 
			// ColumnAssigner
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(386, 153);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ColumnAssigner";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.Text = "パラメータの割り当て";
			this.Load += new System.EventHandler(this.ParamAssigner_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.ComboBox paramAngleCombo;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ComboBox paramYCombo;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ComboBox paramXCombo;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button okButton;
	}
}