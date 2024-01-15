#include "utilidades.h"

using namespace std;

void CalculaDispersion(vector<double> distancias, double &dispersion) {

    // Calculamos la distancia maxima y la distancia minima
    double maximo = *max_element(distancias.begin(), distancias.end());
    double minimo = *min_element(distancias.begin(), distancias.end());

    // Calculamos la dispersion que es la diferencia entre la distancia maxima y la minima
    dispersion = maximo - minimo;
}

void SumaDistancias(vector<double> &distancias, vector<int> puntos, int m, double **M) {

    distancias.resize(m);

    // Recorremos los puntos
    for(int i=0; i<m; i++) {
        // Inicializamos la distancia del punto de indice i a los demas a 0
        distancias[i] = 0;
        for(int j=0; j<m; j++) {
            // Sumamos a la distancia del punto de indice i con el resto la distancias con el punto de indice j
            distancias[i] +=  M[puntos[i]][puntos[j]];
        }
    }
}

void MostrarPuntos(vector<int> x) {
    for(int k=0; k<x.size(); k++) {
        cout << x[k] << " ";  
    }
    cout << endl;
}

void MostrarResultado(vector<int> &puntos, double dispersion) {
    cout << "Puntos con menos dispersion:\n";
    MostrarPuntos(puntos);
    cout << "Dispersion: " << dispersion << endl;
}



istream & operator >>(istream & is, vector<Elemento> &V) {
    int fil, col, n, m;
    double distancia;
    is>>n;
    is>>m;
    is.ignore();//quitamos \n
    vector<Elemento> Vaux;
    int tam = ((n*n)/2)-(n/2);
    Elemento ele;
    ele.n = n;
    ele.m = m;

    for(int i=0; i<tam; i++) {
        is>>fil;
        is>>col;
        is>>distancia;
        ele.i = fil;
        ele.j = col;
        ele.distancia = distancia;
        Vaux.push_back(ele);        
        is.ignore();
    }
    V=Vaux;
    return is;
}
