#include "bl.h"


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


void BL(vector<int> &puntos, int n, int m, double **M, double &dispersion, int maxEvals) {

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
    int evaluaciones = 0;


    // Buscaremos aleatoriamente una mejor solucion de la actual mientras hagamos menos de 100000 iteraciones o que no se encuentre ninguna mejor solución.
    while(evaluaciones < maxEvals && mejora) {
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
        }   
    }

    // Mostramos el numero de evaluaciones
    //cout << "\n------>  Evaluaciones: " << evaluaciones << endl;
}
