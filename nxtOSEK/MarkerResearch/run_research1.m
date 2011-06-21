## run_research(3)
## アルゴリズム1. 移動平均値がある閾値よりも大きければ黒区間、でなければ灰色区間。

## Author: nseo <nseo@NSEO>
## Created: 2009-07-23

Ts = 4; % 4ms
nsample = 500;
GRAYLINE_THRESH = 575;

daq = read_light_daq('light_daq/daq.csv');

[MEAN, STD, X] = gen_movingavg(daq, nsample-1, nsample);

MEAN = [ones(1,nsample-1)*0 MEAN];
figure; plot(MEAN); title('Moving Average'); %xlabel('Sampling Time'); ylabel('Average');
ylim([500 700]);
print -deps 'daq_movingavg.eps'; print -djpg 'daq_movingavg.jpg';

BLACKLINE = MEAN >= GRAYLINE_THRESH;
figure; plot(BLACKLINE); title('BlackLine Detection with Algorithm 1'); xlabel('Sampling Time'); ylabel('Result');
ylim([0 1.1]);
print -deps 'daq_isblack_algorithm1.eps'; print -djpg 'daq_isblack_algorithm1.jpg';
