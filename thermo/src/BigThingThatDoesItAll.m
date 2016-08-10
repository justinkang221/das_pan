function [P_in,K_c,k,c,e] = BigThingThatDoesItAll(Xvect,Yvect,Tvect)

Tamb = 293;

%inital guess
P_in = 6;
K_c = 4;
k = 120;
c = 400;
e = 0.5;

delta_P_in = 0.25;
delta_K_c = 1;
delta_k = 50;
delta_c = 100;
delta_e = 0.3;

decreases = 0;

while(true)
tic()
currBestChi = ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k,c,e),Xvect,Yvect,Tvect);
moved = false;

if (ChiSquareCalc(FiniteDiffBrassRod(P_in+delta_P_in,Tamb,K_c,k,c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    P_in = P_in+delta_P_in;
    moved = true;
elseif (ChiSquareCalc(FiniteDiffBrassRod(P_in-delta_P_in,Tamb,K_c,k,c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    P_in = P_in-delta_P_in;
    moved = true;
end

if (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c+delta_K_c,k,c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    K_c = K_c+delta_K_c;
    moved = true;
elseif (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c-delta_K_c,k,c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    K_c = K_c-delta_K_c;
    moved = true;
end
%{
if (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k+delta_k,c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    k = k+delta_k;
    moved = true;
elseif (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k-delta_k,c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    k = k-delta_k;
    moved = true;
end

if (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k,c+delta_c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    c = c+delta_c;
    moved = true;
elseif (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k,c-delta_c,e),Xvect,Yvect,Tvect)-currBestChi < 0)
    c = c-delta_c;
    moved = true;
end
%}
%{
if (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k,c,e+delta_e),Xvect,Yvect,Tvect)-currBestChi < 0)
    e = e+delta_e;
    moved = true;
elseif (ChiSquareCalc(FiniteDiffBrassRod(P_in,Tamb,K_c,k,c,e-delta_e),Xvect,Yvect,Tvect)-currBestChi < 0)
    e = e-delta_e;
    moved = true;
end
%}
if (moved == false)
    decreases = decreases+1;
    delta_P_in = delta_P_in/2;
    delta_K_c = delta_K_c/2;
    delta_k = delta_k/2;
    delta_c = delta_c/2;
    delta_e = delta_e/2;
end
if (decreases > 5)
    break
end
toc()
P_in
K_c
k
c
e
end
    
end