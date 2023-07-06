#ifndef RLANG_H
#define RLANG_H

#include "Kruskal.h"
#include "Data.h"

typedef struct Rlang{

	vector<vector<bool>> solution;
	vector<double> lambdas;
	vector<int> subgradients;
	int sumSubgradients;
	double cost;

}Rlang;

void setOriginalCostMatrix(vvi *const matrix, Data *const data);
void solveLagrangianRelaxation(Rlang *const base, const vvi *const distanceMatrix, const double upperBound);

#endif