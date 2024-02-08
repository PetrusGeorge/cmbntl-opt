#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include "Data.h"

typedef struct Solution{
    
    std::vector<int> sequence;
    double latency;
}Solution;

typedef struct Subsequence{

    double T, C;
    int W;
    int first, last;

    inline static Subsequence Concatenate(const Subsequence& sigma1, const Subsequence& sigma2, Data& data){

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

void UpdateAllSubsequence(Solution *s, std::vector<std::vector<Subsequence>>& subSequenceMatrix, Data& data);
void UpdateMoveSubsequence(Solution *s, std::vector<std::vector<Subsequence>>& subSequenceMatrix, Data& data, int start, int end);

#endif
