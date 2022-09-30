
clear;

ground1 = importdata('ground1.mat'); ground1 = ground1(:,2);
ground2 = importdata('ground2.mat'); ground2 = ground2(:,2);
ground3 = importdata('ground3.mat'); ground3 = ground3(:,2);
ground4 = importdata('ground4.mat'); ground4 = ground4(:,2);
ground5 = importdata('ground5.mat'); ground5 = ground5(:,2);

plot(ground1)