
clear;

ground1 = importdata('ground1.mat'); ground1 = ground1(3:end,2);
ground2 = importdata('ground2.mat'); ground2 = ground2(3:end,2);
ground3 = importdata('ground3.mat'); ground3 = ground3(3:end,2);
ground4 = importdata('ground4.mat'); ground4 = ground4(3:end,2);
ground5 = importdata('ground5.mat'); ground5 = ground5(3:end,2);

clf; hold on;
plot(ground1, "b*");
plot(ground2, "r*");
plot(ground3, "g*");
plot(ground4, "c*");
plot(ground5, "k*");