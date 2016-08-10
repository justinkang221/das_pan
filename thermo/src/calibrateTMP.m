function f = calibrateTMP(A, n)

v = zeros(n, 1);
T = zeros(n, 1);

disp(['calibrating TMP: ', int2str(n), ' data points']);

for i = 1 : n
    T(i) = input(['enter temperature (', int2str(i), ') >  ']);
    v(i) = readVoltage(A, 'A0');    
end

[a, ~] = polyfit(v, T, 1);

%{
disp('v:');
disp(v);
disp('T:');
disp(T);
disp('m:');
disp(a(1));
disp('b:');
disp(a(2));
%}

f = @(voltage) a(1)*voltage + a(2);

end
