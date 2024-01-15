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

    while(nueva_poblacion.size()<10) {        
        // Generamos dos indices aleatorios distintos.   
        p1 = rand()%10;
        p2 = rand()%10;

        while(p1==p2) {
           p2 = rand()%10;
        }

        // Nos quedamos con el mejor padre realizando un torneo binario.
        mejor_padre = MejorSolucion(poblacion[p1], poblacion[p2], m, M, evaluaciones);

        // Lo añadimos a la población.
        nueva_poblacion.push_back(mejor_padre);
    }

    poblacion = nueva_poblacion;
}

vector<bool> Cruce(vector<bool> a, vector<bool> b, vector<int> &nosel, int n) {
    vector<bool> hijo;

    // Generamos el hijo del cruce entre 'a' y 'b'
    for(int i=0; i<n; i++) {
        // Los indices que comparte 'a' y 'b' tambien los compartirá el hijo 
        if(a[i] == b[i]) {
            hijo.push_back(a[i]);

        // Los indices que no comparten se escogerá para el hijo uno de los dos aleatoriamente.    
        }else {
            // Guardamos los indices de los puntos que no comparten para su posterior uso.
            nosel.push_back(i);
            if(rand()%2 == 0) {
                hijo.push_back(a[i]);
            }else {
                hijo.push_back(b[i]);
            }
        }
    }

    return hijo;
}

void QuitaPunto(vector<double> distancias,  vector<bool> &hijo, vector<int> &puntos, vector<int> &nosel) {
    int peor_punto;
    double media=0, mayor_dif=-1, actual_dif;
    // Calculo la media de las distancias
    for(int i=0; i<distancias.size(); i++) {
        media += distancias[i];
    }
    media /= distancias.size();

    // Busco entre todas las distancias la que su diferencia respecto a la media sea la más alta y asi eliminar ese punto.
    for(int i=0; i<distancias.size(); i++) {
        actual_dif = abs(distancias[i]-media);
        if(actual_dif > mayor_dif && nosel.end()!=find(nosel.begin(), nosel.end(), puntos[i])) {
            mayor_dif = actual_dif;
            peor_punto = i;
        }
    }

    hijo[puntos[peor_punto]] = false;
    puntos.erase(find(puntos.begin(), puntos.end(), puntos[peor_punto]));
}

void AniadePunto(vector<bool> &hijo, vector<int> &puntos, vector<int> &nosel, double **M) { 
    vector<double> distancias;
    vector<int> paux;

    int mejor_punto;
    double media=0, menor_dif=99999, actual_dif;

    // Comprobamos para cada punto no seleccionado si añadiendolo obtenemos la dispersion más baja respecto al resto.
    for(int i=0; i<nosel.size(); i++) {
        if(!hijo[nosel[i]]) {
            paux = puntos;
            paux.push_back(nosel[i]);
            SumaDistancias(distancias, paux, paux.size(), M);

            // Calculo la media de las distancias
            for(int i=0; i<distancias.size(); i++) {
                media += distancias[i];
            }
            media /= distancias.size();

            actual_dif = abs(distancias[i] - media);
            if(actual_dif < menor_dif) {
                menor_dif = actual_dif;
                mejor_punto = nosel[i];
            }
            media = 0;
        }
    }

    hijo[mejor_punto] = true;
    puntos.push_back(mejor_punto);
    nosel.erase(find(nosel.begin(), nosel.end(), mejor_punto));
}


void Reparacion(vector<bool> &hijo, vector<int> &nosel, int n, int m, double **M) {
    
    vector<int> puntos;
    // Guardamos los indices que valen 1 
    for(int i=0; i<hijo.size(); i++) {
        if(hijo[i]) {
            puntos.push_back(i);
        }
    }
    
    // Si hay más 1s de los necesarios
    if(puntos.size() > m) {
        vector<double> distancias;
        // Quitamos 1s hasta tener 'm' 1s
        while(puntos.size() != m) {
            SumaDistancias(distancias, puntos, puntos.size(), M);
            QuitaPunto(distancias, hijo, puntos, nosel);
        }
    // Si hay menos 1s de los necesarios
    }else if(puntos.size() < m) {
        while(puntos.size() != m) {
            // Añadimos 1s hasta tener 'm' 1s
            AniadePunto(hijo, puntos, nosel, M);
        }
    }
}

void OperadorCruce(vector<vector<bool>> &poblacion, int n, int m, double **M) {

    vector<vector<bool>> nueva_poblacion;
    vector<bool> hijo;    
    // Calculamos el numero de padres a cruzar con probabilidad de cruce de 0.7
    int n_cruce = poblacion.size()*0.7;
    int i;

    for(i=0; i<n_cruce; i++) {
        vector<int> nosel;
        // Obtenemos el hijo generado por el cruce de dos padres. 
        hijo = Cruce(poblacion[i], poblacion[i+1], nosel, n);
        // Reparamos el hijo
        Reparacion(hijo, nosel, n, m, M);
        // Lo añadimos a la nueva poblacion.
        nueva_poblacion.push_back(hijo);
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


void AGGUniforme(vector<vector<bool>> &poblacion, int n, int m, double **M, int &evaluaciones) {

    vector<vector<bool>> poblacion_siguiente;
    vector<bool> mejor_sol_anterior, peor_sol_actual, aux;

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


void BL(vector<int> &puntos, int n, int m, double **M, double &dispersion, int &evaluaciones) {

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
    while(eval < 100000 && mejora && vecinos < 400) {
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
            evaluaciones++;
            eval++;
        }   
    }
}


void AM_Variante1(vector<vector<bool>> &poblacion, int n, int m, double **M) {

    // Calculamos la poblacion
    CalcularPoblacion(poblacion, n, m);

    int evaluaciones=0, generacion=0;
    while(evaluaciones<100000) {

        // Cada 10 generaciones aplicamos BL
        if(generacion == 10) {
            generacion = 0;
            // Aplicamos BL  a todos los individuos de la poblacion
            for(int i=0; i<10; i++) {
                vector<int> puntos;
                // Guardamos los indices de los 1s
                for(int j=0; j<n; j++) {
                    if(poblacion[i][j]) {
                        puntos.push_back(j);
                    }
                }
                
                double dispersion;
                // Aplicamos BL
                BL(puntos, n, m, M, dispersion, evaluaciones);
                
                // SUstituimos la solucion generada en BL en la poblacion
                for(int j=0; j<poblacion[i].size(); j++) {
                    poblacion[i][j] = false;
                }
                for(int j=0; j<puntos.size(); j++) {
                    poblacion[i][puntos[j]] = true;
                }
            }
        }else {
            // Aplicamos AGG Uniforme
            AGGUniforme(poblacion, n, m, M, evaluaciones);
        }
        generacion++;
    }
}

void AM_Variante2(vector<vector<bool>> &poblacion, int n, int m, double **M) {
    
    vector<vector<bool>> poblacion_siguiente;
    vector<bool> mejor_sol_anterior, peor_sol_actual, aux;
    // Calculamos la poblacion
    CalcularPoblacion(poblacion, n, m);

    int evaluaciones=0, generacion=0;
    while(evaluaciones<100000) {     
        // Cada 10 generaciones aplicamos BL   
        if(generacion == 10) {
            generacion = 0;            
            vector<int> puntos;
            // Generamos un indice aleatorio
            int i = rand()%10;
            // Guardamos los indices que valen 1 en el cromosoma de indice 'i'
            for(int j=0; j<poblacion[i].size(); j++) {
                if(poblacion[i][j]) {
                    puntos.push_back(j);
                }
            }

            double dispersion;
            // Aplicamos BL
            BL(puntos, n, m, M, dispersion, evaluaciones);

            // SUstituimos la solucion generada en BL en la poblacion
            for(int j=0; j<poblacion[i].size(); j++) {
                poblacion[i][j] = false;
            }
            for(int j=0; j<puntos.size(); j++) {
                poblacion[i][puntos[j]] = true;
            }
            
        }else {
            // Aplicamos AGG Uniforme
            AGGUniforme(poblacion, n, m, M, evaluaciones);            
        }
        generacion++;
    }
}

void AM_Variante3(vector<vector<bool>> &poblacion, int n, int m, double **M) {
    
    vector<vector<bool>> poblacion_siguiente;
    vector<bool> mejor_sol_anterior, peor_sol_actual, aux;
    // Calculamos la poblacion
    CalcularPoblacion(poblacion, n, m);


    int evaluaciones=0, generacion=0;
    while(evaluaciones<100000) {   
        // Cada 10 generaciones aplicamos BL        
        if(generacion == 10) {
            generacion = 0;    
            vector<bool> mejor;        
            vector<int> puntos;

            // Calculamos la mejor solucion de la poblacion
            mejor = CalculaMejorSolucionPoblacion(poblacion, m, M, evaluaciones);       
            
            vector<vector<bool>>::iterator it;
            it = find(poblacion.begin(), poblacion.end(), mejor);

            // Guardamos los indices que valen 1 dell mejor
            for(int j=0; j<mejor.size(); j++) {
                if(mejor[j]) {
                    puntos.push_back(j);
                }
            }

            double dispersion;
            // Aplicamos BL al mejor
            BL(puntos, n, m, M, dispersion, evaluaciones);

            // SUstituimos la solucion generada en BL en la poblacion
            for(int j=0; j<mejor.size(); j++) {
                mejor[j] = false;
            }
            for(int j=0; j<puntos.size(); j++) {
                mejor[puntos[j]] = true;
            }

            *it = mejor;
            
        }else {
            // Aplicamos AGG Uniforme
            AGGUniforme(poblacion, n, m, M, evaluaciones);            
        }
        generacion++;
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

    //srand((unsigned) time(NULL));

    vector<vector<bool>> poblacion;
    vector<bool> mejor;
    int evaluaciones = 0;

    chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now(); 
    AM_Variante1(poblacion, n, m, M);
    //AM_Variante2(poblacion, n, m, M);
    //AM_Variante3(poblacion, n, m, M);
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
    
    cout << "AM\t" << dispersion << endl;
    cout << "Tiempo_AM\t" << tiempo.count() << endl;


}