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

using namespace std::chrono;


vector<bool> CalcularSolucion(int n, int m) {
    set<int> seleccionados;
    // Inicializamos la solucion/cromosoma a false, es decir, todo a 0.
    vector<bool> solucion(n, false);
    int num;

    // Generamos m indices aleatorios que serán los puntos que valdrán 1 en el cromosoma y los puntos seleccionados para la solución.
    while(seleccionados.size()<m) {
        num = rand()%n;
        seleccionados.insert(num);
    }

    // Recorremos el cromosoma y cambiamos a true los seleccionados.
    for(auto it = seleccionados.begin(); it!=seleccionados.end(); it++) {
        solucion[*it] = true;
    }
    
    return solucion;
}


void CalcularPoblacion(vector<vector<bool>> &poblacion, int n, int m) {
    vector<bool> aux;

    // Generamos 50 cromosomas para formar la poblacion de tamaño 50.
    while (poblacion.size()<50) {
        aux = CalcularSolucion(n, m);
        poblacion.push_back(aux);
    }
}


vector<bool> MejorSolucion(vector<bool> a, vector<bool> b, int m, double **M, int &evaluaciones) {
    evaluaciones+=2;
    vector<int> sel_a, sel_b;
    // Guardamos los indices que valen 1 en sel_a y sel_b
    for(int i=0; i<a.size(); i++) {
        if(a[i]) {
            sel_a.push_back(i);
        }
        if(b[i]) {
            sel_b.push_back(i);
        }
    }

    // Calculamos la suma de las distancias a esos puntos.
    vector<double> dist_a, dist_b;
    SumaDistancias(dist_a, sel_a, m, M);
    SumaDistancias(dist_b, sel_b, m, M);
    // Calculamos las dispersiones que tienen.
    double disp_a, disp_b;
    CalculaDispersion(dist_a, disp_a);
    CalculaDispersion(dist_b, disp_b);

    // Comparamos cual tiene la menor dispersion y lo devolvemos.
    if(disp_a > disp_b) {
        return b;
    }else {
        return a;
    }    
}

vector<bool> PeorSolucion(vector<bool> a, vector<bool> b, int m, double **M, int &evaluaciones) {
    evaluaciones+=2;
    vector<int> sel_a, sel_b;
    // Guardamos los indices que valen 1 en sel_a y sel_b
    for(int i=0; i<a.size(); i++) {
        if(a[i]) {
            sel_a.push_back(i);
        }
        if(b[i]) {
            sel_b.push_back(i);
        }
    }

    // Calculamos la suma de las distancias a esos puntos.
    vector<double> dist_a, dist_b;
    SumaDistancias(dist_a, sel_a, m, M);
    SumaDistancias(dist_b, sel_b, m, M);
    // Calculamos las dispersiones que tienen.
    double disp_a, disp_b;
    CalculaDispersion(dist_a, disp_a);
    CalculaDispersion(dist_b, disp_b);

    // Comparamos cual tiene la mayor dispersion y lo devolvemos.
    if(disp_a < disp_b) {
        return b;
    }else {
        return a;
    }    
}


vector<bool> CalculaMejorSolucionPoblacion(vector<vector<bool>> poblacion, int m, double **M, int &evaluaciones) {
    int mejor_sol;
    double mejor_dispersion = 999999;
    
    for(int i=0; i<poblacion.size(); i++) {
        vector<int> puntos;
        // Guardamos los indices que valen 1 
        for(int j=0; j<poblacion[i].size(); j++) {
            if(poblacion[i][j]) {
                puntos.push_back(j);
            }
        }
        vector<double> distancias, dispersiones;
        // Calculamos la suma de las distancias a esos puntos.
        SumaDistancias(distancias, puntos, m, M);
        double d=0;
        // Calculamos la dispersion que tienen.
        CalculaDispersion(distancias, d);
        evaluaciones++;
        
        // Si mejora la mejor dispersion que tenemos actualizamos con la nueva mejor dispersion.
        if(d<mejor_dispersion) {
            mejor_dispersion = d;
            mejor_sol = i;
        }
    }   

    return poblacion[mejor_sol];
}

vector<bool> CalculaPeorSolucionPoblacion(vector<vector<bool>> poblacion, int m, double **M, int &evaluaciones) {
    int peor_sol;
    double peor_dispersion = -1;
    
    for(int i=0; i<poblacion.size(); i++) {
        vector<int> puntos;
        // Guardamos los indices que valen 1 
        for(int j=0; j<poblacion[i].size(); j++) {
            if(poblacion[i][j]) {
                puntos.push_back(j);
            }
        }
        vector<double> distancias, dispersiones;
        // Calculamos la suma de las distancias a esos puntos.
        SumaDistancias(distancias, puntos, m, M);
        double d=0;
        // Calculamos la dispersion que tienen.
        CalculaDispersion(distancias, d);
        evaluaciones++;
        
        // Si empeora la peor dispersion que tenemos actualizamos con la nueva peor dispersion.
        if(d>peor_dispersion) {
            peor_dispersion = d;
            peor_sol = i;
        }
    }   

    return poblacion[peor_sol];
}

void OperadorSeleccion(vector<vector<bool>> &poblacion, int m, double **M, int &evaluaciones) {
    int p1, p2;
    vector<bool> mejor_padre;
    vector<vector<bool>> nueva_poblacion;


    while(nueva_poblacion.size()<50) {  

        // Generamos dos indices aleatorios distintos.      
        p1 = rand()%50;
        p2 = rand()%50;

        while(p1==p2) {
           p2 = rand()%50;
        }

        // Nos quedamos con el mejor padre realizando un torneo binario.
        mejor_padre = MejorSolucion(poblacion[p1], poblacion[p2], m, M, evaluaciones);

        // Lo añadimos a la población.
        nueva_poblacion.push_back(mejor_padre);
    }

    poblacion = nueva_poblacion;
}

void Cruce(vector<bool> a, vector<bool> b, vector<bool> &hijo1, vector<bool> &hijo2, int n) {
   
    vector<bool> restantes1, restantes2;

    // Guardamos los indices de los valores que no coincidan en los padres
    for(int i=0; i<n; i++) {
        if(a[i] != b[i]) {
            restantes1.push_back(a[i]);
            restantes2.push_back(b[i]);
        }
    }

    // Desordenamos los indices
    random_shuffle(restantes1.begin(), restantes1.end());
    random_shuffle(restantes2.begin(), restantes2.end());   

    int j=0;
    for(int i=0; i<n; i++) {
        // SI coinciden en los padres coinciden en los hijos
        if(a[i] == b[i]) {
            hijo1.push_back(a[i]);
            hijo2.push_back(b[i]);
        // SI no cogemos un valor de los restantes en el padre    
        }else {
            hijo1.push_back(restantes1[j]);
            hijo2.push_back(restantes2[j]);
            j++;
        }
    }
}


void OperadorCruce(vector<vector<bool>> &poblacion, int n, int m, double **M) {

    vector<vector<bool>> nueva_poblacion;
    // Calculamos el numero de padres a cruzar con probabilidad de cruce de 0.7
    int n_cruce = poblacion.size()*0.7;
    int i;

    for(i=0; i<n_cruce+1; i+=2) {
        vector<bool> hijo1, hijo2; 
        // Generamos los dos nuevos hijos por cruce   
        Cruce(poblacion[i], poblacion[i+1], hijo1, hijo2, n);
        // Los añadimos a la nueva poblacion
        nueva_poblacion.push_back(hijo1);
        nueva_poblacion.push_back(hijo2);
    }
    // Despues de añadir los hijos generados por cruce, añadimos el resto sin cruzar
    while(i<poblacion.size()) {
        nueva_poblacion.push_back(poblacion[i]);
        i++;
    }

    poblacion = nueva_poblacion;
}


void Mutar(vector<vector<bool>> &poblacion) {
    set<int> ind_mut;
    int n_mutaciones;
    // Calculamos el numero de mutaciones sobre el total
    n_mutaciones = poblacion.size()*0.1;
    while(ind_mut.size() < n_mutaciones) {
        // Generamos indices aleatorios para mutar
        ind_mut.insert(rand()%poblacion.size());
    }

    // Recorremos los indices para realizar las mutaciones
    for(auto it=ind_mut.begin(); it!=ind_mut.end(); it++) {
        bool aux;

        // Generamos 2 indices aleatorios para intercambiar los valores y asi mutar
        int i=rand()%poblacion[*it].size(); 
        int j=rand()%poblacion[*it].size(); 
        // los indices deben de ser distintos y no pueden tener el mismo valor
        while(i==j || poblacion[*it][i]==poblacion[*it][j]) {
            j=rand()%poblacion[*it].size(); 
        }
        aux = poblacion[*it][i];
        poblacion[*it][i] = poblacion[*it][j];
        poblacion[*it][j] = aux;
    }
}


void AGGPosicion(vector<vector<bool>> &poblacion, int n, int m, double **M) {

    vector<vector<bool>> poblacion_siguiente;
    vector<bool> mejor_sol_anterior, peor_sol_actual, aux;
    // Calculamos la poblacion
    CalcularPoblacion(poblacion, n, m);
    
    int evaluaciones=0;
    while(evaluaciones<100000) {

        // Calculamos la mejor solucion de la poblacion actual
        mejor_sol_anterior = CalculaMejorSolucionPoblacion(poblacion, m, M, evaluaciones);
        // Nos disponemos a calcular la poblacion siguiente
        poblacion_siguiente = poblacion;
        // SELECCIONAMOS
        OperadorSeleccion(poblacion_siguiente, m, M, evaluaciones);
        // CRUZAMOS
        OperadorCruce(poblacion_siguiente, n, m, M);
        // MUTAMOS
        Mutar(poblacion_siguiente);
        
        // Calculamos la peor solucion de la poblacion siguiente
        peor_sol_actual = CalculaPeorSolucionPoblacion(poblacion_siguiente, m, M, evaluaciones);
        aux = MejorSolucion(mejor_sol_anterior, peor_sol_actual, m, M, evaluaciones);

        // Si la mejor solucion de la poblacion anterior es mejor que la peor de la siguiente poblacion
        if(aux!= peor_sol_actual) {
            // Sustituimos por la mejor
            vector<vector<bool>>::iterator it;
            it = find(poblacion_siguiente.begin(), poblacion_siguiente.end(), peor_sol_actual);
            *it = mejor_sol_anterior;
        }
        
        poblacion = poblacion_siguiente;
    }

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

    vector<vector<bool>> poblacion;
    vector<bool> mejor;
    int evaluaciones = 0;

    chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now(); 
    AGGPosicion(poblacion, n, m, M);
    chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio); 

    mejor = CalculaMejorSolucionPoblacion(poblacion, m, M, evaluaciones);
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
    
    cout << "AGG-posicion\t" << dispersion << endl;
    cout << "Tiempo_AGG-p\t" << tiempo.count() << endl;


}