#include "utilidades.h"
#include "bl.h"
#include "cmath"
#include <chrono>
#include <thread>
#include <ctime>


using namespace std::chrono;


// Funcion que calcula una solucion aleatoria de 'm' puntos seleccionados entre 'n' puntos totales
vector<int> GenerarSolucionAleatoria(int n, int m) {
    vector<int> solucion;
    while(solucion.size() < m) {
        int aleatorio = rand()%n;
        if(solucion.end() == find(solucion.begin(), solucion.end(), aleatorio)) {
            solucion.push_back(aleatorio);
        }
    }
    return solucion;
}

// Funcion que muta una solucion intercambiando 0.3*m elementos seleccionados por 0.3*m elementos NO seleccionados
void Mutar(vector<int> &solucion, int n, int m) {
    int num_cambios = 0.3*m;
    vector<int> cambios, posiciones;

    // Generamos aleatoriamente 0.3*m puntos no seleccionados
    while(cambios.size() < num_cambios) {
        int aleatorio = rand()%n;
        if(solucion.end() == find(solucion.begin(), solucion.end(), aleatorio) 
                && cambios.end() == find(cambios.begin(), cambios.end(), aleatorio)) {

            cambios.push_back(aleatorio);
        }
    }
    // Generamos aleatoriamente 0.3*m posiciones para realizar el intercambio
    while(posiciones.size() < num_cambios) {
        int aleatorio = rand()%m;
        if(posiciones.end() == find(posiciones.begin(), posiciones.end(), aleatorio)) {
            posiciones.push_back(aleatorio);
        }
    }

    // Realizamos el intercambio
    for(int i=0; i<num_cambios; i++) {
        solucion[posiciones[i]] = cambios[i];
    }
}


// Algoritmo de Búsqueda Local Reiterada
pair<vector<int>, double> ILS(int n, int m, double **M) {
    pair<vector<int>, double> mejor;

    vector<int> solucion, mejor_sol, sol_mutada;

    // Generamos una solucion aleatoria inicial
    solucion = GenerarSolucionAleatoria(n, m);

    double dispersion, mejor_dispersion, dispersion_mutada;
    int iteraciones=0;

    // Aplicamos BL a la solucion inicial 
    BL(solucion, n, m, M, dispersion, 10000);

    // A la solucion inicial la consideramos la mejor hasta el momento
    mejor_dispersion = dispersion;
    mejor_sol = solucion;

    // Durante 9 iteraciones haremos lo siguiente
    while(iteraciones < 9) {

        // Hacemos una copia de la mejor solucion y la mutamos
        sol_mutada = mejor_sol;
        Mutar(sol_mutada, n, m);

        // Aplicamos BL a la solucion mutada
        BL(sol_mutada, n, m, M, dispersion_mutada, 10000);

        // Comparamos entre la mejor solucion hasta el momento y la mejor solucion mutada para quedarnos con la mejor
        if(dispersion_mutada < mejor_dispersion) {
            mejor_sol = sol_mutada;
            mejor_dispersion = dispersion_mutada;
        } 

        // Actualizamos la solucion para la siguiente solucion
        iteraciones++;
    }

    mejor.first = mejor_sol;
    mejor.second = mejor_dispersion;
    
    return mejor;
}


int main() {
    vector<Elemento> V;
    vector<int> puntos;
    int n, m;

    cin >>  V;
    n = V[0].n;
    m = V[0].m;

    
    // Reservamos memoria para la Matriz.
    double **D = new double*[n];
    for(int i=0; i <n; i++) {
        D[i] = new double[n];
    }
    for(int i=0; i<V.size(); i++) {
        D[V[i].i][V[i].j] = V[i].distancia;
        D[V[i].j][V[i].i] = V[i].distancia;
    }


    pair<vector<int>, double> p;
    double dispersion;
    vector<int> solucion;

    chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now(); 
    p = ILS(n, m, D);
    chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio); 

    solucion = p.first;
    dispersion = p.second;
    
    cout << "Dispersion\t" << dispersion << endl;  
    cout << "Tiempo\t" << tiempo.count() << endl;

}