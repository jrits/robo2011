マーカ解析 Octave ユーティリティ
=========================================================================
1.はじめに
マーカ解析を行うための Octave プログラム群です。

2.ファイル構成
MarkerResearch フォルダ下は以下のファイルから構成されています。

 meanstd.m                  データの平均値、標準偏差を計算するサブルーチン。
 read_light_daq.m           BluetoothDaq、LightLogger ユーティリティから生成されたログファイルを読みこむサブルーチン。
 gen_movingavg.m            移動平均信号を生成するためのサブルーチン。
 
実験用
 run_plot_data.m            データをただプロットするスクリプト。light_daq/daq.csv を読み込む。
 run_research1.m            アルゴリズム１の実験用スクリプト。light_daq/daq.csv を読み込む。
 run_research2.m            アルゴリズム２の実験用スクリプト。light_daq/daq.csv を読み込む。
 run_research3.m            アルゴリズム３の実験用スクリプト。light_daq/daq.csv を読み込む。

その他サンプル
 run_read_light_daq.m       read_light_daq の使い方例

過去物
 gen_hist.m                 入力信号のヒストグラムを生成するためのサブルーチン。# 現在使用していない
 gen_sthist.m               短時間ヒストグラムを生成するためのサブルーチン。# 現在使用していない
 run_plot_hist.m            ヒストグラムデータをプロットするスクリプト。# 現在使用していない
 run_plot_sthist.m          短時間ヒストグラムデータをプロットするスクリプト。# 現在使用していない

3. 使用方法
 light_daq/daq.csv に調査対象のログを上書きする。
 octave> run_plot_data % 光センサデータをプロットする。
 octave> run_research1 % アルゴリズム１の実験
 octave> run_research2 % アルゴリズム２の実験
 octave> run_research3 % アルゴリズム３の実験
 