% Uvoz podatkov iz dokumentov 
podatki = readmatrix("vozlisca_temperature_dn2.txt", "NumHeaderLines",4);
x = podatki(:,1);
y = podatki(:,2);
T = podatki(:,3);

celice = readmatrix("celice_dn2.txt", "NumHeaderLines",2);

% Definicija iskanih vrednosti
x_iskan = 0.403;
y_iskan = 0.503;

% 1. metoda: uporaba scatteredInterpolant
tic;
F1 = scatteredInterpolant(x, y, T, "linear");
T1 = F1(x_iskan, y_iskan);
cas1 = toc;

% 2. metoda: uporaba griddedInterpolant
x_edini = unique(x);
y_edini = unique(y);
[X, Y] = ndgrid(x_edini, y_edini); 
matrika = reshape(T, length(x_edini), length(y_edini));
tic;
F2 = griddedInterpolant(X, Y, matrika, "linear");
T2 = F2(x_iskan, y_iskan);
cas2 = toc;

% 3. metoda: lastna funkcija za bilinearno interpolacijo
tic; 
cell_found = false; % Spremenljivka za preverjanje, ali smo našli celico

for i = 1:size(celice, 1)
    % Preberemo indekse celice
    pt1 = celice(i, 1);
    pt2 = celice(i, 2);
    pt3 = celice(i, 3);
    pt4 = celice(i, 4);

    % Koordinate in temperature na vogalih celice
    x1 = x(pt1);
    y1 = y(pt1);
    T11 = T(pt1);

    x2 = x(pt2);
    y2 = y(pt2);
    T21 = T(pt2);

    x3 = x(pt3);
    y3 = y(pt3);
    T22 = T(pt3);

    x4 = x(pt4);
    y4 = y(pt4);
    T12 = T(pt4);

    % Preverimo, ali je točka znotraj celice
    if x_iskan >= x1 && x_iskan <= x2 && y_iskan >= y1 && y_iskan <= y3
        % Bilinearna interpolacija po x-osi
        K1 = (x2 - x_iskan) / (x2 - x1) * T11 + (x_iskan - x1) / (x2 - x1) * T21;
        K2 = (x2 - x_iskan) / (x2 - x1) * T12 + (x_iskan - x1) / (x2 - x1) * T22;

        % Bilinearna interpolacija po y-osi
        T3 = (y3 - y_iskan) / (y3 - y1) * K1 + (y_iskan - y1) / (y3 - y1) * K2;
        break;
    end
end

cas3 = toc;

%največja temperatura:
T_max = max(T);
index = find(T==T_max);
x_max = x(index);
y_max = y(index);

% Izpis rezultatov za primerjavo med posameznimi funkcijami
fprintf('Bilinearna interpolacija: %.6f (čas: %.6f s)\n', T3, cas3);
fprintf('Interpolacija s scatteredInterpolant: %.6f (čas: %.6f s)\n', T1, cas1);
fprintf('Interpolacija s griddedInterpolant: %.6f (čas: %.6f s)\n', T2, cas2);
