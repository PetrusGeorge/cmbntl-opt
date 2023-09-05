#include "Data.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <chrono>

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
    double latency;
    
}Solution;

typedef struct Insertion{
    
    int insertedVertex;
    int removedVertex;
    double delta;
    
}Insertion;

typedef struct Subsequence{

    double T, C;
    int W;
    int first, last;

    inline static Subsequence Concatenate(Subsequence& sigma1, Subsequence& sigma2, Data& data){

        Subsequence sigma;
        double temp = data.getDistance(sigma1.last, sigma2.first);
        sigma.W  = sigma1.W + sigma2.W;
        sigma.T = sigma1.T + temp + sigma2.T;
        sigma.C = sigma1.C + sigma2.W * (sigma1.T + temp) + sigma2.C;
        sigma.first = sigma1.first;
        sigma.last = sigma2.last;
        
        return sigma;
    }

}Subsequence;

void UpdateAllSubsequence(Solution *s, vector<vector<Subsequence>>& subSequenceMatrix, Data& data){

    int n = s->sequence.size();

    for(int i = 0; i < n; i++){

        subSequenceMatrix[i][i].W = (i > 0);
        subSequenceMatrix[i][i].C = 0;
        subSequenceMatrix[i][i].T = 0;
        subSequenceMatrix[i][i].first = s->sequence[i];
        subSequenceMatrix[i][i].last = s->sequence[i];
    }
    
    for(int i = 0; i < n; i++){

        for(int j = i + 1; j < n; j++){
  

            subSequenceMatrix[i][j] = Subsequence::Concatenate(subSequenceMatrix[i][j - 1], subSequenceMatrix[j][j], data);
        }
    }

    for(int i = n - 1; i >= 0; i--){

        for(int j = i - 1; j >= 0; j--){


            subSequenceMatrix[i][j] = Subsequence::Concatenate(subSequenceMatrix[i][j + 1], subSequenceMatrix[j][j], data);
        }
    }

    s->latency = subSequenceMatrix[0][dimension].C;
}

void UpdateMoveSubsequence(Solution *s, vector<vector<Subsequence>>& subSequenceMatrix, Data& data, int start, int end){

    int n = s->sequence.size();

    for(int i = start; i <= end; i++){

        subSequenceMatrix[i][i].first = s->sequence[i];
        subSequenceMatrix[i][i].last = s->sequence[i];

    }

    for(int i = 0; i <= end; i++){

        for(int j = start; j < n; j++){
            
            if(i >= j){

                continue;
            }

            subSequenceMatrix[i][j] = Subsequence::Concatenate(subSequenceMatrix[i][j - 1], subSequenceMatrix[j][j], data);
        }
    }
    
    for(int i = n - 1; i >= start; i--){

        for(int j = end; j >= 0; j--){

            if(i <= j){

                continue;
            }

            subSequenceMatrix[i][j] = Subsequence::Concatenate(subSequenceMatrix[i][j + 1], subSequenceMatrix[j][j], data);
        }
    }

    s->latency = subSequenceMatrix[0][dimension].C;
}

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

bool bestSwap(Solution *s, Data& data, vector<vector<Subsequence>>& subSequenceMatrix){

    int bestA, bestB;
    int size = s->sequence.size();
    double bestLatency = s->latency;

    Subsequence sigma1, sigma2, sigma3, sigma4;

    for(int i = 1; i < size - 1; i++){
       
        for(int j = i + 1; j < size - 1; j++){
            
           if(i != j - 1){
                
                sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][i - 1], subSequenceMatrix[j][j], data);
                sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[i + 1][j - 1], data);
                sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[i][i], data);
                sigma4 = Subsequence::Concatenate(sigma3, subSequenceMatrix[j + 1][size - 1], data);   
            }

            else{

                sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][i - 1], subSequenceMatrix[j][j], data);
                sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[i][i], data);
                sigma4 = Subsequence::Concatenate(sigma2, subSequenceMatrix[j + 1][size - 1], data);
            }

            if(sigma4.C < bestLatency){

                bestLatency = sigma4.C;
                bestA = i;
                bestB = j;
            }
        }
    }

    if(bestLatency < s->latency){

        swap(s->sequence[bestA], s->sequence[bestB]);

        UpdateMoveSubsequence(s, subSequenceMatrix, data, bestA, bestB);

        return true;
    }

    return false;
}

bool bestOrOPT(Solution *s, int choice, Data& data, vector<vector<Subsequence>>& subSequenceMatrix){

    int bestA, bestB;
    int size = s->sequence.size();
    double bestLatency = s->latency;

    Subsequence sigma1, sigma2, sigma3;

    if(choice == 1){
        
        for(int i = 1; i < size - 2; i++){
          
            for(int j = 0; j < size - 1; j++){
                
                if(i - j <= 1 && i - j >= 0){ 
                  
                    continue;  
                }

                if(i > j){

                    sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][j], subSequenceMatrix[i][i], data);
                    sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[j + 1][i - 1], data);
                    sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[i + 1][size - 1], data);
                }
                
                else{

                    sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][i - 1], subSequenceMatrix[i + 1][j], data);
                    sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[i][i], data);
                    sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[j + 1][size - 1], data);
                    
                }

                if(sigma3.C < bestLatency){

                    bestLatency = sigma3.C;
                    bestA = i;
                    bestB = j;
                }
            }
        }
        

        if(bestLatency < s->latency){

            if(bestA > bestB){
       
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.erase(s->sequence.begin() + bestA + 1);

                UpdateMoveSubsequence(s, subSequenceMatrix, data, bestB, bestA);
            }

            else{

                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.erase(s->sequence.begin() + bestA);
                
                UpdateMoveSubsequence(s, subSequenceMatrix, data, bestA, bestB);
            }

            return true;
        }

    }
    

    else if(choice == 2){

        for(int i = 1; i < size - 3; i++){

            for(int j = 0; j < size - 1; j++){

                if(i - j <= 1 && i - j >= -1){ 
                  
                    continue;  
                }

                if(i > j){

                    sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][j], subSequenceMatrix[i + 1][i], data);
                    sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[j + 1][i - 1], data);
                    sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[i + 2][size - 1], data);
                }

                else{

                    sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][i - 1], subSequenceMatrix[i + 2][j], data);
                    sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[i + 1][i], data);
                    sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[j + 1][size - 1], data);
                }

                if(sigma3.C < bestLatency){

                    bestLatency = sigma3.C;
                    bestA = i;
                    bestB = j;
                }
            }
        }

        if(bestLatency < s->latency){
            
            if(bestA > bestB){
             
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.erase(s->sequence.begin() + bestA + 2);
                s->sequence.erase(s->sequence.begin() + bestA + 2);

                UpdateMoveSubsequence(s, subSequenceMatrix, data, bestB, bestA + 1);
            }
            
            else{

                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 1]);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
                
                UpdateMoveSubsequence(s, subSequenceMatrix, data, bestA, bestB + 1);
            }


            return true;
        }

    }

    else if(choice == 3){

        for(int i = 1; i < size - 4; i++){

            for(int j = 0; j < size - 1; j++){
                
                if(i - j <= 2 && i - j >= -2){ 
                  
                    continue;  
                }

                if(i > j){

                    sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][j], subSequenceMatrix[i + 2][i], data);
                    sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[j + 1][i - 1], data);
                    sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[i + 3][size - 1], data);
                }

                else{
                    
                    sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][i - 1], subSequenceMatrix[i + 3][j], data);
                    sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[i + 2][i], data);
                    sigma3 = Subsequence::Concatenate(sigma2, subSequenceMatrix[j + 1][size - 1], data);
                }

                if(sigma3.C < bestLatency){

                    bestLatency = sigma3.C;
                    bestA = i;
                    bestB = j;
                }
            }
        }

        if(bestLatency < s->latency){

            if(bestA > bestB){
                
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 4]);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                
                UpdateMoveSubsequence(s, subSequenceMatrix, data, bestB, bestA + 2);
            }

            else{

                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 1]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);    

                UpdateMoveSubsequence(s, subSequenceMatrix, data, bestA - 1, bestB + 1);
            }

            return true;
        }

    }

    return false;
}

bool best2Opt(Solution *s, Data& data, vector<vector<Subsequence>>& subSequenceMatrix){

    int bestA, bestB;
    int size = s->sequence.size();
    double bestCost = s->latency;
   
    Subsequence sigma1, sigma2;

    for(int i = 1; i < size - 1; i++){

        for(int j = i + 2; j < size - 1; j++){

            sigma1 = Subsequence::Concatenate(subSequenceMatrix[0][i - 1], subSequenceMatrix[j - 1][i], data);
            sigma2 = Subsequence::Concatenate(sigma1, subSequenceMatrix[j][size - 1], data);

            if(sigma2.C < bestCost){
                
                bestCost = sigma2.C;
                bestA = i;
                bestB = j;
            }
        }
    }

    if(bestCost < s->latency){
            
        int qnt =  (bestB - bestA) / 2;

        for(int i = 0; i < qnt; i++){

            swap(s->sequence[bestB - 1 - i], s->sequence[bestA + i]);
        }
        
        UpdateMoveSubsequence(s, subSequenceMatrix, data, bestA, bestB);
        
        return true;
    }

    return false;    
}

void localSearch(Solution *s, Data& data, vector<vector<Subsequence>>& subSequenceMatrix){

    vector<int> options {1, 2, 3, 4, 5};
    bool better = false;
    
    while(options.empty() == false){

        int n = rand() % options.size();
        
        switch (options[n]){

        case 1:
            better = bestSwap(s, data, subSequenceMatrix);
            break;
        
        case 2:
            better = best2Opt(s, data, subSequenceMatrix);
            break;

        case 3:
            better = bestOrOPT(s, 1, data, subSequenceMatrix);
            break;
        
        case 4:
            better = bestOrOPT(s, 2, data, subSequenceMatrix);
            break;

        case 5:
            better = bestOrOPT(s, 3, data, subSequenceMatrix);
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

void pertubation(const Solution *s, Solution *receiver, Data& data){

    vector<int> sequenceCopy = s->sequence;
    int qtyBack, qtyFront;
    int sizeB , sizeA, indexB, indexA;

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
    
    receiver->sequence = sequenceCopy;
}

Solution ILS(int maxIter, int maxIterILS, Data& data){
    
    Solution bestOfAll;
    bestOfAll.latency = INFINITY;
    vector<vector<Subsequence>> subSequenceMatrix(dimension + 1 , vector<Subsequence>(dimension + 1));

    for(int i = 0; i < maxIter; i++){
        
        Solution current = construction(data);
        UpdateAllSubsequence(&current, subSequenceMatrix, data);
        Solution currentBest = current;
        
        int ls = 0;
        
        while(ls <= maxIterILS){
            localSearch(&current, data, subSequenceMatrix);

            if(current.latency < currentBest.latency){
                
                currentBest = current;
                ls = 0;
            }
            
            pertubation(&currentBest, &current, data);
            UpdateAllSubsequence(&current, subSequenceMatrix, data);
            ls++;
        }
        
        if(currentBest.latency < bestOfAll.latency){
        
            bestOfAll = currentBest;
        }
        
    }
    
    return bestOfAll;
}

int main(int argc, char** argv){
 
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

    if(dimension < 100){
        maxIterILS = dimension;
    }
    else{
        maxIterILS = 100;
    }

    start = chrono::system_clock::now();
   
    for(int i = 0; i < 10; i++){

        vector<vector<Subsequence>> subSequenceMatrix(dimension + 1, vector<Subsequence>(dimension + 1));
        result = ILS(10, maxIterILS, data);
        UpdateAllSubsequence(&result, subSequenceMatrix, data);
        sumCost += result.latency; 
    }

    end = chrono::system_clock::now();
 
	chrono::duration<double> time = end - start;

    sumCost /= 10;

    cout << (time.count())/10 << " " << sumCost << "\n\n";
    
    return 0;
}
