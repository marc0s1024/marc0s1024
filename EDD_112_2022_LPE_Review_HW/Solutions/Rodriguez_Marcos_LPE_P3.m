%% Part 03 Marcos Rodriguez

clear
close all
clc

% initialize symbolic variables
syms x q y t

% initialize equation 1, differentiate, and display derivative
f(x) = sin(2*(x^3));
diffEq1 = diff(f(x),x);
disp('The derivative of Equation 1 with respect to x is:');
disp(diffEq1);

% initialize equation 2, differentiate twice, and display double derivative
f(x,q) = (3*q*x^3)-(exp(x*q));
diffEq2a = diff(f(x,q),x);
diffEq2b = diff(diffEq2a,q);
disp('The double derivative of Equation 2 first with respect to x and then with respect to q is:');
disp(diffEq2b);

% initialize equation 3, integrate, and display integral
f(y) = 6*(y^2)*cos(2*(y^3));
intEq3 = int(f(y),y);
disp('The integral of Equation 3 with respect to y is:');
disp(intEq3);

% initialize equation 4, integrate from 5 to 10, and display definite integral
g(x) = sin(x)*cos(x);
intEq4 = int(g(x),x,5,10);
disp('The definite integral of Equation 4 with respect to x from 5 to 10 is:');
disp(intEq4);

% initialize equation 5, integrate from 0 to x, and display definite integral
f(t) = exp(t)/(1+exp(2*t));
intEq5 = int(f(t),t,0,x);
disp('The definite integral of Equation 5 with respect to t from 0 to x is:');
disp(intEq5);