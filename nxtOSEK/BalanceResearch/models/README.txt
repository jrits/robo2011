balancer_param.c �p�����[�^�������@ by ���� ����

1. octave �̃C���X�g�[��

Matlab ���w������\�Z���Ȃ����߁A�t���[�� matlab �N���[���ł��� octave ���g�p�����B
http://www.gnu.org/software/octave/download.html
��� octave ���_�E�����[�h�B
# Windows �ł̏ꍇ�Acygwin �� octave ���� Octave Forge �ł̕����ǂ��B

�C���X�g�[�����́A�wOctave Forge�x�Ƀ`�F�b�N�����A�S�Ă� toolbox (���C�u����) ���C���X�g�[������B
# �S�Ăł���K�v�͂Ȃ���������Ȃ����A�S�ē���Ăق����ق�����Ŗʓ|�����Ȃ��Ǝv����B

2. �����܂��Ȏ菇

param_plant.m �Œ�`����Ă���ԑ̏d�ʂȂǂ̃p�����[�^���A�v�������l�ɒu�������A���s�B

�@% NXTway-GS Parameters
�@m = 0.03;						% wheel weight [kg]
�@R = 0.04;						% wheel radius [m]
�@Jw = m * R^2 / 2;				% wheel inertia moment [kgm^2]
�@M = 0.6;						% body weight [kg]
�@W = 0.14;						% body width [m]
�@D = 0.04;						% body depth [m]
�@H = 0.144;					% body height [m]
�@L = H / 2;					% distance of the center of mass from the wheel axle [m]
�@Jpsi = M * L^2 / 3;			% body pitch inertia moment [kgm^2]
�@Jphi = M * (W^2 + D^2) / 12;	% body yaw inertia moment [kgm^2]
�@fm = 0.0022;					% friction coefficient between body & DC motor
�@fw = 0;						% friction coefficient between wheel & floor

 octave> param_plant

param_controller.m �����s���Ak_f �Ȃ� balancer_param.c ���̃p�����[�^�Ɉ�v����ϐ��̒l���Q�Ƃ���B

 octave> param_controller
 octave> k_f
 k_f =

   -0.83508  -34.18958   -1.09951   -2.81409

3. �v�����@

m % wheel weight
	�ԗւ̏d�����΂˔����g�p���Čv��
R % wheel radius
	�ԗ֔��a���K/���̂������g�p���Čv��
M % body weight
	�ԑ̂̏d�����΂˔����g�p���Čv��
W % body width
	�ԑ̂̕�(�E�ԗ֒��S���獶�ԗ֒��S)���K/���̂������g�p���Čv��
D % body depth
	�ԑ̂̉��s(NXT�{�̂̌���)���K/���̂������g�p���Čv��
H % body height
	�ԑ̂̍������K/���̂������g�p���Čv���B�Ԏ�����d�S�܂ł̋��� x 2�B
L % distance of the center of mass from the wheel axle [m]
	�Ԏ�����d�S�܂ł̋������K/���̂������g�p���Čv��

�Q�l:nxtway_gs\docs\english\NXTway-GS Model-Based Design.pdf�@p6,7.

4. �o�����X�p�����[�^

�p�����[�^	�@�\
k_f			�T�[�{����p�t�B�[�h�o�b�N�Q�C��
k_i			�T�[�{����p�ϕ��Q�C��
k_thetadot	���x�ڕW�p�Q�C��
k_phidot	��]���x�ڕW�p�Q�C��
k_sync		�ԗ֓���P����p�Q�C��
a_b			���[�p�X�t�B���^�W���i�o�b�e���d�����ϒl�v�Z�p�j
a_d			���[�p�X�t�B���^�W���i���x�m�C�Y�}���p�j
a_r			���[�p�X�t�B���^�W���i�I�[�o�[�V���[�g�}���p�j
a_gc		���[�p�X�t�B���^�W���i�W���C���I�t�Z�b�g�r���p�j
a_gd		���[�p�X�t�B���^�W���i�W���C���h���t�g�΍��p�j
pwm_gain	���C�⏞��Q�C��
pwm_offset	���C�⏞��I�t�Z�b�g
dst_thr		��Q���������
turn_angle	�������s���E��]�p�x
