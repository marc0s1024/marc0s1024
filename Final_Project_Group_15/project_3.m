% project_3.m Plot script

% Define simulation parameters for gen_state, measure_state, and
% RLS_estimation
pv0 = [0; 100; 0; 100]; % initial position and velocity ([xpos, xvel, ypos, yvel])
sigma_x = 5; % Standard deviation of x velocity noise
sigma_y = 5; % Standard deviation of y velocity noise
noise_lvls = [25; 10; 25; 10]; % measurement noise levels ([xpos, xvel, ypos, yvel])

% Generate true state sequence with gen_state
for t= 0:1:60
    true_states = gen_state(t, pv0, sigma_x, sigma_y);
end

% Measure true state sequence with measure_state
measured_states = measure_state(eye(4), noise_lvls, true_states);

% Apply RLS filter to measured state sequence using RLS_estimation
A = eye(4);
R = 0.999; % Forgetting factor set to 0.999 (values from 0.999 to 1.01 work)
est_states = RLS_estimation(A, R, measured_states);

% Plot results
figure;
plot(true_states(1,:), true_states(3,:), '-r', 'LineWidth', 1.5);
hold on;
plot(measured_states(1,:), measured_states(3,:), 'xk');
hold on;
plot(est_states(1,:), est_states(3,:), '-g', 'LineWidth', 1.5);
xlabel('X position (m)');
ylabel('Y position (m)');
legend('True positions', 'Measured positions', 'Estimated positions');
