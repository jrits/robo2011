## run_research(3)
## アルゴリズム3. 時間差分データの移動平均値がある閾値よりも大きければ黒区間、でなければ灰色区間。

## Author: nseo <nseo@NSEO>
## Created: 2009-07-23

Ts = 4; % 4ms
nsample = 500;
GRAYLINE_MOVINGAVG_DIFFTHRESH = 1.03;

daq = read_light_daq('light_daq/daq.csv');

daq = [daq(1); daq];
diff = abs(daq(2:end) - daq(1:end-1));

figure; plot((0:length(diff)-1).*Ts, diff); title('Diff'); xlabel('Time (ms)'); ylabel('Difference');
ylim([0,10]);
print -deps 'daq_diff.eps'; print -djpg 'daq_diff.jpg';

[MEAN, STD, X] = gen_movingavg(diff, nsample-1, nsample);

MEAN = [ones(1,nsample-1)*0 MEAN];
figure; plot(MEAN); title('Moving Average of Diff'); %xlabel('Sampling Time'); ylabel('Average');
print -deps 'daq_movingavg_diff.eps'; print -djpg 'daq_movingavg_diff.jpg';

BLACKLINE = MEAN >= GRAYLINE_MOVINGAVG_DIFFTHRESH;
figure; plot(BLACKLINE); title('BlackLine Detection with Algorithm 3'); xlabel('Sampling Time'); ylabel('Result');
ylim([0 1.1]);
print -deps 'daq_isblack_algorithm3.eps'; print -djpg 'daq_isblack_algorithm3.jpg';
