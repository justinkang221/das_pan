k = 6;
n = 5;
position = [0 1 2 3 4 5];
step = 0.5; % s

f = { @(x) sin(x);
    @(x) cos(x);
    @(x) 1/x;
    @(x) log(x);
    @(x) 2^(-x);
    @(x) sin(x+pi/4);
};

data = zeros(n,k+2);

figure
subplot(2,1,1);
plots = cell(k+1,1);
for i = 1:k
    plots{i} = plot(0,0);
    hold on;
end
title('Temperature vs Time');
legend('A0','A1','A2','A3','A4','A5');
xlabel('time (s)');
ylabel('Temperature (C)');

subplot(2,1,2);
plots{k+1} = plot(0,0);
title('Temperature vs Position');
xlabel('position (cm)');
ylabel('Temperature (C)');

tic;
for t = 1:n
    data(t,1) = toc;
    data(t,2) = 20;
    
    subplot(2,1,1);
    for i = 1:k
        data(t,i+2) = f{i}(t);
        set(plots{i}, 'xdata', data(:,1), 'ydata', data(:, i+2));
    end
    title(['Time ' num2str(data(t,1))]);
    
    subplot(2,1,2);
    set(plots{k+1}, 'xdata', position, 'ydata', data(t,3:end));
    
    pause(step);
end

time = zeros(6*n,1);
x = zeros(6*n,1);
temperature = zeros(6*n,1);

for i=1:n
    for j=1:k
        time(k*(i-1)+j) = data(i,1);
        x(k*(i-1)+j) = position(j);
        temperature(k*(i-1)+j) = data(i, j+2);
    end
end

figure
plot(data(:,1), data(:,2));
title('Reference Temperature vs Time');
xlabel('time (s)');
ylabel('Temperature (C)');

figure
mesh(position, data(:,1), data(:, 3:k+2));
title('Data surface');
xlabel('position (cm)');
ylabel('time (s)');
zlabel('Temperature (C)');
