#include "utilidades.h"
#include "bl.h"
#include "cmath"
#include <chrono>
#include <thread>
#include <ctime>


using namespace std::chrono;

// Funcion que calcula el coste o dispersion de una solucion
double CosteSolucion(vector<int> solucion, int m, double **D) {
    vector<double> distancias;
    SumaDistancias(distancias, solucion, m, D);
    double dispersion=0;
    CalculaDispersion(distancias, dispersion);

    return dispersion;
}

// Funcion que enfria la temperatura segun el esquema de Cauchy modificado
void Enfriar(double &T, double T_0, double T_f, int M) {
    double beta;
    beta = (T_0-T_f)/(M*T_0*T_f);

    T = T/(1+(beta*T));
}

// Funcion que que genera un vecino aleatorio de una solucion inicial
void GenerarVecinoAleatorio(vector<int> &vecino, int n, int m) {
    int posicion, aleatorio = -1;

    // Escogemos aleatoriamente la posicion del punto a sustituir
    // Generamos  un punto no seleccionado
    while(aleatorio == -1) {
        aleatorio = rand()%n;
        if(vecino.end() != find(vecino.begin(), vecino.end(), aleatorio)) {
            aleatorio = -1;
        }
    }
    // Escogemos aleatoriamente la posicion del punto a sustituir
    posicion = rand()%m;
    vecino[posicion] = aleatorio;
}

// Algoritmo de Enfriamiento Simulado
void ES(vector<int> &solucion, int n, int m, double **D, double &dispersion) {

    double T_0, T_actual, T_f=0.001, mu=0.3, phi=0.3, coste;

    // Calculamos el coste o la dispersion de la solucion
    coste = CosteSolucion(solucion, m, D);

    // Calculamos la temperatura inicial
    T_0 = (mu*coste)/(-log(phi));
    T_actual = T_0;

    int max_vecinos, max_exitos, M, evaluaciones=0, vecinos=0, exitos=0;
    max_vecinos = 10*n;
    max_exitos = 0.1*max_vecinos;
    M = 10000/max_vecinos;

    bool mejora = true;

    // Mientra no se cumpla el criterio de parada:
    //     + Temperatura actual sea <= que la temperatura final
    //     + Se alcancen 100000 evaluaciones
    //     + No se mejore la solucion tras una evaluacion
    while(T_actual > T_f && evaluaciones < 10000 && mejora) {
        // No enfriaremos la temperatura mientra no hayamos explorado el maximo de vecinos o superemos el maximo de exitos de mejora
        while(vecinos < max_vecinos && exitos < max_exitos) {
            vector<int> solucion_sig;
            double coste_sig, diferencia;

            // Generamos la siguiente solucion vecina
            solucion_sig = solucion;
            GenerarVecinoAleatorio(solucion_sig, n, m);
            vecinos++;

            // Calculamos el coste
            coste_sig = CosteSolucion(solucion_sig, m, D);

            // Calculamos la diferencia de costes con la mejor solucion
            diferencia = coste_sig - coste;

            // Comprobamos si es mejor solucion que la mejor hasta el momento y si no lo es aleatoriamente se aceptará o no según el criterio de aceptación 
            if(diferencia < 0 || rand()%2 < exp(-diferencia/T_actual)) {
                solucion = solucion_sig;
                coste = coste_sig;
                exitos++;
            }
        }

        // Enfriamos la temperatura actual
        Enfriar(T_actual, T_0, T_f, M);
        
        // COmprobamos si no ha habido mejoras  
        if(exitos == 0) {
            mejora = false;
        }

        // Terminamos e incrementamos las iteraciones y ajustamos los valores a 0
        evaluaciones++;
        vecinos = 0;
        exitos = 0;
    }

    dispersion=coste;
}

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

// Algoritmo Hibrido de ILS y ES (ILS-ES)
pair<vector<int>, double> ILS_ES(int n, int m, double **M) {
    pair<vector<int>, double> mejor;

    vector<int> solucion, mejor_sol, sol_mutada;

    // Generamos una solucion aleatoria inicial
    solucion = GenerarSolucionAleatoria(n, m);

    double dispersion, mejor_dispersion, dispersion_mutada;
    int iteraciones=0;

    // Aplicamos ES a la solucion inicial 
    ES(solucion, n, m, M, dispersion);

    // A la solucion inicial la consideramos la mejor hasta el momento
    mejor_dispersion = dispersion;
    mejor_sol = solucion;

    // Durante 9 iteraciones haremos lo siguiente
    while(iteraciones < 9) {
        
        // Hacemos una copia de la mejor solucion y la mutamos
        sol_mutada = mejor_sol;
        Mutar(sol_mutada, n, m);
        
        // Aplicamos ES a la solucion mutada
        ES(sol_mutada, n, m, M, dispersion_mutada);

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
    p = ILS_ES(n, m, D);
    chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio); 

    solucion = p.first;
    dispersion = p.second;
    
    cout << "Dispersion\t" << dispersion << endl;  
    cout << "Tiempo\t" << tiempo.count() << endl;   

}