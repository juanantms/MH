#include "utilidades.h"
#include "cmath"
#include "bl.h"
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

// Algoritmo de Búsqueda Multiarranque Básica
pair<vector<int>, double> BMB(int n, int m, double **M) {

    pair<vector<int>, double> mejor;

    vector<int> solucion, mejor_sol, sol_aux;
    vector<double> distancia;
    double dispersion=0, mejor_dispersion=0;

    // Generamos aleatoriamente la solucion inicial
    solucion = GenerarSolucionAleatoria(n, m);

    // La guardamos como la mejor hasta el momento
    mejor_sol = solucion;
    SumaDistancias(distancia, solucion, m, M);
    CalculaDispersion(distancia, mejor_dispersion);

    
    // Realizamos 10 iteraciones
    for(int i=0; i<10; i++) {

        // Le aplicamos la BL a la solucion con un maximo de 10000 evaluaciones
        BL(solucion, n, m, M, dispersion, 10000);

        // Comprobamos si la nueva solucion mejora a la mejor hasta el momento
        if(dispersion < mejor_dispersion) {
            // Si lo es la actualizamos
            mejor_sol = solucion;
            mejor_dispersion = dispersion;
        }

        // Generamos una solucion aleatoria
        solucion = GenerarSolucionAleatoria(n, m);
    }
    
    mejor.first = mejor_sol;
    mejor.second = mejor_dispersion;

    // Devolvemos la mejor solucion y la dispersion
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
    p = BMB(n, m, D);
    chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio); 

    solucion = p.first;
    dispersion = p.second;
    
    cout << "Dispersion\t" << dispersion << endl;  
    cout << "Tiempo\t" << tiempo.count() << endl;   

}