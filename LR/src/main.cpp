#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <list>
#include <chrono>
#include "RLang.h"

typedef struct Node{

    Rlang rlang;
	vector<pair<int,int>> forbiddenArcs;
	int chosen;
    vector<int> chosenConnections;

}Node;

int pickBiggerDegree(vector<int> subgradients){

    int biggerIndex = 0;
    int lowerSubgradient = subgradients[0];


    for(int i = 1; i < subgradients.size(); i++){

        if(subgradients[i] < lowerSubgradient){

            biggerIndex = i;
            lowerSubgradient = subgradients[i];
        }
    }

    return biggerIndex;
}

vector<int> returnConnections(const vector<vector<bool>> *const solution, int index){

    vector<int> connections;

    for(int i = 0; i < solution->size(); i++){

        if((*solution)[index][i]){

            connections.push_back(i);
        }
    }

    return connections;
}

Node bnb(const vvi *const cost, double upperBound){

    Node root, bestNode;
    list<Node> tree;
    const int dimension = cost->size();
    
    vvi fluidCost = *cost;

    Rlang *r = new Rlang;
    solveLagrangianRelaxation(r, cost, upperBound);

    root.rlang = *r;
    root.chosen = pickBiggerDegree(r->subgradients);
    root.chosenConnections = returnConnections(&r->solution, root.chosen);

    tree.push_back(root);

    delete r;
    
    while(!tree.empty()){

        Node node;

        node = tree.back();
        tree.pop_back();

        if(node.rlang.sumSubgradients == 0){

            if(node.rlang.cost <= upperBound){
                
                upperBound = node.rlang.cost;
                bestNode = node;
                continue;
            }
        }

        for(int i = 0; i < node.chosenConnections.size(); i++){

            Node n;
            n.forbiddenArcs = node.forbiddenArcs;
            n.rlang = node.rlang;

            n.forbiddenArcs.push_back(make_pair(node.chosen, node.chosenConnections[i]));

            for(int j = 0; j < dimension; j++){

                for(int k = 0; k < dimension; k++){

                    fluidCost[j][k] = (*cost)[j][k];
                }
            }

            for(int j = 0; j < n.forbiddenArcs.size(); j++){
                
                fluidCost[n.forbiddenArcs[j].first][n.forbiddenArcs[j].second] = INFINITY;
                fluidCost[n.forbiddenArcs[j].second][n.forbiddenArcs[j].first] = INFINITY;
            }

            solveLagrangianRelaxation(&n.rlang, &fluidCost, upperBound);
            
            if(n.rlang.cost < upperBound){

                n.chosen = pickBiggerDegree(n.rlang.subgradients);
                n.chosenConnections = returnConnections(&n.rlang.solution, n.chosen);
                tree.push_back(n);
            }
        } 
    }

    return bestNode;
}

int main(int argc, char** argv){
	
	chrono::time_point<std::chrono::system_clock> start, end;

	Node result;
	
	Data *const data = new Data(argc, argv[1]);
    data->read();
	const int dimension = data->getDimension();

    const double upperBound = stod(argv[2]) + 1;
    vvi cost;

    setOriginalCostMatrix(&cost, data);

	delete data;

    for(int i = 0; i < data->getDimension(); i++){

        cost[i][i] = INFINITY;
    }

    start = chrono::system_clock::now();
    result = bnb(&cost, upperBound);
    end = chrono::system_clock::now();    
 
	chrono::duration<double> time = end - start;

    std::cout << time.count() << " " << result.rlang.cost << endl;

	return 0;
}