#include <iostream>
#include <vector>
#include <set>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <utilidades.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <ctime>
#include "random.cpp"

using Random = effolkronium::random_static;
using namespace std::chrono;


vector<bool> CalcularHabitat(int n, int m) {
    set<int> seleccionados;
    // Inicializamos la solucion/cromosoma a false, es decir, todo a 0.
    vector<bool> habitat(n, false);
    int num;

    // Generamos m indices aleatorios que serán los puntos que valdrán 1 en el cromosoma y los puntos seleccionados para la solución.
    while(seleccionados.size()<m) {
        num = rand()%n;
        seleccionados.insert(num);
    }

    // Recorremos el cromosoma y cambiamos a true los seleccionados.
    for(auto it = seleccionados.begin(); it!=seleccionados.end(); it++) {
        habitat[*it] = true;
    }
    
    return habitat;
}


void CalcularEcosistema(vector<vector<bool>> &ecosistema, int n, int m) {
    vector<bool> aux;

    // Generamos 50 cromosomas para formar la poblacion de tamaño 50.
    while (ecosistema.size()<50) {
        aux = CalcularHabitat(n, m);
        ecosistema.push_back(aux);
    }
}

void CalcularDispersionesEcosistema(vector<vector<bool>> ecosistema, vector<double> &dispersiones, int m, double **M) {

    for(int i=0; i<ecosistema.size(); i++) {
        vector<int> sel;
        // Guardamos los indices que valen 1 en sel
        for(int j=0; j<ecosistema[0].size(); j++) {
            if(ecosistema[i][j]) {
                sel.push_back(j);
            }
        }

        vector<double> dist;
        SumaDistancias(dist, sel, m, M);
        // Calculamos las dispersiones que tienen.
        double disp;
        CalculaDispersion(dist, disp);

        dispersiones.push_back(disp);
    }
}

double CalcularDispersionHabitat(vector<bool> habitat, int m, double **M) {
    vector<int> sel;
    // Guardamos los indices que valen 1 en sel
    for(int i=0; i<habitat.size(); i++) {
        if(habitat[i]) {
            sel.push_back(i);
        }
    }
    vector<double> dist;
    SumaDistancias(dist, sel, m, M);
    // Calculamos las dispersion que tienen.
    double disp;
    CalculaDispersion(dist, disp);

    return disp;
}

void OrdenarEcosistema(vector<double> &dispersiones, vector<vector<bool>> &ecosistema) {

    vector<pair<double, int>> indices;
    for(int i=0; i<dispersiones.size(); i++) {
        pair<double, int> p;
        p.first = dispersiones[i];
        p.second = i;
        indices.push_back(p);
    }

    //sort(indices.begin(), indices.end(), std::greater<pair<double, int>>());
    sort(indices.begin(), indices.end());

    vector<vector<bool>> nuevo_ecosistema;
    vector<double> nuevas_dispersiones;
    for(int i=0; i<dispersiones.size(); i++) {
        nuevo_ecosistema.push_back(ecosistema[indices[i].second]);
        nuevas_dispersiones.push_back(indices[i].first);
    }

    dispersiones = nuevas_dispersiones;
    ecosistema = nuevo_ecosistema;
}


int IndiceRouletteWheel(vector<double> mu) {
    double sum_mu = 0;

    for(int i=0; i<mu.size(); i++){
        sum_mu += mu[i];
    }

    double aux = 0;
    double aleatorio = Random::get<double>(0, (int) sum_mu);

    for(int i=0; i<mu.size(); i++){
        aux += mu[i];
        if(aux>=aleatorio){
            return i;
        }
    }

    return -1;
}


bool Migrar(vector<bool> h1, vector<bool> &h2) {

    vector<int> sel_h1, sel_h2;
    // Guardamos los indices que valen 1 en sel_h1 y sel_h2
    for(int i=0; i<h1.size(); i++) {
        if(h1[i]) {
            sel_h1.push_back(i);
        }
        if(h2[i]) {
            sel_h2.push_back(i);
        }
    }

    // Comprobamos que no sean iguales
    int iguales = 0;
    for(int i=0; i<sel_h1.size(); i++) {
        if(sel_h1[i] == sel_h2[i]) {
            iguales++;
        }
    }

    // Si son iguales devolvemos false
    if(iguales == sel_h1.size()) {
        return false;
    }

    // Calculamos dos elementos seleccionados que sean diferentes en ambos habitats
    int pos_h1=rand()%sel_h1.size(), pos_h2=rand()%sel_h2.size();

    // Comprobamos que el elemento de h1 no está en h2
    bool repite=true;
    while(repite) {
        repite = false;
        for(int i=0; i<sel_h2.size() && !repite; i++) {
            if(sel_h1[pos_h1] == sel_h2[i]) {
                repite = true;
            }
        }
        if(repite) {
            pos_h1=rand()%sel_h1.size();
        }
    }

    // Una vez tengamos un seleccionado de h2 para cambiarlo a no seleccionado
    // y un elelemnto de h1 que en h2 se volverá seleccionado haremos el cambio
    h2[sel_h2[pos_h2]] = false;
    h2[sel_h1[pos_h1]] = true;

    // Devolvemos true para informar de que se ha migrado correctamente
    return true;
}

void Mutar(vector<bool> &habitat) {
    vector<int> sel, no_sel;
    // Guardamos los indices que valen 1 en sel y los que valen 0 en no_sel
    for(int i=0; i<habitat.size(); i++) {
        if(habitat[i]) {
            sel.push_back(i);
        }else {
            no_sel.push_back(i);
        }
    }

    // Generamos dos indices aleatorios para sel y no_sel
    int pos_sel=rand()%sel.size(), pos_nosel=rand()%no_sel.size();

    // Hacemos la mutación intercambiando los valores de dos elementos del habitat
    habitat[sel[pos_sel]] = false;
    habitat[no_sel[pos_nosel]] = true;
}


vector<bool> BBO(int n, int m, double **M) {

    vector<vector<bool>> ecosistema;
    vector<double> dispersiones_ecosistema;
    
    // Calculamos el ecosistema
    CalcularEcosistema(ecosistema, n, m);

    // Índice de Inmigración y de Emigración:
    vector<double> lambda(ecosistema.size()), mu(ecosistema.size());
    for(int i=0; i < ecosistema.size(); i++) {
        mu[i] = double (ecosistema.size()-i)/(ecosistema.size()+1);
        lambda[i] = 1 - double (ecosistema.size()-i)/(ecosistema.size()+1);
    }

    // Elites del ecosistema
    int elites = 0.05*ecosistema.size(); 
    
    // Calculamos las dispersiones de cada habitat del ecosistema
    CalcularDispersionesEcosistema(ecosistema, dispersiones_ecosistema, m, M);
    // Ordenamos el ecosistema
    OrdenarEcosistema(dispersiones_ecosistema, ecosistema);
    
    int generaciones=0;
    // Realizaremos el BBO durante 1000 generaciones de ecosistemas
    while(generaciones<1000) {
        // Guardamos los soluciones elites para el elitismo.
        vector<vector<bool>> elites_ecosistema;
        for(int i=0; i<elites; i++) {
            elites_ecosistema.push_back(ecosistema[i]);
        }

        for(int i=0; i<ecosistema.size(); i++) {
            
            // Si se cumple la condición se producirá una emigración al habitat 'i'
            if(Random::get<double>(0,1) <= lambda[i]) {  
                vector<double> mu_aux(mu);
                mu_aux[i] = 0;

                int k = IndiceRouletteWheel(mu_aux);

                bool migra = Migrar(ecosistema[k], ecosistema[i]);
                
                while(!migra) {
                    //cout << "iguales" << endl;
                    k = IndiceRouletteWheel(mu_aux);
                    
                    migra = Migrar(ecosistema[k], ecosistema[i]);
                }

                // Actualizamos la dispersion del nuevo habitat
                dispersiones_ecosistema[i] = CalcularDispersionHabitat(ecosistema[i], m, M); 
            }

            // Si se cumple la condición se producirá una mutación al habitat 'i'
            if(Random::get<double>(0,1) < 0.1) {
                // Mutamos
                Mutar(ecosistema[i]);

                // Actualizamos la dispersion del nuevo habitat
                dispersiones_ecosistema[i] = CalcularDispersionHabitat(ecosistema[i], m, M); 
            }
        }
        // Una vez terminada la generación ordenamos el ecosistema de mayor a menor dispersión
        OrdenarEcosistema(dispersiones_ecosistema, ecosistema);

        // Sustituimos las 5 peores soluciones por las 5 elites calculadas al comienzo de la generación y aplicamos elitismo
        for(int i=0; i<elites; i++) {
            ecosistema[ecosistema.size()-1-i] = elites_ecosistema[i];
        }

        // Volvemos a ordenar el ecosistema
        OrdenarEcosistema(dispersiones_ecosistema, ecosistema);

        // Avanzamos de generación
        generaciones++;
    }

    // Devolvemos la mejor solución;
    return ecosistema[0];
}


int main()  {

    vector<Elemento> V;
    int n, m;

    cin >>  V;
    n = V[0].n;
    m = V[0].m;

    // Reservamos memoria para la Matriz.
    double **M = new double*[n];
    for(int i=0; i <n; i++) {
        M[i] = new double[n];
    }
    for(int i=0; i<V.size(); i++) {
        M[V[i].i][V[i].j] = V[i].distancia;
        M[V[i].j][V[i].i] = V[i].distancia;
    }

    vector<bool> mejor;

    chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now(); 
    mejor = BBO(n, m, M);
    chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio); 

    vector<int> puntos;
    for(int i=0; i<mejor.size(); i++) {
        if(mejor[i]) {
            puntos.push_back(i);
        }
    }
    vector<double> distancias;
    SumaDistancias(distancias, puntos, m, M);
    double dispersion;
    CalculaDispersion(distancias, dispersion);

    cout << "BBO\t" << dispersion << endl;
    cout << "Tiempo_BBO\t" << tiempo.count() << endl;

}