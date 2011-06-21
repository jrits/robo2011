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

## run_read

## Author: nseo <nseo@NSEO>
## Created: 2009-07-15

white = read_light_daq('light_daq/white.csv');
gray = read_light_daq('light_daq/gray.csv');
black = read_light_daq('light_daq/black.csv');
%black_zigzag = read_light_daq('light_daq/black_zigzag.csv');
%blackgray_zigzag = read_light_daq('light_daq/blackgray_zigzag.csv');
% gray_zigzag = read_light_daq('light_daq/gray_zigzag.csv');
