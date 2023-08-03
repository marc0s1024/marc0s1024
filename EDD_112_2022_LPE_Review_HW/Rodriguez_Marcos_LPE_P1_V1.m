%% Part 1-a: Marcos Rodriguez

clear
close all
clc

% initialize variables (user input & array)
numpoints = input('Enter the number of points to plot: ');
arr = randi([90,100],[3,numpoints]);

% initialize and format plot
figure('Name','Sea Monkeys Groovin','NumberTitle','on');
scatter3(arr(1,:),arr(2,:),arr(3,:),15,'MarkerEdgeColor','black','MarkerFaceColor',[0.9290 0.6940 0.1250]);
axis([0,100,0,100,0,100]);
set(gca,'Color',[0 0.4470 0.7410],'XTick',0:20:100,'YTick',0:20:100,'FontSize',8);
xlabel('x, in cm');
ylabel('y, in cm');
zlabel('z, in cm');
title('Sea Monkeys in a Fish Tank at Step: 0','FontSize',8,'FontWeight','Bold');
axis square;
grid on;

% moving points on x,y,or z axis 400 times
for i=0:400
    pause(0.1);
    rand = randi(3);
    switch rand
        case 1
            for j=1:numpoints
                if arr(1,j) == 0
                    arr(1,j) = arr(1,j) + randi([0 1]);
                elseif arr(1,j) == 100
                    arr(1,j) = arr(1,j) + randi([-1 0]);
                else
                    arr(1,j) = arr(1,j) + randi([-1 1]);
                end
            end
        case 2
            for k=1:numpoints
                if arr(2,k) == 0
                    arr(2,k) = arr(2,k) + randi([0 1]);
                elseif arr(2,k) == 100
                    arr(2,k) = arr(2,k) + randi([-1 0]);
                else
                    arr(2,k) = arr(2,k) + randi([-1 1]);
                end
            end
        case 3
            for m=1:numpoints
                if arr(3,m) == 0
                    arr(3,m) = arr(3,m) + randi([0 1]);
                elseif arr(3,m) == 100
                    arr(3,m) = arr(3,m) + randi([-1 0]);
                else
                    arr(3,m) = arr(3,m) + randi([-1 1]);
                end
            end
    end
    % plotting points after each iteration
    scatter3(arr(1,:),arr(2,:),arr(3,:),15,'MarkerEdgeColor','black','MarkerFaceColor',[0.9290 0.6940 0.1250]);
    axis([0,100,0,100,0,100]);
    set(gca,'Color',[0 0.4470 0.7410],'XTick',0:20:100,'YTick',0:20:100,'FontSize',8);
    xlabel('x, in cm');
    ylabel('y, in cm');
    zlabel('z, in cm');
    title('Sea Monkeys in a Fish Tank at Step: ',num2str(i),'FontSize',8,'FontWeight','bold');
    axis square;
    grid on;
end