#ifndef __GREEDY_H__
#define __GREEDY_H__

#include "utilidades.h"

void CalculaMejorPunto(vector<double> &sumadistancias, int n, vector<int> &puntos, double **M, double &dispersion);

void Greedy(vector<int> &puntos, int n, int m, double **M, double &dispersion);

#endif