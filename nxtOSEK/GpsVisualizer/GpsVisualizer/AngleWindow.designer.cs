namespace GpsVisualizer
{
	partial class AngleWindow
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
			this.components = new System.ComponentModel.Container();
			this.anglePicture = new System.Windows.Forms.PictureBox();
			this.opacityTrack = new System.Windows.Forms.TrackBar();
			this.toolTip = new System.Windows.Forms.ToolTip(this.components);
			((System.ComponentModel.ISupportInitialize)(this.anglePicture)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.opacityTrack)).BeginInit();
			this.SuspendLayout();
			// 
			// anglePicture
			// 
			this.anglePicture.Location = new System.Drawing.Point(0, 0);
			this.anglePicture.Name = "anglePicture";
			this.anglePicture.Size = new System.Drawing.Size(128, 128);
			this.anglePicture.TabIndex = 0;
			this.anglePicture.TabStop = false;
			// 
			// opacityTrack
			// 
			this.opacityTrack.AutoSize = false;
			this.opacityTrack.Location = new System.Drawing.Point(56, 152);
			this.opacityTrack.Margin = new System.Windows.Forms.Padding(1);
			this.opacityTrack.Maximum = 100;
			this.opacityTrack.Minimum = 30;
			this.opacityTrack.Name = "opacityTrack";
			this.opacityTrack.Size = new System.Drawing.Size(64, 16);
			this.opacityTrack.TabIndex = 1;
			this.opacityTrack.TickFrequency = 10;
			this.toolTip.SetToolTip(this.opacityTrack, "ウィンドウの透明度を指定します。");
			this.opacityTrack.Value = 30;
			this.opacityTrack.ValueChanged += new System.EventHandler(this.opacityTrack_ValueChanged);
			// 
			// AngleForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(248, 281);
			this.ControlBox = false;
			this.Controls.Add(this.opacityTrack);
			this.Controls.Add(this.anglePicture);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AngleForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.Text = "Angle";
			((System.ComponentModel.ISupportInitialize)(this.anglePicture)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.opacityTrack)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PictureBox anglePicture;
		private System.Windows.Forms.TrackBar opacityTrack;
		private System.Windows.Forms.ToolTip toolTip;
	}
}