## run_plot_data

## Author: nseo <nseo@NSEO>
## Created: 2009-07-22

daq = read_light_daq('light_daq/daq.csv');

figure; plot(daq); ylim([500, 750]);
print -deps 'daq.eps'; print -djpg 'daq.jpg';
