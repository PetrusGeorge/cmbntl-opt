#include "Data.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

#define BACK 0
#define FRONT 1

int dimension;
//Candidate vertex for random choices
vector<int> CLFix;

auto nThreads = thread::hardware_concurrency();

typedef struct Solution{
    
    vector<int> sequence;
    double total;
    
}Solution;

typedef struct Insertion{
    
    int insertedVertex;
    int removedVertex;
    double delta;
    
}Insertion;

Solution chooseRandom(Data& data, vector<int> CL){

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

vector<Insertion> calculateInsertionCost(const Solution *subTour, Data& data, vector<int> CL){
    
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

void insertRandom(Solution *incompleteSolution, vector<Insertion> list, vector<int> CL){
    
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

void construction(Data& data, Solution *receiver){
 
    //reset candidate vertices
    vector<int> CL = CLFix;
    Solution s;
    
    s = chooseRandom(data, CL);

    while(!CL.empty()){
        
        vector<Insertion> insertionCost = calculateInsertionCost(&s, data, CL);

        sort(insertionCost.begin(), insertionCost.end(), compare);

        insertRandom(&s, insertionCost, CL);
    }
    
    *receiver = s;
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

void joinAllThreads(thread t[]){

    for(int i = 0; i < nThreads; i++){

        t[i].join();
    }
}

Solution ILS(int maxIter, int maxIterILS, Data& data){
    
    Solution bestOfAll;
    bestOfAll.total = INFINITY;
    
    for(int i = 0; i < maxIter; i++){

        thread t[nThreads];        
        Solution now[nThreads];

        for(int i = 0; i < nThreads; i++){

            t[i] = thread(construction, ref(data), &now[i]);
        }

        joinAllThreads(t);

        Solution bestNow = now[0];
        
        int ls = 0;
        
        while(ls <= maxIterILS){

            for(int i = 0; i < nThreads; i++){
                t[i] = thread(localSearch, &now[i], ref(data));
            }

            joinAllThreads(t);

            for(int i = 0; i < nThreads; i++){
             
                if(now[i].total < bestNow.total){
                    
                    bestNow = now[i];
                    ls = 0;
                }
            }

            for(int i = 0; i < nThreads; i++){
             
                t[i] = thread(Pertubacao, &bestNow, &now[i], ref(data));
            }

            joinAllThreads(t);

            ls++;
        }
        
        if(bestNow.total < bestOfAll.total){
        
            bestOfAll = bestNow;
        }
        
    }
    
    return bestOfAll;
}

int main(int argc, char** argv) {
 
    chrono::time_point<std::chrono::system_clock> start, end;
    srand((unsigned) time(NULL));
    Solution result;
    int maxIterILS;
    double sumCost = 0;

    auto data = Data(argc, argv[1]);
    data.read();
    dimension = data.getDimension();

    
    for(int i = 0; i < dimension - 1; i++){

        CLFix.push_back(i + 2);
    }

    if(dimension < 150){
        maxIterILS = dimension/nThreads;
    }
    else{
        maxIterILS = dimension / (2*nThreads);
    }

    start = chrono::system_clock::now();
   
    for(int i = 0; i < 1; i++){

        result = ILS(50, maxIterILS, data);
        sumCost += result.total; 
    }
	
    end = chrono::system_clock::now();
 
	chrono::duration<double> time = end - start;

    cout << (time.count())/1 << " " << sumCost/1 << "\n\n";
    
    return 0;
}
