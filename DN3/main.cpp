#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>


//Izraèun funkcije ArcTan in razvoj v Taylorjevo vrsto
double ArcTan(double* x, int* N_steps) {
    double rezultat = 0.0;
    for (int n = 0; n < *N_steps; ++n) {
        rezultat += pow(-1, n) * pow(*x, 2 * n + 1) / (2 * n + 1);
    }
    return rezultat;
}

double integral(double x) {
    return exp(3 * x) * ArcTan(new double(x / 2), new int(10));
}

double trapezno_pravilo(double x1, double x2, int n) {
    double korak = (x2 - x1) / n;
    double vsota = 0.5 * (integral(x1) + integral(x2));

    for (int i = 1; i < n; ++i) {
        double r = x1 + i * korak;
        vsota += integral(r);
    }

    return korak * vsota;
}

int main() {
    double a = 0.0, b = M_PI / 4;
    int razdelki = 750;

    double rezultat = trapezno_pravilo(a, b, razdelki);

    std::cout << "Vrednost integrala je: " << rezultat;

    return 0;
}