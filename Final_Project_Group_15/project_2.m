% project_2.m Plot script

% Define parameters for gen_state and measure_state
sigma_x = 1;
sigma_y = 1;
pv0 = [0; 100; 0; 100]; % initial position and velocity ([xpos, xvel, ypos, yvel])
t = 0:1:60;
noise_lvls = [25; 10; 25; 10]; % measurement noise levels ([xpos, xvel, ypos, yvel])

% Define system matrix for measure_state
C = eye(4);
    
% Generate state trajectory using gen_state
states = zeros(length(pv0), length(t));
states(:,1) = pv0;
for i=2:length(t)
    % table used to assign each 4 by 1 column output of gen_state to each
    % column of states
    states(:,i) = table(gen_state(t(i), states(:,i-1), sigma_x, sigma_y)).Var1(:,i);
end
    
% Measure states with noise using measure_state
meas = zeros(size(C,1), length(t));
for i=1:length(t)
    meas(:,i) = measure_state(C, noise_lvls, states(:,i));
end
 
% Plot state trajectory and measured states 
% X vs Y Pos
figure;
subplot(2,2,1); 
plot(states(1,:), states(3,:), '-r', meas(1,:), meas(3,:), '.b');
xlabel('X position (m)'); 
ylabel('Y position (m)'); 
legend('True', 'Measured');

% X pos vs time
subplot(2,2,2); 
plot(t, states(1,:), '-r', t, meas(1,:), '.b');
xlabel('Time (s)'); 
ylabel('X position (m)'); 
legend('True', 'Measured');

% Y pos vs time
subplot(2,2,3); 
plot(t, states(3,:), '-r', t, meas(3,:), '.b');
xlabel('Time (s)'); 
ylabel('Y position (m)'); 
legend('True', 'Measured');

% Velocity vs time
subplot(2,2,4); 
plot(t, states(2,:), '-r'); 
hold on; 
plot(t, meas(2,:), '.b');
hold on; 
plot(t, states(4,:), '-g'); 
hold on;
plot(t, meas(4,:), '.c'); 
hold off;
xlabel('Time (s)'); 
ylabel('Velocity (m/s)'); 
legend('X true', 'X measured', 'Y true', 'Y measured');
