clear;

ground1 = importdata('ground1.mat'); ground1 = ground1(3:end-1,2);
ground2 = importdata('ground3.mat'); ground2 = ground2(3:end,2);
ground3 = importdata('ground4.mat'); ground3 = ground3(3:end,2);
ground4 = importdata('ground5.mat'); ground4 = ground4(3:end,2);

% sensor recorded a value every .25 seconds
t = .1*linspace(1, length(ground1)/4, 147);

clf; hold on;
plot(t, ground1, "*");
plot(t, ground2, "*");
plot(t, ground3, "*");
plot(t, ground4, "*");

ylabel('Voltage (mV)');
xlabel('Time (seconds)');
legend('Trial 1', 'Trial 2', 'Trial 3', 'Trial 4');