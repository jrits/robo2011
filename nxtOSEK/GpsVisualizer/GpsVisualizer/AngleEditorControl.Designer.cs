namespace GpsVisualizer
{
    partial class AngleEditorControl
    {
        /// <summary> 
        /// 必要なデザイナ変数です。
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

        #region コンポーネント デザイナで生成されたコード

        /// <summary> 
        /// デザイナ サポートに必要なメソッドです。このメソッドの内容を 
        /// コード エディタで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.chkDisable = new System.Windows.Forms.CheckBox();
            this.picAngleCircle = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.picAngleCircle)).BeginInit();
            this.SuspendLayout();
            // 
            // chkDisable
            // 
            this.chkDisable.AutoSize = true;
            this.chkDisable.Location = new System.Drawing.Point(3, 130);
            this.chkDisable.Name = "chkDisable";
            this.chkDisable.Size = new System.Drawing.Size(93, 16);
            this.chkDisable.TabIndex = 0;
            this.chkDisable.Text = "指定しない(&D)";
            this.chkDisable.UseVisualStyleBackColor = true;
            this.chkDisable.CheckedChanged += new System.EventHandler(this.chkDisable_CheckedChanged);
            // 
            // picAngleCircle
            // 
            this.picAngleCircle.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picAngleCircle.Location = new System.Drawing.Point(3, 3);
            this.picAngleCircle.Name = "picAngleCircle";
            this.picAngleCircle.Size = new System.Drawing.Size(121, 121);
            this.picAngleCircle.TabIndex = 2;
            this.picAngleCircle.TabStop = false;
            this.picAngleCircle.MouseMove += new System.Windows.Forms.MouseEventHandler(this.picAngleCircle_MouseMove);
            this.picAngleCircle.MouseDown += new System.Windows.Forms.MouseEventHandler(this.picAngleCircle_MouseDown);
            this.picAngleCircle.MouseUp += new System.Windows.Forms.MouseEventHandler(this.picAngleCircle_MouseUp);
            // 
            // AngleEditorControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.picAngleCircle);
            this.Controls.Add(this.chkDisable);
            this.Name = "AngleEditorControl";
            this.Size = new System.Drawing.Size(127, 147);
            ((System.ComponentModel.ISupportInitialize)(this.picAngleCircle)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox chkDisable;
        private System.Windows.Forms.PictureBox picAngleCircle;

    }
}
