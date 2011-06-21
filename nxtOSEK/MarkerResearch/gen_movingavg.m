% gen_movingavg - 入力信号から移動平均信号を生成する
%
% Synopsis
%   [MEAN, STD, X] = gen_movingavg(x, noverlap, nsample)
%
% Description
%   入力信号から移動平均信号を生成する
%
% Inputs ([]s are optional)
%   (vector) x        1 x N. 入力信号
%   (scalar) noverlap 次のフレーム(平均を取る範囲)とオーバーラップするサンプルの数
%   (scalar) nsample  各フレーム(平均を取る範囲)のサンプルの数
%
% Outputs ([]s are optional)
%   (vector) MEAN     1 x K. 移動平均信号
%   (vector) STD      1 x K. 移動偏差信号
%   (vector) X        1 x K. プロットする際にしようするx軸の値。各フレームの中央の添字
%
% Examples
%
% See also

% Authors
%   Naotoshi Seo <sonots(at)sonots.com>
%
% License
%   MIT License.
%
% Changes
%   2009-07-23  First Edition
function [MEAN, STD, X] = gen_movingavg(x, noverlap, nsample)
 N = length(x);
 MEAN = [];
 STD = [];
 pos = 1;
 while (pos+nsample <= N)
     frame = x(pos:pos+nsample-1);
     pos = pos + (nsample - noverlap);
	 MEAN = [MEAN mean(frame)];
	 STD = [STD sqrt(cov(frame))]; % biased variance
 end
 X = round(nsample/2):(nsample-noverlap):N-1-round(nsample/2);
endfunction


