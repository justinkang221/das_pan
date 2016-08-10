clear A B;

% constants and parameters
k = 6; % number of thermocouples
n = 300; % number of data points
position = [1.6 6.6 11.7 17.1 22.5 28.1]; % positions of the thermocouples
step = 1;

assert(k ~= size(position,1), 'Number of thermocouples and positions provided are not the same.');

% The TMP circuit output is connected to A at terminal A0
% A = arduino('/dev/cu.usbmodem1421');

%{
calibrate TMP

if input('override TMP, assume constant ref temperature? (Yes: 1, No: 0) >  ')
    TMPV2T = @(voltage) 0;
elseif ~input('use previous TMP calibration? (Yes: 1, No: 0) >  ')
    TMPV2T = calibrateTMP(A, k);
end
assert(~isempty(TMPV2T), 'TMP calibration failed');
%}

% search for arduino B
try
    B = arduino('/dev/cu.usbmodem1411');
catch
    error('no arduino found in COM9, could not calbrate thermocouples');    
end

%{
 calibrate thermocouples
if ~input('use previous thermocouple calibration? (Yes: 1, No: 0) >  ')
    coupleV2dT = calibrateCouple(A, B, 3, TMPV2T);
end
assert(~isempty(coupleV2dT), 'thermocouple calibration failed');
%}

% first column is time, second column is reference temperature, rest are thermocouples
% rows correspond to each time step
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

refT = 21.3;

tic;
for t = 1:n
    data(t,1) = toc;
    
    % refT = TMPV2T(readVoltage(A, 'A0'));
    data(t,2) = refT;
    
    subplot(2,1,1);
    for i = 1:k
        data(t,i+2) = (readVoltage(B, ['A',int2str(i-1)]));
        set(plots{i}, 'xdata', data(:,1), 'ydata', data(:, i+2));
    end
    title(['Time ' num2str(data(t,1)) ' s']);
    
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
mesh(position, data(:,1), data(:, 3:k+2));
title('Data surface');
xlabel('position (cm)');
ylabel('time (s)');
zlabel('Temperature (C)');
