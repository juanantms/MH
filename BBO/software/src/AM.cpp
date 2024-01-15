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


void BBO(vector<vector<bool>> &ecosistema, int n, int m, double **M) {

    vector<double> dispersiones_ecosistema;
    
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
    // Realizaremos el BBO durante 100 generaciones de ecosistemas
    while(generaciones<100) {
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

}


void IntercambiarPunto(pair<int, int> tupla, vector<int> &puntos, vector<double> &distancias, double **M) {

    double aux=0, sumapunto=0;

    // Recorremos los puntos y el vector distancias para ir actualizandolo:
    for(int i = 0; i<puntos.size(); i++) {
        // Si coincide el punto a actualizar con el que vamos a eliminar guardamos su distancia total para un posterior uso.
        if(puntos[i] == tupla.first) {
            sumapunto=distancias[i];
        }else{
            // Restamos al punto de indice i la distancia con el punto a eliminar que es tupla.first
            distancias[i] -=  M[puntos[i]][tupla.first];
            // Sumamos al punto de indice i la distancia con el punto a añadir que es tupla.second
            distancias[i] +=  M[puntos[i]][tupla.second];
            // Guardamos la distancia entre los puntos con el que vamos a añadir
            aux += M[puntos[i]][tupla.second];
        }
    }

    // Utilizamos iteradores para intercambiar los valores
    vector<int>::iterator it_p = find(puntos.begin(), puntos.end(), tupla.first);
    // Intercambiamos el valor del punto a eliminar con el que vamos a añadir
    *it_p = tupla.second;
    // Utilizamos la varible sumapunto para buscar la posicion en la que está en el vector de distancias
    vector<double>::iterator it_d = find(distancias.begin(), distancias.end(), sumapunto);
    // Intercambiamos el valor de la distancia del punto a eliminar con la que hemos calculado del punto a añadir
    *it_d = aux;
}


void BL(vector<int> &puntos, int n, int m, double **M, double &dispersion, int evaluaciones) {

    vector<int> restantes, paux;
    vector<double> distancias, daux;

    // Creamos el vector con los puntos restantes o no seleccionados
    for(int i=0; i<n; i++) {
        if(puntos.end() == find(puntos.begin(), puntos.end(), i)) {
            restantes.push_back(i);
        }
    }

    // Calculamos las sumas de las distancias de un punto con los demas para cada uno.
    SumaDistancias(distancias, puntos, m, M);
    // Calculados la dispersion
    CalculaDispersion(distancias, dispersion);

    paux = puntos;
    daux = distancias;
    double dispersion_candidata;
    bool mejora = true;
    vector<pair<int, int>> tuplas;
    int vecinos=0;
    int eval=0;

    // Buscaremos aleatoriamente una mejor solucion de la actual mientras hagamos menos de 100000 iteraciones o que no se encuentre ninguna mejor solución.
    while(eval < evaluaciones && mejora && vecinos < 400) {
        // Generamos el vecindario
        // Una tupla la representadomos con un pair<int,int> donde first es el punto a eliminar y second con el punto a añadir
        pair<int, int> tupla;
        tuplas.clear();
        // Generamos todas las posibles combinaciones que existen entre los puntos seleccionados y los no seleccionados
        for(int i=0; i<puntos.size(); i++) {
            for(int j=0; j<restantes.size(); j++) {
                tupla.first = puntos[i];
                tupla.second = restantes[j];
                tuplas.push_back(tupla);
            }
        }

        // Desordenamos aleatoriamente el vector con los puntos no seleccionados
        random_shuffle(tuplas.begin(), tuplas.end());

        mejora = false;
        // Probamos tuplas hasta que la dispersion se mejore
        for (int i = 0; i < tuplas.size() && !mejora; i++) {
            vecinos++;
            IntercambiarPunto(tuplas[i], paux, daux, M);
            CalculaDispersion(daux, dispersion_candidata);

            // Si la dispersion mejora
            if(dispersion_candidata < dispersion) {
                // Actualizamos la nueva mejor dispersion
                dispersion = dispersion_candidata;
                // los puntos que mejoran la anterior dispersion
                puntos = paux;
                // las nuevas distancias entre los nuevos puntos
                distancias = daux;

                // Cambiamos 'mejora' a true para salir del bucle
                mejora = true;

                // Actualizamos los puntos restantes o no seleccionados añadiendo el punto que intercambiamos por el nuevo
                restantes.push_back(tuplas[i].first);
                // y eliminando el punto que disminuye la dispersion.
                restantes.erase(find(restantes.begin(), restantes.end(), tuplas[i].second));
            
            }else {
                // Si no mejora restablecemos los cambios en paux y daux a las ultima solucion
                paux = puntos;
                daux = distancias;
            }

            // Por cada tupla que comprobamos si mejora la dispersion incrementaremos 'evaluaciones' en 1
            eval++;
        }   
    }
}


void AM(vector<vector<bool>> &ecosistema, int n, int m, double **M) {

    // Calculamos la ecosistema
    CalcularEcosistema(ecosistema, n, m);

    int evaluaciones=0, generacion=0;
    while(generacion<20) {
        // Cada 5 generaciones aplicamos BL
        if(generacion%5 == 0) {
            // Aplicamos BL  a todos los individuos de la ecosistema
            for(int i=0; i<10; i++) {
                vector<int> puntos;
                // Guardamos los indices de los 1s
                for(int j=0; j<n; j++) {
                    if(ecosistema[i][j]) {
                        puntos.push_back(j);
                    }
                }
                
                double dispersion;
                // Aplicamos BL
                BL(puntos, n, m, M, dispersion, 10000);
                
                // Sustituimos la solucion generada en BL en la ecosistema
                for(int j=0; j<ecosistema[i].size(); j++) {
                    ecosistema[i][j] = false;
                }
                for(int j=0; j<puntos.size(); j++) {
                    ecosistema[i][puntos[j]] = true;
                }
            }
        }else {
            // Aplicamos BBO
            BBO(ecosistema, n, m, M);
        }
        generacion++;
    }

    vector<double> dispersiones_ecosistema;
    CalcularDispersionesEcosistema(ecosistema, dispersiones_ecosistema, m, M);

    OrdenarEcosistema(dispersiones_ecosistema, ecosistema);
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

    //srand((unsigned) time(NULL));

    vector<vector<bool>> ecosistema;
    vector<bool> mejor;
    int evaluaciones = 0;

    chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now(); 
    AM(ecosistema, n, m, M);
    chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio); 

    mejor = ecosistema[0];
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
    
    cout << "AM\t" << dispersion << endl;
    cout << "Tiempo_AM\t" << tiempo.count() << endl;
}