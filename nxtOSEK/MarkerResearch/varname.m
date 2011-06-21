% varname - Get a name of a variable
%
% Synopsis
%   [name] = varname(var)
%
% Description
%   Get a name of a variable 
%
% Inputs ([]s are optional)
%   (variable) var    Any kinds of a variable
%
% Outputs ([]s are optional)
%   (string) name     The variable name
%
% Example
%   >> a = 1;
%   >> varname(a)
%   ans
%       'a'
function [name] = varname(var)
name = inputname(1);
endfunction