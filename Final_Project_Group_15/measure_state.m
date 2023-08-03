function [D] = measure_state(C, noise_lvls, state_vectors)
%   Inputs:  C = general sensor matrix 
%            noise_lvls = vector containing noise levels used for diagonals
%            of matrix D
%            state_vectors = holds sequence of state vectors to be measured
%   Output:  D = time-varying measurment vector

% initialize diagonal matrix D with noise_lvls
D = diag(noise_lvls);

% measure state vectors (output)
D = C*state_vectors + mvnrnd(zeros(size(C,1),1), D, size(state_vectors,2))';

% check noise_lvls > 0
if any(noise_lvls <= 0)
    error('Noise levels must be greater than 0');
end
end