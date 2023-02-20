#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <chrono>
#include "Data.h"
#include "hungarian.h"

using namespace std;

#define BACK 0
#define FRONT 1

int dimension;
//Candidate vertex for random choices
vector<int> CLFix;
vector<int> CL;

typedef struct Solution{
    
    vector<int> sequence;
    double total;
    
}Solution;

typedef struct Insertion{
    
    int insertedVertex;
    int removedVertex;
    double delta;
    
}Insertion;

Solution chooseRandom(Data& data){

    Solution choice {{1},0};

    //Choose three random vertex
    for(int i = 0; i < 3; i++){ 
        
        unsigned random = rand() % CL.size();
    
        choice.sequence.push_back(CL[random]);
        CL.erase(CL.begin() + random);
    }

    choice.sequence.push_back(1);
    
    //Calculate total distance
    for(int a = 0, b = 1; b < choice.sequence.size(); a++, b++){
        
        choice.total += data.getDistance(choice.sequence[a],choice.sequence[b]);
    }

    return choice;
}

vector<Insertion> calculateInsertionCost(const Solution *subTour, Data& data){
    
    vector<Insertion> insertionCost((subTour->sequence.size() - 1) * CL.size());
    
    int l = 0;
    
    for(int a = 0, b = 1; b < subTour->sequence.size(); a++, b++){
        
        int i = subTour->sequence[a], j = subTour->sequence[b];

        for(auto k : CL){
            
            insertionCost[l].delta = data.getDistance(i,k) + data.getDistance(k,j) - data.getDistance(i,j);
            insertionCost[l].removedVertex = b;
            insertionCost[l].insertedVertex = k;
            l++;
        }
    }
    
    return insertionCost;
}

bool compare(Insertion x, Insertion y){

    return x.delta < y.delta;
}

void insertRandom(Solution *incompleteSolution, vector<Insertion> list){
    
    double alpha = ((double) rand() / RAND_MAX) + 0.00001;
    int chose = rand() % ((int) ceil(alpha * list.size()));
    
    incompleteSolution->sequence.insert(incompleteSolution->sequence.begin() + list[chose].removedVertex, list[chose].insertedVertex);
    incompleteSolution->total += list[chose].delta;
    
    for(int i = 0; i < CL.size(); i++){
        
        if(list[chose].insertedVertex == CL[i]){
            CL.erase(CL.begin() + i);
            break;
        }
    }
}

Solution construction(Data& data){
 
    //reset candidate vertices
    CL = CLFix;
    Solution s;
    
    s = chooseRandom(data);

    while(!CL.empty()){
        
        vector<Insertion> insertionCost = calculateInsertionCost(&s, data);

        sort(insertionCost.begin(), insertionCost.end(), compare);

        insertRandom(&s, insertionCost);
    }
    
    return s;
}

bool bestSwap(Solution *s, Data& data){

    double bestDelta = 0, delta;
    int bestA, bestB;
    int size = s->sequence.size();
    int vertexA, vertexANext, vertexAPre;
    int vertexB, vertexBNext, vertexBPre;

    for(int i = 1; i < size - 1; i++){

        vertexA = s->sequence[i];
        vertexANext = s->sequence[i + 1];
        vertexAPre = s->sequence[i - 1];

        for(int j = i + 1; j < size - 1; j++){

            vertexB = s->sequence[j];
            vertexBNext = s->sequence[j + 1];
            vertexBPre = s->sequence[j - 1];
            
            if(j != i + 1){
                delta = data.getDistance(vertexA,vertexBPre) + data.getDistance(vertexA,vertexBNext) + data.getDistance(vertexB, vertexAPre) + data.getDistance(vertexB,vertexANext)
                -data.getDistance(vertexA, vertexAPre) - data.getDistance(vertexA, vertexANext) - data.getDistance(vertexB, vertexBPre) - data.getDistance(vertexB, vertexBNext);
            }

            else{
                delta = data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexB,vertexAPre) - data.getDistance(vertexA, vertexAPre) - data.getDistance(vertexB,vertexBNext);
            }

            if(delta < bestDelta){
                bestDelta = delta;
                bestA = i;
                bestB = j;
            }
        }
    }

    if(bestDelta < 0){
        swap(s->sequence[bestA], s->sequence[bestB]);
        s->total += bestDelta;
        return true;
    }

    return false;
}

bool bestOrOPT(Solution *s, int choice, Data& data){

    double bestDelta = 0, delta;
    int bestA, bestB;
    int size = s->sequence.size();
    int vertexA, vertexAPre, vertexANext;
    int vertexB, vertexBPre, vertexBNext;

    if(choice == 1){

        for(int i = 1; i < size - 2; i++){

            vertexA = s->sequence[i];
            vertexANext = s->sequence[i+1];
            vertexAPre = s->sequence[i-1]; 

            for(int j = 0; j < size - 1; j++){

                if(i - j <= 1 && i - j >= 0){
                    continue;
                }

                vertexB = s->sequence[j];
                vertexBNext = s->sequence[j + 1];

                delta = data.getDistance(vertexA, vertexB) + data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexAPre, vertexANext)
                -(data.getDistance(vertexA, vertexAPre) +  data.getDistance(vertexA, vertexANext) + data.getDistance(vertexB, vertexBNext));
            
                if(delta < bestDelta){
                    bestDelta = delta;
                    bestA = i;
                    bestB = j;
                }
            }
            
        }
            
        if(bestDelta < 0){
            
            if(bestA > bestB){
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.erase(s->sequence.begin() + bestA + 1);
            }

            else{
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.erase(s->sequence.begin() + bestA);
            }
            
            s->total += bestDelta;
            return true;
        }

    }

    else if(choice == 2){

        int vertexA2;

        for(int i = 1; i < size - 3; i++){

            vertexA = s->sequence[i];
            vertexA2 = s->sequence[i + 1];
            vertexANext = s->sequence[i+2];
            vertexAPre = s->sequence[i-1]; 

            for(int j = 0; j < size - 1; j++){

                if(i - j <= 1 && i - j >= -1){
                    continue;
                }

                vertexB = s->sequence[j];
                vertexBNext = s->sequence[j + 1];

                delta = data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexA2, vertexB) + data.getDistance(vertexAPre, vertexANext) 
                - (data.getDistance(vertexA, vertexAPre) + data.getDistance(vertexA2, vertexANext) + data.getDistance(vertexB, vertexBNext));
            
                if(delta < bestDelta){
                    bestDelta = delta;
                    bestA = i;
                    bestB = j;
                }
            }
            
        }
            
        if(bestDelta < 0){
            
            if(bestA > bestB){

                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.erase(s->sequence.begin() + bestA + 2);
                s->sequence.erase(s->sequence.begin() + bestA + 2);
            }
            else{

                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 1]);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
            }

            s->total += bestDelta;
            return true;
        }

    }

    else if(choice == 3){

        int vertexA3;

        for(int i = 1; i < size - 4; i++){

            vertexA = s->sequence[i];
            vertexA3 = s->sequence[i + 2];
            vertexANext = s->sequence[i+3];
            vertexAPre = s->sequence[i-1]; 

            for(int j = 0; j < size - 1; j++){
                
                if(i - j <= 2 && i - j >= -2){
                    continue;
                }

                vertexB = s->sequence[j];
                vertexBNext = s->sequence[j + 1];

                delta = data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexA3, vertexB) + data.getDistance(vertexAPre, vertexANext) 
                - (data.getDistance(vertexA, vertexAPre) + data.getDistance(vertexA3, vertexANext) + data.getDistance(vertexB, vertexBNext));
            
                if(delta < bestDelta){
                    bestDelta = delta;
                    bestA = i;
                    bestB = j;
                }
            }
            
        }
            
        if(bestDelta < 0){

            if(bestA > bestB){
            
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 4]);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
            }
            else{

                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 1]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
            }      
            s->total += bestDelta;
            return true;
        }

    }

    return false;
}

bool best2OPT(Solution *s, Data& data){


    double bestDelta = 0, delta;
    int bestA, bestB;
    int size = s->sequence.size();
    int vertexA, vertexANext;
    int vertexB, vertexBNext;

    for(int i = 0; i < size - 1; i++){

        vertexA = s->sequence[i];
        vertexANext = s->sequence[i + 1];

        for(int j = i + 2; j < size - 1; j++){

            if(i == 0 && j == size - 2){
                continue;
            }

            vertexB = s->sequence[j];
            vertexBNext = s->sequence[j + 1];
            
            delta = data.getDistance(vertexA, vertexB) + data.getDistance(vertexANext, vertexBNext) 
            - (data.getDistance(vertexA, vertexANext) + data.getDistance(vertexB, vertexBNext));

            if(delta < bestDelta){
                bestDelta = delta;
                bestA = i;
                bestB = j;
            }
        }
    }

    if(bestDelta < 0){
        vector<int> change;
        vector<int> inverted;
        
        for(int i = bestB; i > bestA; i--){

            inverted.push_back(s->sequence[i]);
        }

        change.insert(change.begin(), s->sequence.begin(), s->sequence.begin() + bestA + 1);
        change.insert(change.end(), inverted.begin(), inverted.end());
        change.insert(change.end(), s->sequence.begin() + bestB + 1, s->sequence.end());

        s->sequence = change;

        s->total += bestDelta;
        return true;
    }

    return false;
}

void localSearch(Solution *s, Data& data){

    vector<int> options {1, 2, 3, 4, 5};
    bool better = false;

    while(options.empty() == false){

        int n = rand() % options.size();

        switch (options[n]){
        case 1:
            better = bestSwap(s, data);
            break;
        
        case 2:
            better = best2OPT(s, data);
            break;

        case 3:
            better = bestOrOPT(s, 1, data);
            break;
        
        case 4:
            better = bestOrOPT(s, 2, data);
            break;

        case 5:
            better = bestOrOPT(s, 3, data);
            break;
        }

        if(better){
            options = {1, 2, 3, 4, 5};
        }
        else{
            options.erase(options.begin() + n);
        }
        
    }
}

void Pertubacao(const Solution *s, Solution *receiver, Data& data){

    vector<int> sequenceCopy = s->sequence;
    int qtyBack, qtyFront;
    int sizeB , sizeA, indexB, indexA;
    double delta, distance;

    if(dimension > 30){
     
        sizeA = (rand() % ((int)ceil(s->sequence.size()/10)))+ 3;
    }

    else{
     
        sizeA = (rand() % 2) + 2;
    }
    indexA = (rand() % (s->sequence.size() - sizeA - 1)) + 1;
    qtyBack = indexA - 1;
    qtyFront = s->sequence.size() - indexA - sizeA - 1;

    int bOrF = rand() % 2;

    if(qtyBack == 0){
        bOrF = FRONT;
    }

    else if(qtyFront == 0){
        bOrF = BACK;
    }
    
    if(bOrF == BACK){

        sizeB = (rand() % qtyBack) + 1;
        indexB = (rand() % (qtyBack - sizeB + 1)) + 1;

        if(sizeA > sizeB){
        
            for(int i = 0; i < sizeB; i++){
                
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for(int i = 0; i < sizeA - sizeB; i++){

                sequenceCopy.insert(sequenceCopy.begin() + indexB + sizeB, sequenceCopy[indexA + sizeA - 1]);
                sequenceCopy.erase(sequenceCopy.begin() + indexA + sizeA);
            }
        }
        
        else{
        
            for(int i = 0; i < sizeA; i++){
                
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for(int i = 0; i < sizeB - sizeA; i++){

                sequenceCopy.insert(sequenceCopy.begin() + indexA + sizeA, sequenceCopy[indexB + sizeA]);
                sequenceCopy.erase(sequenceCopy.begin() + indexB + sizeA);
            }
        }
    }
     
    else if(bOrF == FRONT){

        sizeB = (rand() % qtyFront) + 1;
        indexB = (rand() % (qtyFront - sizeB + 1)) + indexA + sizeA;

        if(sizeA > sizeB){
            
            for(int i = 0; i < sizeB; i++){
                
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for(int i = 0; i < sizeA - sizeB; i++){
                
                sequenceCopy.insert(sequenceCopy.begin() + indexB + sizeB, sequenceCopy[indexA + sizeB]);
                sequenceCopy.erase(sequenceCopy.begin() + indexA + sizeB);
            }
        }
        else{

            for(int i = 0; i < sizeA; i++){
                
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for(int i = 0; i < sizeB - sizeA; i++){

                sequenceCopy.insert(sequenceCopy.begin() + indexA + sizeA, sequenceCopy[indexB + sizeB - 1]);
                sequenceCopy.erase(sequenceCopy.begin() + indexB + sizeB);
            }
        }      
    }
    
    if(indexA + sizeA != indexB && indexB + sizeB != indexA){
        delta = data.getDistance(s->sequence[indexA], s->sequence[indexB - 1]) + data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexB + sizeB])
        + data.getDistance(s->sequence[indexB], s->sequence[indexA - 1]) + data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexA + sizeA])
        -(data.getDistance(s->sequence[indexA], s->sequence[indexA - 1]) + data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexA + sizeA])
        + data.getDistance(s->sequence[indexB], s->sequence[indexB - 1]) + data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexB + sizeB]));
    }

    else if(bOrF == BACK){

        delta = data.getDistance(s->sequence[indexA], s->sequence[indexB - 1]) + data.getDistance(s->sequence[indexB], s->sequence[indexA + sizeA - 1]) 
        + data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexA + sizeA])-(data.getDistance(s->sequence[indexA], s->sequence[indexA - 1]) 
        + data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexA + sizeA]) + data.getDistance(s->sequence[indexB], s->sequence[indexB - 1]));
    }

    else if(bOrF == FRONT){

        delta = data.getDistance(s->sequence[indexB + sizeB - 1],s->sequence[indexA]) + data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexB + sizeB])
        + data.getDistance(s->sequence[indexB], s->sequence[indexA - 1])
        -(data.getDistance(s->sequence[indexA], s->sequence[indexA - 1]) + data.getDistance(s->sequence[indexB], s->sequence[indexB-1]) + data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexB + sizeB]));
    }
    

    receiver->sequence = sequenceCopy;
    receiver->total = delta + s->total;  
}

Solution ILS(int maxIter, int maxIterILS, Data& data){
    
    Solution bestOfAll;
    bestOfAll.total = INFINITY;
    
    for(int i = 0; i < maxIter; i++){
        
        Solution now = construction(data); 
        Solution bestNow = now;
        
        int ls = 0;
        
        while(ls <= maxIterILS){
            localSearch(&now, data);

            if(now.total < bestNow.total){
                
                bestNow = now;
                ls = 0;
            }
            
            Pertubacao(&bestNow, &now, data);
            ls++;
        }
        
        if(bestNow.total < bestOfAll.total){
        
            bestOfAll = bestNow;
        }
        
    }
    
    return bestOfAll;
}

Solution runHeuristic(int argc, char *argv){
	
	chrono::time_point<std::chrono::system_clock> start, end;
    srand((unsigned) time(NULL));
    Solution result;
    int maxIterILS;
    double sumCost = 0;

    auto data = Data(argc, argv);
    data.read();
    dimension = data.getDimension();

    
    for(int i = 0; i < dimension - 1; i++){

        CLFix.push_back(i + 2);
    }

    if(dimension < 150){
        maxIterILS = dimension;
    }
    else{
        maxIterILS = dimension / 2;
    }

    start = chrono::system_clock::now();    

    result = ILS(50, maxIterILS, data);
    sumCost += result.total; 
	
    end = chrono::system_clock::now();
 
	chrono::duration<double> time = end - start;

    cout << (time.count()) << " " << sumCost << "\n\n";

	return result;
    
}

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

Node bnb(double **cost, int heuristicResult){

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

    double upperBound = heuristicResult;

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
	
	Solution a = runHeuristic(argc, argv[1]);

	chrono::time_point<std::chrono::system_clock> start, end;

	Node result;
	
	Data * data = new Data(argc, argv[1]);
	data->read();
	dimension = data->getDimension();

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
    result = bnb(cost, a.total);
    end = chrono::system_clock::now();    
 
	chrono::duration<double> time = end - start;

    cout << time.count() << " " << result.lowerBound << endl;
    
	for (int i = 0; i < dimension; i++) delete [] cost[i];
	delete [] cost;
	delete data;
	
	return 0;
}