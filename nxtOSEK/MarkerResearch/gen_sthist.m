## Copyright (C) 2009 nseo
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <http://www.gnu.org/licenses/>.

## gen_sthist

## Author: nseo <nseo@NSEO>
## Created: 2009-07-22

% Example
%
%  x = rand(1,1000);
%  intervals = 0:0.1:1;
%  [H, Y, X] = gen_stshift(x, 0, 100, intervals);
%  hist = gen_hist(x, intervals);
%  plot(intervals(1:end-1), hist);
function [H, Y, X] = gen_sthist(x, noverlap, nsample, intervals)
 N = length(x);
 H = [];
 pos = 1;
 while (pos+nsample <= N)
     frame = x(pos:pos+nsample-1);
     pos = pos + (nsample - noverlap);
	 h = gen_hist(frame, intervals);
	 h = h(:); % make sure to be a column vector
     H = [H h];
 end
 Y = intervals(1:end-1);
 X = round(nsample/2):(nsample-noverlap):N-1-round(nsample/2);
endfunction