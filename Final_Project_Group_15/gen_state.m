function [x] = gen_state(t, pv0, sigma_x, sigma_y)
%   Inputs:  t = time spacing vector
%            pv0 = column vector containing initial x pos, y pos, x vel, y vel
%            sigma_x = x multiplier
%            sigma_y = y multiplier
%   Output:  x = time-varying state vector

% state transition matrix
A = [1 t 0 0; 0 1 0 0; 0 0 1 t; 0 0 0 1];

% input matrix
B = [0 0; sigma_x 0; 0 sigma_y; 0 0];

% length variable
time_span = 0:1:60;
time_len = length(time_span);

% generate randominput vectors with dimension 2
randInp = randn(2, time_len);

% initialize time-varying state vector
x = zeros(4, time_len);

% fill state vector
x(:,1) = pv0;

% iterate states through time range
for i = 2:time_len
    x(:,i) = A*x(:,i-1) + B*randInp(:,i);
end

end