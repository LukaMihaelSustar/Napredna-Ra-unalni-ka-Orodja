//Uvozimo knjižnice za 
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <omp.h>
#include <ctime>
#include <iostream>
#include <sstream>



using namespace std;

int main() {

    // inicializiramo matriko A
    vector<vector<double>> A;

    // inicializiramo vektor b;
    vector<double> b;

    // vnesemo ime datoteke, ki vsebuje podatke
    std::string filename = "datoteka_A_b.txt";

    // odpremo in preberemo datoteko
    std::ifstream infile;
    infile.open(filename);

    // velikost matrike A imamo podano v prvi vrstici -> preberemo 
    std::string prva_vrstica_string;
    std::getline(infile, prva_vrstica_string);

    // spremenimo delimiter iz = v presledek
    std::replace(prva_vrstica_string.begin(), prva_vrstica_string.end(), '=', ' ');

    // definiramo stringstream za lažje shranjevanje
    std::istringstream iss(prva_vrstica_string);
    std::string string_A; // string A
    std::string string_n; // string n
    int n; // n nam pove velikost matrike A

    iss >> string_A;
    iss >> string_n;
    iss >> n;

    std::cout << "Velikost matrike A: " << n << "x" << n << std::endl;;  //Izpis in preverjanje velikosti matrike A

    //Vseh naslednjih n vrstic ima elemente, ki jih shranimo v matriko A
    for (int iiA = 0; iiA < n; iiA++)
    {
        // preberemo vrstico
        std::string line;
        std::getline(infile, line);
        // zamenjamo ';' s  ' ', ker smo tako definirali nov delimiter v prejšnih vrsticah
        std::replace(line.begin(), line.end(), ';', ' ');

        // pretvorimo nazaj v string
        std::istringstream iss_column(line);

        // definiramo nov vektor, za shranjevanje vrstic
        vector<double> vrstica;

        for (int column = 0; column < n; column++)
        {
            double element_A = 0;
            iss_column >> element_A;
            vrstica.push_back(element_A);
        }

        // ko imamo vektor za vrstico, ga damo v A
        A.push_back(vrstica);
    }

    //tako imamo sestavljeno matriko A

    //naslednja vrstica v datoteki je prazna in jo preskočimo
    std::string prazna_vrstica;
    std::getline(infile, prazna_vrstica);

    // branje vektorja b
    std::string string_b;
    std::getline(infile, string_b);

    // ponovitev predhodnjega dela kode
    std::replace(string_b.begin(), string_b.end(), '>', ' ');
    std::istringstream iss_b(string_b);
    int n_b; //velikost vektorja b

    iss_b >> string_A;
    iss_b >> string_n;
    iss_b >> n_b;

    std::cout << "Velikost vektorja b: " << n_b << std::endl;;  //Velikosti matrike A in vektorja b morata biti enaki

    for (int iib = 0; iib < n_b; iib++)
    {
        // branje in shranjevanje
        std::string line_b_element;
        std::getline(infile, line_b_element);
        std::istringstream iss_b_element(line_b_element);

        double b_element = 0;
        iss_b_element >> b_element;

        b.push_back(b_element);
    }

    // Sedaj imamo matriko A in vektor b. Lahko napisemo Gauss-Seidel metodo. Najprej
    // inicializiramo vektor resitve T, npr. na 100 stopinj.

    //-----------------------------------------------------------------------------
    //Reševanje z metodo Gauss-Seidel
    //-----------------------------------------------------------------------------

    //Nastavimo vektor rešitve na poljubno vrednost
    vector<double> T;
    for (int iiT = 0; iiT < n_b; iiT++)
    {
        T.push_back(50);
    }

    auto start_time = std::chrono::high_resolution_clock::now(); //Merjenje časa

    for (int ii = 0; ii < 2000; ii++)  //ii predstavlja število iteracij -> določeno v navodilu naloge
    {
        int n = b.size();

        for (int j = 0; j < n;j++) {
            double d = b[j];

            for (int i = 0; i < n; i++) {
                if (j != i) {
                    d -= A[j][i] * T[i];
                }
                T[j] = d / A[j][j];
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_duration = end_time - start_time;


    // Zapis največje vrednosti
    double T_max = 0;
    for (int iiT = 0; iiT < n_b; iiT++)
    {
        cout << T[iiT] << endl;

        if (T[iiT] > T_max) {
            T_max = T[iiT];
        }
    }
    std::cout << "Cas potreben za izracun po Gauss-Seidl-ovi metodi: " << time_duration.count() << " sekund" << std::endl;
    cout << "Najvisja temperatura je " << T_max << " stopinj C." << endl;



    return 0;
}


//Paralelizacija Gauss - Seidlove metode ni neposredno možna, saj so rezultati temperatur v vsakem koraku iteracije odvisni
// od prejšnjih vrednosti. To povzroča težave pri branju in pisanju podatkov iz vektorja temperatur v različnih niti.
//Ena izmed rešitev je rahla modifikacija metode, kjer uporabimo dva ločena vektorja – enega za branje starih temperatur in drugega
// za zapisovanje novih rezultatov. Po končani iteraciji zamenjamo starega z novim vektorjem, kar predstavlja Jacobijevo metodo.
