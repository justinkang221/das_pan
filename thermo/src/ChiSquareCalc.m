function ChiSquared = ChiSquareCalc(T_func, Xvect, Yvect, Tvect)
if (~isa(T_func, 'sfit'))
    ChiSquared = inf;
    return
end
ChiSquared = 0;
for i = 1:length(Tvect)
    ChiSquared = ChiSquared + (T_func(Xvect(i),Yvect(i))-Tvect(i))^2/Tvect(i);
end
end