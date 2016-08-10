function T_func = FiniteDiffBrassRod(P_in,Tamb,K_c,k,c,e)

dx = 0.03; %m
dt = 10; %s
iterations = 360; 
a = 0.0111; %m
X = 0:dx:0.3;
rho = 8500; %kg/m^3
Boltzmann = 5.67*10^-8;
D = (k*dt)/(c*rho*dx^2)*(diag([-1 -2*ones(1,length(X)-2) -1]) + diag([1 ones(1,length(X)-2)], 1) + diag([ones(1,length(X) -2) 1], -1)) + (eye(length(X)));
T = zeros(iterations+1, length(X));
T(1,:) = 293*ones(1,length(X));
for i=1:iterations
    v = -2*dt*(K_c*(T(i,:)'-Tamb) + e*Boltzmann*(T(i,:)'.^4-Tamb^4))/(rho*a*c);
    v(1,1) = v(1,1) +  (P_in*dt)/(c*rho*pi*a^2*dx);
    
    v(length(X),1) = v(length(X),1) - dt*(K_c*(T(i,length(X))-Tamb) + e*Boltzmann*(T(i,length(X))^4-Tamb^4))/(rho*dx*c);
    T(i+1,:) = (D*T(i,:)' + v);
end
%{
mesh(X,linspace(1,iterations*dt,iterations+1),T)
xlabel('Position on the rod (m)')
ylabel('Time elapsed (s)')
zlabel('Temperature (K)')
title('Andys boss-ass graph')
%}
Xvect = zeros(1,length(X)*(iterations+1));
for i = 0:iterations
    Xvect(1,length(X)*i+1:length(X)*i+length(X)) = X;
end

Yvect = zeros(1,length(X)*(iterations+1));
for i = 0:iterations
    Yvect(1,length(X)*i+1:length(X)*i+length(X)) = i*dt*ones(1,length(X));
end

Tvect = zeros(1,length(X)*(iterations+1));
for i = 0:iterations
    for j = 1:length(X)
        Tvect(1,length(X)*i+j) = T(i+1,j);
    end
end
try
T_func = fit([Xvect', Yvect'],Tvect','linearinterp');
catch ME
    T_func = false;
end