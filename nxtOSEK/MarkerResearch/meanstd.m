% meanstd - 平均値と偏差値の計算
%
% Synopsis
%   [mu, std] = meanstd(data)
%
% Description
%   平均値と偏差値の計算
%
% Inputs ([]s are optional)
%   (vector) data     1 x N. データ群。
%
% Outputs ([]s are optional)
%   (scalar) mu       平均
%   (scalar) std      偏差
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
%   2009-07-15  First Edition
function [mu, std] = meanstd(data)
mu = mean(data);
std = sqrt(cov(data));
endfunction
