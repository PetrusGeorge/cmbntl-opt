#ifndef ILS_H
#define ILS_H

#include "Data.h"
#include "Structures.h"
#include <vector>
#include <algorithm>

#define BACK 0
#define FRONT 1

typedef struct ConstructionNode{
    
    int vertex;
    static Data* data;
    static int currentVertex;

    inline bool operator<(const ConstructionNode& right){

        return data->getDistance(currentVertex, this->vertex) < data->getDistance(currentVertex, right.vertex);
    }
}ConstructionNode;

Solution Construction(Data& data, std::vector<ConstructionNode> cl);
bool bestSwap(Solution *s, Data& data, std::vector<std::vector<Subsequence>>& subSequenceMatrix);
bool bestOrOPT(Solution *s, int choice, Data& data, std::vector<std::vector<Subsequence>>& subSequenceMatrix);
bool best2Opt(Solution *s, Data& data, std::vector<std::vector<Subsequence>>& subSequenceMatrix);
void LocalSearch(Solution *s, Data& data, std::vector<std::vector<Subsequence>>& subSequenceMatrix);
void Pertubation(const Solution *s, Solution *receiver, Data& data);
Solution ILS(int maxIter, int maxIterILS, Data& data);

#endif 
