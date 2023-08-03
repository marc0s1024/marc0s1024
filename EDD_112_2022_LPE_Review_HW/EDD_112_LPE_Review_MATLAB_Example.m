%% EDD 112 - 2022 LPE Question 
%
% This program is provided to help with the Problem 1-a of the LPE Review Homework.
% In this example file, 20 random points are plotted on a 50x50 grid. the points
% then take 35 random steps along the x-axis in either direction.
%
% Written By: Koenraad Gieskes
% Written For: EDD 112

%% Initializing variables
 clear
 close all
 clc
 
 Numpoints= 20;
 Array = randi([0 50],[2 Numpoints]);
  
 %% Initial Plot
 
 figure('Name','Moving Points','NumberTitle','on');
 scatter(Array(1,:),Array(2,:));
 axis([0 50 0 50]);
 axis square;
 
 %% Making the points move side to side
 
 for i=0:35
    pause(0.1);
    for j=1:Numpoints
        if Array(1,j)== 0
            Array(1,j) = Array(1,j)+randi([0 1]);
        elseif Array(1,j) == 50
            Array(1,j) = Array(1,j)+randi([-1 0]);
        else
            Array(1,j) = Array(1,j)+randi([-1 1]);
        end
        
    end
    scatter(Array(1,:),Array(2,:));
    axis([0 50 0 50]);
    axis square;
 end