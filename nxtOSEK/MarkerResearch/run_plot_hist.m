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

## run_hist

## Author: nseo <nseo@NSEO>
## Created: 2009-07-15

run_read_light_daq;
%figure; hist(white, 20); print -dpng 'white_hist.png';
%figure; hist(gray, 20); print -dpng 'gray_hist.png';
%figure; hist(black, 20); print -dpng 'black_hist.png';
figure; hist(white, 20); hold on; hist(gray, 20); hist(black, 20); 
print -deps 'all_hist.eps'; print -dpng 'all_hist.jpg';
figure; hist(black_zigzag, 200);  xlim([500, 750]);
print -deps 'black_zigzag_hist.eps'; print -dpng 'black_zigzag_hist.jpg';
%figure; hist(gray_zigzag, 100); 
%print -deps 'gray_zigzag_hist.eps'; print -dpng 'gray_zigzag_hist.png';
