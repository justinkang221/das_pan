function f = calibrateCouple(A, B, n, TMPV2T)

v = zeros(n, 6);
dT = zeros(n, 1);

disp(['calibrating thermocouples: ', int2str(n), ' data points']);

for i = 1 : n
    % for each data point, prompt user for calib bath temperature
    dT(i) = input(['enter calib temperature (', int2str(i), ') >> ']) - TMPV2T(6.4);
    for k = 1 : 6
        % measure each couple voltage separately and store them in v
        v(i, k) = readVoltage(B, ['A', int2str(k-1)]);
        if v(i, k) == 0
            disp(['error: couple ', int2str(k), ' produced negative V']);
        end
    end      
end

f = cell(6, 1);
for k = 1 : 6
    % k is the thermocouple number
    
    % plot calibration
    figure
    plot(v(:, k),dT, 'ro');
    title(['couple ', int2str(k), ' calibration']);
    
    % exclude any zeros from linear fit
    z = find(v(:,k)~=0, 1, 'first');
    [a, ~] = polyfit(v(z:end, k), dT, 1);
    f{k} = @(voltage) a(1)*voltage + a(2);
end
hold off;
end
