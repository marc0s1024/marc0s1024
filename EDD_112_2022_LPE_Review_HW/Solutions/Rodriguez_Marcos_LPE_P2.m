%% Part 02 Marcos Rodriguez

clear
close all
clc

% initialize variables
Vo = input('Enter the Vo in Volts: ');
R = input('Enter the R in kiloOhms: ');
L = input('Enter the L in milliHenries: ');
C = input('Enter C in picoFarads: ');

% convert inputs to proper units
r = R*10^3;
l = L*10^-3;
c = C*10^-12;

% initialize equations and list
wo = 1/(sqrt(l*c));
deltaW = r/l;
Q = wo/deltaW;
w = 0:10*10^6;
absVr = abs((Vo*r)./(sqrt((r^2)+(((w*l)-(1./(w*c))).^2))));

% plot the calulated absVr and label plot
plot(w,absVr);
axis auto;
title(['|Vr| as a function of \omega (V_0 = ',num2str(Vo),', R = ',num2str(R),'k\Omega, L = ',num2str(L),'mH, C = ',num2str(C),'pF)'])
ylabel('|Vr|, in volts')
xlabel('\omega, in rad/sec')
grid on

% display values for ω0, Δω, and Q
disp(['The value for ω0 is: ',num2str(wo)]);
disp(['The value for Δω is: ',num2str(deltaW)]);
disp(['The value for Q is: ',num2str(Q)]);

% determine if system is overdamped, underdamped, or critically damped
if Q < 0.5
    disp('The system is overdamped.')
elseif Q > 0.5
    disp('The system is underdamped.')
elseif Q == 0.5
    disp('The system is critically damped.')
end




    
