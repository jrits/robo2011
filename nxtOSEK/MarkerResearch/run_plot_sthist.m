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

## run_plot_sthist

## Author: nseo <nseo@NSEO>
## Created: 2009-07-22

run_read_light_daq;

[H, Y, X] = gen_sthist(black_zigzag, 0, 100, 550:5:700);
figure; plot_sthist(H, Y, X); xlabel('Time (Sampling Time)'); ylabel('Light Amplitude');
print -deps 'black_zigzag_sthist.eps'; print -djpg 'black_zigzag_sthist.jpg';

[H, Y, X] = gen_sthist(blackgray_zigzag, 0, 100, 550:5:700);
figure; plot_sthist(H, Y, X); xlabel('Time (Sampling Time)'); ylabel('Light Amplitude');
print -deps 'blackgray_zigzag_sthist.eps'; print -djpg 'blackgray_zigzag_sthist.jpg';




