balancer_param.c パラメータ調整方法 by 瀬尾 直利

1. octave のインストール

Matlab を購入する予算がないため、フリーの matlab クローンである octave を使用した。
http://www.gnu.org/software/octave/download.html
より octave をダウンロード。
# Windows 版の場合、cygwin の octave よりも Octave Forge 版の方が良い。

インストール時は、『Octave Forge』にチェックを入れ、全ての toolbox (ライブラリ) をインストールする。
# 全てである必要はないかもしれないが、全て入れてほいたほうが後で面倒が少ないと思われる。

2. おおまかな手順

param_plant.m で定義されている車体重量などのパラメータを、計測した値に置き換え、実行。

　% NXTway-GS Parameters
　m = 0.03;						% wheel weight [kg]
　R = 0.04;						% wheel radius [m]
　Jw = m * R^2 / 2;				% wheel inertia moment [kgm^2]
　M = 0.6;						% body weight [kg]
　W = 0.14;						% body width [m]
　D = 0.04;						% body depth [m]
　H = 0.144;					% body height [m]
　L = H / 2;					% distance of the center of mass from the wheel axle [m]
　Jpsi = M * L^2 / 3;			% body pitch inertia moment [kgm^2]
　Jphi = M * (W^2 + D^2) / 12;	% body yaw inertia moment [kgm^2]
　fm = 0.0022;					% friction coefficient between body & DC motor
　fw = 0;						% friction coefficient between wheel & floor

 octave> param_plant

param_controller.m を実行し、k_f など balancer_param.c 内のパラメータに一致する変数の値を参照する。

 octave> param_controller
 octave> k_f
 k_f =

   -0.83508  -34.18958   -1.09951   -2.81409

3. 計測方法

m % wheel weight
	車輪の重さをばね秤を使用して計測
R % wheel radius
	車輪半径を定規/ものさしを使用して計測
M % body weight
	車体の重さをばね秤を使用して計測
W % body width
	車体の幅(右車輪中心から左車輪中心)を定規/ものさしを使用して計測
D % body depth
	車体の奥行(NXT本体の厚み)を定規/ものさしを使用して計測
H % body height
	車体の高さを定規/ものさしを使用して計測。車軸から重心までの距離 x 2。
L % distance of the center of mass from the wheel axle [m]
	車軸から重心までの距離を定規/ものさしを使用して計測

参考:nxtway_gs\docs\english\NXTway-GS Model-Based Design.pdf　p6,7.

4. バランスパラメータ

パラメータ	機能
k_f			サーボ制御用フィードバックゲイン
k_i			サーボ制御用積分ゲイン
k_thetadot	速度目標用ゲイン
k_phidot	回転速度目標用ゲイン
k_sync		車輪同期P制御用ゲイン
a_b			ローパスフィルタ係数（バッテリ電圧平均値計算用）
a_d			ローパスフィルタ係数（速度ノイズ抑制用）
a_r			ローパスフィルタ係数（オーバーシュート抑制用）
a_gc		ローパスフィルタ係数（ジャイロオフセット較正用）
a_gd		ローパスフィルタ係数（ジャイロドリフト対策用）
pwm_gain	摩擦補償器ゲイン
pwm_offset	摩擦補償器オフセット
dst_thr		障害物回避距離
turn_angle	自律走行時右回転角度
