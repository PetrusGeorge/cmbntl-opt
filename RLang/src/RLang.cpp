#include <iostream>
#include <vector>
#include <cmath>
#include "Data.h"
#include "Kruskal.h"

#define MAXITERATIONS 30

vvi teste = {{0,30,26,50,40},
			 {30,0,24,40,50},
			 {26,24,0,24,26},
		 	 {50,40,24,0,30},
		 	 {40,50,26,30,0}};

void setOriginalCostMatrix(vvi *matrix, Data *data){


	for(int i = 0; i < data->getDimension(); i++){

		vector<double> aux;
		
		for(int j = 0; j < data->getDimension(); j++){

			aux.push_back(data->getDistance(i,j));
		}

		matrix->push_back(aux);
	}//read original matrix from data
}

void setRelaxedCostMatrix(vvi *matrix, vector<double> *lambdas){

	for(int i = 0; i < matrix->size(); i++){

		for(int j = 0; j < matrix->size(); j++){

			(*matrix)[i][j] -= (*lambdas)[i] + (*lambdas)[j];
		}
	}

}

vvi removeFirstNode(vvi matrix){

	matrix.erase(matrix.begin());

	for(auto& k : matrix){

		k.erase(k.begin());
	}

	return matrix;
}

vector<vector<bool>> insertInitialPoint(vii noInitialSolution, vvi *cost){

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
	finalSolution[secondLowerIndex][0] = finalSolution[0][secondLowerIndex] = true;

	for(auto k : noInitialSolution){

		finalSolution[k.first + 1][k.second + 1] = finalSolution[k.second + 1][k.first + 1] = true;
	}

	return finalSolution;
}

double sumArray(vector<double> *array){

	double sum = 0;

	for(auto k : *array){

		sum += k;
	}

	return sum;
}

vector<int> calculateSubGradient(vector<vector<bool>> solution){

	vector<int> subGradient;

	subGradient.push_back(0);

	for(int i = 1; i < solution.size(); i++){

		int value = 2; //calculus for subgradient

		for(int j = i + 1; j < solution.size(); j++){

			if(solution[i][j]){
				value--;
				cout << "a";
			}
		}

		for(int j = i - 1; j >= 0; j--){

			if(solution[j][i]){

				value--;
				cout << "b";
			}
		}
		cout << endl;
		subGradient.push_back(value);
	}

	return subGradient;
}

int main(int argc, char** argv){


	Data * data = new Data(argc, argv[1]);
	data->read();

	vvi *distanceMatrix = &teste; //original matrix

	vector<double> lambdas(distanceMatrix->size(), 0);
	vector<double> bestLambdas = lambdas;
	int iterations = 0, eps = 1;
	double bestCost = 0, upperBound = 148;

	int i = 0;


	while(eps > 5e-4){

		double cost;
		vvi relaxedCost = *distanceMatrix;

		setRelaxedCostMatrix(&relaxedCost, &lambdas);

		for(auto x : relaxedCost){

			for(auto y : x){

				cout << y << " ";
			}

			cout << endl;
		}

		Kruskal tree(removeFirstNode(relaxedCost)); //Minimal spanning Tree solver
		
		cost = tree.MST(teste.size()) + (2* sumArray(&lambdas));
		
		vector<vector<bool>> solution = insertInitialPoint(tree.getEdges(), &relaxedCost);

		for(int i = 1; i < solution[0].size(); i++){

			if(solution[0][i]){

				cost += relaxedCost[0][i];
			}
		}

		cout << "Cost: " << cost << endl;

		if(cost > bestCost){

			bestCost = cost;
			bestLambdas = lambdas;
			iterations = 0;
		}


		else{

			iterations++;
			if(iterations >= MAXITERATIONS){

				iterations = 0;
				eps /= 2;
			}
		}
		for(auto p : solution){

			for(auto a : p){

				cout << a << " ";
			}

			cout << endl;
		}
		vector<int> subGradient = calculateSubGradient(solution);

		for(auto s : subGradient){

			cout << "Sub: " << s << endl;
		}
		
		double mi;

	
		int sumSubGradient = 0;

		for(auto s : subGradient){

			sumSubGradient += pow(s,2);
		}
		mi = (eps*(upperBound - cost))/ sumSubGradient;

		cout << "Mi:" << mi << endl;

		for(int i = 0; i < lambdas.size(); i++){

			lambdas[i] += mi * (subGradient[i]);
		}

		for(auto l : lambdas){

			cout << "Lambda: " << l << endl;
		}

	}
	cout << bestCost << endl;
	//delete distanceMatrix;
	delete data;
}