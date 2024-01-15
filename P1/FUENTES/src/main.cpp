#include "bl.h"
#include "greedy.h"
#include "utilidades.h"
#include <chrono>
#include <thread>
#include <ctime>

using namespace std::chrono;

struct Elemento {
    int i;
    int j;
    double distancia;
    int n;
    int m;
    
    friend ostream & operator<<(ostream & os, const Elemento & e) {
        os << e.distancia;

        return os;
    }
};

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


int main()  {

    vector<Elemento> V;
    vector<int> puntos;
    int n, m;

    cin >>  V;
    n = V[0].n;
    m = V[0].m;

    cout << "\nNumero de elementos: " << n << endl;
    cout << "ELementos a seleccionar: " << m << "\n\n";
    
    // Reservamos memoria para la Matriz.
    double **M = new double*[n];
    for(int i=0; i <n; i++) {
        M[i] = new double[n];
    }
    for(int i=0; i<V.size(); i++) {
        M[V[i].i][V[i].j] = V[i].distancia;
        M[V[i].j][V[i].i] = V[i].distancia;
    }


    int semillas[5] = {1,2,3,4,5};  

    cout << "+++++++++++++ GREEDY +++++++++++++++++++++++++++++++++++++++\n";
    double costeMedio=0, tiempoMedio=0;
    for(int i=0; i<5; i++) {
        puntos.clear();
        srand(semillas[i]);
        while(puntos.size() < 2) {
            int aleatorio = rand()%n;
            if(puntos.end() == find(puntos.begin(), puntos.end(), aleatorio)) {
                puntos.push_back(aleatorio);
            }
        }

        double dispersion = 0;

        chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now();
        Greedy(puntos, n, m, M, dispersion);
        chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();
        costeMedio += dispersion;
        //MostrarResultado(puntos, dispersion);
        // Calculo el tiempo que le ha tomado al algoritmo ejecutarse
        chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio);
        //cout <<"\nTiempo Consumido: " << tiempo.count() << " ms";
        tiempoMedio += tiempo.count();
    }
    cout << "Coste medio:   " << costeMedio/5 << endl;
    cout << "Tiempo medio:  " << tiempoMedio/5 << endl;


    cout << "\n\n+++++++++++++ BUSQUEDA LOCAL +++++++++++++++++++++++++++++++++++++++\n";

    tiempoMedio=0;
    costeMedio=0;
    for(int i=0; i<5; i++) {        
        puntos.clear();
        srand(semillas[i]);
        while(puntos.size() < m) {
            int aleatorio = rand()%n;
            if(puntos.end() == find(puntos.begin(), puntos.end(), aleatorio)) {
                puntos.push_back(aleatorio);
            }
        }

        double dispersion = 0;
        chrono::high_resolution_clock::time_point momentoInicio = chrono::high_resolution_clock::now();
        BL(puntos, n, m, M, dispersion);
        chrono::high_resolution_clock::time_point momentoFin = chrono::high_resolution_clock::now();
        costeMedio += dispersion;
        //MostrarResultado(puntos, dispersion);
        // Calculo el tiempo que le ha tomado al algoritmo ejecutarse
        chrono::duration<double> tiempo = chrono::duration_cast<chrono::duration<double>>(momentoFin - momentoInicio);
        //cout <<"\nTiempo Consumido: " << tiempo.count() << " ms";
        tiempoMedio += tiempo.count();
    }

    cout << "Coste medio:   " << costeMedio/5 << endl;
    cout << "Tiempo medio:  " << tiempoMedio/5 << endl;

    cout << "\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";


}