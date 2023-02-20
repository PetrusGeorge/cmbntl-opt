#include <iostream>
#include <vector>
#include <chrono>
#include "Data.h"
#include "hungarian.h"

using namespace std;

int dimension;
//Candidate vertex for random choices
vector<int> CLFix;

typedef struct Node{

	vector<pair<int,int>> forbiddenArcs;
	vector<vector<int>> subtour;
	double lowerBound;
	int chosen;
	bool feasible;

}Node;

vector<vector<int>> findSubtour(hungarian_problem_t *h){

    vector<vector<int>> subtours;
    vector<int> currentSubtour;
    vector<int> vertexList = CLFix;
    int first, now;

    vertexList.insert(vertexList.begin(), 1);

    while(!vertexList.empty()){

        first = now = vertexList[0];
        currentSubtour.push_back(now);
        vertexList.erase(vertexList.begin());

        for(int i = 1; i <= h->num_cols; i++){

            if(h->assignment[now - 1][i - 1] ==  1){

                currentSubtour.push_back(i);
                
                for(int j = 0; j < vertexList.size(); j++){

                    if(vertexList[j] == i){

                        vertexList.erase(vertexList.begin() + j);
                    }
                }

                if(i == first){

                    break;
                }

                else{

                    now = i;
                    i = 0;
                }
            }
        }

        subtours.push_back(currentSubtour);        
        currentSubtour.clear();
    }


    return subtours;
}

int findSmallestSubtour(vector<vector<int>> subtour){

    int smallestIndex = 0, smallestSize;

    for(int i = 1; i < subtour.size(); i++){

        smallestSize = subtour[smallestIndex].size();

        if(subtour[i].size() < smallestSize){

            smallestIndex = i;
        }
    }

    return smallestIndex;
}

Node bnb(double **cost){

    Node root, bestNode;
    vector<Node> tree;
    
    double **fluidCost = new double*[dimension];

    for(int i = 0; i < dimension; i++){

        fluidCost[i] = new double[dimension];

        for(int j = 0; j < dimension; j++){

            fluidCost[i][j] = cost[i][j];
        }
    }

    hungarian_problem_t h;
    hungarian_init(&h,cost, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);

    double upperBound = 1000000000;

    root.lowerBound = hungarian_solve(&h);
    root.subtour = findSubtour(&h);
    root.feasible = root.subtour.size() == 1;
    root.chosen = findSmallestSubtour(root.subtour);

    tree.push_back(root);

    hungarian_free(&h);
    
    while(!tree.empty()){

        Node node;
        node = tree.back();
        tree.erase(tree.end());

        if(node.feasible){

            if(node.lowerBound <= upperBound){
                
                upperBound = node.lowerBound;
                bestNode = node;
            }

            continue;
        }


        for(int i = 0; i < node.subtour[node.chosen].size() - 1; i++){

            Node n;
            n.forbiddenArcs = node.forbiddenArcs;

            pair<int, int> forbiddenArc = {node.subtour[node.chosen][i], node.subtour[node.chosen][i + 1]
            };

            n.forbiddenArcs.push_back(forbiddenArc);

            for(int j = 0; j < dimension; j++){

                for(int k = 0; k < dimension; k++){

                    fluidCost[j][k] = cost[j][k];
                }
            }

            for(int j = 0; j < n.forbiddenArcs.size(); j++){

                fluidCost[n.forbiddenArcs[j].first - 1][n.forbiddenArcs[j].second - 1] = 1000000000;
            }

            hungarian_problem_t h2;

            hungarian_init(&h2, fluidCost, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);

            n.lowerBound = hungarian_solve(&h2);
            
            if(n.lowerBound <= upperBound ){

                n.subtour = findSubtour(&h2);
                n.feasible = n.subtour.size() == 1;
                n.chosen = findSmallestSubtour(n.subtour);

                tree.push_back(n);
            }
            
            hungarian_free(&h2);        
        } 
    }
    
    for(int i = 0; i < dimension; i++) delete [] fluidCost[i];
    delete [] fluidCost;

    return bestNode;
}


int main(int argc, char** argv){
	
	chrono::time_point<std::chrono::system_clock> start, end;

	Node result;
	
	Data * data = new Data(argc, argv[1]);
	data->read();
	dimension = data->getDimension();

    for(int i = 1; i <= dimension; i++){

        CLFix.push_back(i);
    }

	double **cost = new double*[dimension];
    
	for (int i = 0; i < dimension; i++){
		
        cost[i] = new double[dimension];
		
        for (int j = 0; j < dimension; j++){

			cost[i][j] = data->getDistance(i + 1,j + 1);
		}
	}

    for(int i = 0; i < dimension; i++){

        cost[i][i] = 1000000000;
    }
    
    start = chrono::system_clock::now();    
    result = bnb(cost);
    end = chrono::system_clock::now();    
 
	chrono::duration<double> time = end - start;

    cout << time.count() << " " << result.lowerBound << endl;
    
	for (int i = 0; i < dimension; i++) delete [] cost[i];
	delete [] cost;
	delete data;
	
	return 0;
}