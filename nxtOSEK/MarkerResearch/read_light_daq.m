% read_light_daq - Read a light field from a csv file created by Daq. 
%
% Synopsis
%   M = read_light_daq(filename)
%
% Description
%   Read a light field data from a csv file created by Daq.
%
% Inputs ([]s are optional)
%   (string) filename The filename to be read
%
% Outputs ([]s are optional)
%   (vector) M        N x 1 array where N is the number of data.
%
% See also
%   csvread

% Authors
%   Naotoshi Seo
%
% License
%   RITS license?
%
% Changes
%   07/14/2009  First Edition

function [ret] = read_light_daq(filename)
tmpfile = [filename, '.tmp'];
system(['tail +2 ', filename, ' > ', tmpfile]);
csv = dlmread(tmpfile, ',');
ret = csv(:,10);
delete(tmpfile);
endfunction
