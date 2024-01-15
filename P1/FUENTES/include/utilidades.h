#ifndef __UTILIDADES_H__
#define __UTILIDADES_H__

#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <algorithm>


using namespace std;

void CalculaDispersion(vector<double> distancias, double &dispersion);

void SumaDistancias(vector<double> &distancias, vector<int> puntos, int m, double **M);

void MostrarPuntos(vector<int> x);

void MostrarResultado(vector<int> &puntos, double dispersion);

#endif