#include "Structures.h"

using namespace std;

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

    s->latency = subSequenceMatrix[0][data.getDimension()].C;
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

    s->latency = subSequenceMatrix[0][data.getDimension()].C;
}
