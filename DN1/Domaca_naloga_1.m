%1. naloga
%pridobivanje podatkov iz datotek

delimiterIn="";
headerlinesIn = 2;
data = importdata("naloga1_1.txt","",2);
t = data.data(); %podatki shranjeni v vektor

%2. naloga
%alternativni način shranjevanja podatkov - brez importdata

fid = fopen("naloga1_2.txt", "r");

prva_vrstica = fgetl(fid);
razdeljena_vrstica = strsplit(prva_vrstica, ": ");
stevilcni_del = razdeljena_vrstica(2);
stevilo = str2double(stevilcni_del);

P = zeros(0, stevilo);
for i = 1:1:stevilo
    vrstica = fgetl(fid);
    P(i) = str2double(vrstica);
end

%Graf moci od casa - P(t)

plot(t, P)
title ("Graf P(t)")
xlabel("t[s]")
ylabel("P[W]")

% 3. naloga - izracun integala

trapezna_rezultat = trapz(t, P);


rezultat = 0;
for i = 1:1:(stevilo-1)
    dt = t(i+1) - t(i); 
    a = dt/2 * (P(i) + P(i+1));
    rezultat = rezultat + a;
end

razlika = rezultat - trapezna_rezultat
