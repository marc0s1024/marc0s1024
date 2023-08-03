function Q_hat = RLS_estimation(A,R,y)
% Inputs:   A: the state model matrix
%           R: the forgetting factor matrix
%           y: matrix of output vectors, where each column is a measurement vector
% Outputs:  Q_hat: matrix of estimated state vectors, where each column is an estimate

% Initialize variables
k = size(y,2); % number of measurements
n = size(A,1); % number of states
P(:,:,1) = eye(n); % initial covariance matrix
Q_hat(:,1) = zeros(n,1); % initial estimate (not output)

% Iterate through measurements
for i = 1:k
    % Compute Kalman gain
    K = P(:,:,i)*A.'/(A*P(:,:,i)*A.'+R);
    
    % Compute covariance matrix
    P(:,:,i+1) = R\eye(n) - K*A/R*P(:,:,i);
    
    % Compute estimated state vector
    Q_hat(:,i+1) = Q_hat(:,i) + K*(y(:,i) - A*Q_hat(:,i));
end

% Remove initial estimate from output
Q_hat = Q_hat(:,2:end);
end
