#include "ILS.h"

using namespace std;

int ConstructionNode::currentVertex = 1;
Data* ConstructionNode::data = NULL;

Solution Construction(Data& data, vector<ConstructionNode> cl){

    Solution s = {{1}, 0.0};

    ConstructionNode::currentVertex = 1;

    while(not cl.empty()){

        sort(cl.begin(), cl.end());

        double alpha = ((double) rand() / RAND_MAX) + 0.00001;
        int chose = rand() % ((int) ceil(alpha * cl.size()));

        ConstructionNode::currentVertex = cl[chose].vertex;
        cl.erase(cl.begin() + chose);

        s.sequence.push_back(ConstructionNode::currentVertex);
    }
    s.sequence.push_back(1);

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

void LocalSearch(Solution *s, Data& data, vector<vector<Subsequence>>& subSequenceMatrix){

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

void Pertubation(const Solution *s, Solution *receiver, Data& data){

    vector<int> sequenceCopy = s->sequence;
    int qtyBack, qtyFront;
    int sizeB , sizeA, indexB, indexA;

    if(data.getDimension() > 30){
     
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
    vector<vector<Subsequence>> subSequenceMatrix(data.getDimension() + 1 , vector<Subsequence>(data.getDimension() + 1));

    vector<ConstructionNode> clFix(data.getDimension()-1);

    for(int i = 0; i < data.getDimension() - 1; i++){

        clFix[i].vertex = i + 2; //goes from 2 to n
    }
    ConstructionNode::data = &data;

    for(int i = 0; i < maxIter; i++){
        
        Solution current = Construction(data, clFix);
        UpdateAllSubsequence(&current, subSequenceMatrix, data);
        Solution currentBest = current;
        
        int ls = 0;
        
        while(ls <= maxIterILS){
            LocalSearch(&current, data, subSequenceMatrix);

            if(current.latency < currentBest.latency){
                
                currentBest = current;
                ls = 0;
            }
            
            Pertubation(&currentBest, &current, data);
            UpdateAllSubsequence(&current, subSequenceMatrix, data);
            ls++;
        }
        
        if(currentBest.latency < bestOfAll.latency){
        
            bestOfAll = currentBest;
        }
    }
    
    return bestOfAll;
}
