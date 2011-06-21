% gen_hist - ヒストグラムを取得する
%
% Synopsis
%   [hist, xout] = gen_hist(x, intervals)
%
% Description
%   ヒストグラムを取得する
%
% Inputs ([]s are optional)
%   (vector) x         入力信号
%   (vector) intervals ヒストグラムの値間隔
%
% Outputs ([]s are optional)
%   (vector) hist      ヒストグラム
%   (vector) xout      プロットする際に使用するx軸の値。intervals
%
% Examples
%    x = rand(1,100);
%    intervals = 0:0.1:1;
%    [hist, xout] = gen_hist(x, intervals);
%    plot(xout, hist);
%
% See also

% Authors
%   Naotoshi Seo <sonots(at)sonots.com>
%
% License
%   MIT License.
%
% Changes
%   2009-07-22  First Edition
function [hist, xout] = gen_hist(x, intervals)
hist = [];
for i = 1:length(intervals)-1
	hist(i) = sum((x >= intervals(i)) & (x < intervals(i+1)));
end
xout = intervals(1:end-1);
endfunction
