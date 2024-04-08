#ifndef CONSTRUCTION_HPP
#define CONSTRUCTION_HPP

#include <vector>
#include "Data.h"

typedef struct Solution {
    std::vector<int> sequence;
    double total;
} Solution;

typedef struct Insertion {
    int insertedVertex;
    int removedVertex;
    double delta;
} Insertion;

Solution construction(Data &);
void Pertubacao(const Solution *, Solution *, Data &);

#endif
