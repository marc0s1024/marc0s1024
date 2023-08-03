% project_1.m Plot script

% initial positions and velocities ([xpos, xvel, ypos, yvel])
pv0 = [0; 100; 0; 100];

% intitialize figure
figure
hold on

% B matrix values (if set to small (<1), small deviations from true path)
sigma_x = 1;
sigma_y = 1;

% plot 2D trajectory
for t= 0:1:60
    x = gen_state(t, pv0, sigma_x, sigma_y);     
    plot(x(1,:), x(3,:));
end

hold off

% Add plot labels and legend
xlabel('X Position (m)')
ylabel('Y Position (m)')
title('2D Aircraft Trajectory')
