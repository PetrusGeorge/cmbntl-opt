#include <iostream>
#include <vector>
#include <cmath>
#include "RLang.h"

#define MAXITERATIONS 30

void setOriginalCostMatrix(vvi *const matrix, Data *const data){

	for(int i = 1; i <= data->getDimension(); i++){

		vector<double> aux;
		
		for(int j = 1; j <= data->getDimension(); j++){

			aux.push_back(data->getDistance(i,j));
		}

		matrix->push_back(aux);
	}//read original matrix from data
}

void setRelaxedCostMatrix(vvi *const matrix, const vector<double> *const lambdas){

	for(int i = 0; i < matrix->size(); i++){

		for(int j = 0; j < matrix->size(); j++){
			if(i != j){
				(*matrix)[i][j] -= (*lambdas)[i] + (*lambdas)[j];
			}
		}
	}//change matrix cost based on lambdas

}

vvi removeFirstNode(vvi matrix){

	matrix.erase(matrix.begin());

	for(auto& k : matrix){

		k.erase(k.begin());
	}//Makes a distance matrix without node 0

	return matrix;
}

vector<vector<bool>> insertInitialPoint(vii noInitialSolution, const vvi *const cost){

	vector<vector<bool>> finalSolution(noInitialSolution.size() + 2, vector<bool>(noInitialSolution.size() + 2, false));

	double lowerCost = INFINITY;
	double secondLowerCost;
	int lowerIndex;
	int secondLowerIndex;
	vector<double> firstLayer = (*cost)[0];

	for(int i = 1; i < firstLayer.size(); i++){

		if(firstLayer[i] < lowerCost){
			
			secondLowerCost = lowerCost;
			secondLowerIndex = lowerIndex;
			lowerCost = firstLayer[i];
			lowerIndex = i;
		}
		else if(firstLayer[i] < secondLowerCost){

			secondLowerCost = firstLayer[i];
			secondLowerIndex = i;
		}

	}

	finalSolution[lowerIndex][0] = finalSolution[0][lowerIndex] =
	finalSolution[secondLowerIndex][0] = finalSolution[0][secondLowerIndex] = true; //connects node 0 with the nearest 2 nodes

	for(auto k : noInitialSolution){

		finalSolution[k.first + 1][k.second + 1] = finalSolution[k.second + 1][k.first + 1] = true;
		//changes the Kruskal method of showing the solution into a bool matrix
	}

	return finalSolution;
}

double sumArray(const vector<double> * const array){

	double sum = 0;

	for(auto k : *array){

		sum += k;
	}

	return sum;
}

vector<int> calculateSubGradient(const vector<vector<bool>> *const solution){

	vector<int> subGradient;

	subGradient.push_back(0);

	for(int i = 1; i < solution->size(); i++){

		int value = 2; //calculus for subgradient

		for(int j = i + 1; j < solution->size(); j++){

			if((*solution)[i][j]){
			
				value--;
			}
		}

		for(int j = i - 1; j >= 0; j--){

			if((*solution)[j][i]){

				value--;
			}
		}

		subGradient.push_back(value);
	}

	return subGradient;
}

int totalSubGradient(const vector<int> *const subgradient){

	int sum = 0;

	for(auto s : *subgradient){

		sum += pow(s,2);
	}

	return sum;
}

void solveLagrangianRelaxation(Rlang *const base, const vvi *const distanceMatrix, const double upperBound){

	Rlang best = *base;
	vector<double> lambdas;
	vector<vector<bool>> solution;
	int iterations = 0;
	double eps = 1;

	if(best.lambdas.empty()){

		lambdas = vector<double>(distanceMatrix->size(), 0);
		best.lambdas = lambdas;
	}
	else{

		lambdas = best.lambdas;
	}


	while(eps > 5e-4){

		double cost;
		vvi relaxedCost = *distanceMatrix;

		setRelaxedCostMatrix(&relaxedCost, &lambdas);

		Kruskal tree(removeFirstNode(relaxedCost)); //Minimal spanning Tree solver
		cost = tree.MST(relaxedCost.size() - 1) + (2* sumArray(&lambdas));

		solution = insertInitialPoint(tree.getEdges(), &relaxedCost);
		
		for(int i = 1; i < solution[0].size(); i++){

			if(solution[0][i]){

				cost += relaxedCost[0][i];
			}
		}

		vector<int> subGradient = calculateSubGradient(&solution);
		int sumSubGradient = totalSubGradient(&subGradient);
		double mi;

		if(cost > best.cost){

			best.cost = cost;
			best.lambdas = lambdas;
			best.solution = solution;
			best.subgradients = subGradient;
			best.sumSubgradients = sumSubGradient;
			iterations = 0;
		}

		else{

			iterations++;

			if(iterations >= MAXITERATIONS){

				iterations = 0;
				eps /= 2;
			}
		}

		if(sumSubGradient == 0){
	
			best.cost = cost;
			best.lambdas = lambdas;
			best.solution = solution;
			best.subgradients = subGradient;
			best.sumSubgradients = sumSubGradient;
			break;
		}
 		else if(upperBound - cost < 1 - 1e-2){

			best.cost = INFINITY;
			break;
		}
		
		mi = (eps*(upperBound - cost))/ sumSubGradient;

		for(int i = 0; i < lambdas.size(); i++){

			lambdas[i] += mi * (subGradient[i]);
		}
	}

	*base = best;
}