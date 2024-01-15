#ifndef __BL_H__
#define __BL_H__

#include "utilidades.h"

void IntercambiarPunto(pair<int, int> tupla, vector<int> &puntos, vector<double> &distancias, double **M);

void BL(vector<int> &puntos, int n, int m, double **M, double &dispersion);

#endif