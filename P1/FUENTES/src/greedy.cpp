#include "greedy.h"


void CalculaMejorPunto(vector<double> &sumadistancias, int n, vector<int> &puntos, double **M, double &dispersion) {

    bool mejora = false;
    vector<double> sumaux, sumacandidata;
    double dispaux;
    double dispcandidata = 999999999;
    int puntocandidato;

    for(int i=0; i<n && !mejora; i++) {

        // Si el punto no está seleccionado, estudiamos si mejora la dispersion o es el que la empeora menos.
        if(puntos.end()==find(puntos.begin(),puntos.end(),i)) {
            
            sumaux = sumadistancias;
            sumaux.resize(puntos.size()+1);

            // Inicializamos a 0 la suma de dsitancias del punto candidato a añadir
            sumaux[puntos.size()] = 0;
            for(int j=0; j<puntos.size(); j++) {
                // Actualizamos las sumas de distancias entres los diferentes puntos.
                sumaux[j] = sumaux[j] + M[i][puntos[j]];
                sumaux[puntos.size()] = sumaux[puntos.size()] + M[i][puntos[j]];
            }

            // Una vez calculadas las sumas de las distancias calcularemos la dispersion entre ellas
            CalculaDispersion(sumaux, dispaux);

            // Comprobamos si la dispersion mejora 
            if(dispaux < dispersion) {
                sumadistancias = sumaux;
                puntos.push_back(i);
                dispersion = dispaux;
                // Como la dispersion se mejora nos quedamos con ese punto y salimos del bucle.
                mejora=true;

             // Si no mejora comprobamos que sea el que menos empeore la dispersion y lo guardamos como candidato.
            }else if(dispaux < dispcandidata) {
                sumacandidata = sumaux;
                puntocandidato = i;
                dispcandidata = dispaux;
            }
        }
    }

    // Si no ha mejorado la dispersion nos quedamos con el que menos la empeora.
    if(!mejora) {
        sumadistancias = sumacandidata;
        puntos.push_back(puntocandidato);
        dispersion = dispcandidata;
    }
}



void Greedy(vector<int> &puntos, int n, int m, double **M, double &dispersion) {
   
    vector<double> sumadist;
    sumadist.resize(m);

    // Calculamos la distancia de los 2 puntos iniciales.
    sumadist[0] = M[puntos[1]][puntos[0]];
    sumadist[1] = M[puntos[1]][puntos[0]];

    // Mientras no tengamos m puntos, calcularemos el siguiente primer mejor punto
    while(puntos.size() < m) {
        CalculaMejorPunto(sumadist, n, puntos, M, dispersion);
    }
}