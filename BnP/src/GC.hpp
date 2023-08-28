#ifndef GC_HPP
#define GC_HPP

#define EPS 0.00001

#include <vector>

#include "Instance.hpp"
#include "MasterProblem.hpp"
#include "SubProblem.hpp"

typedef struct Node{

    std::vector<std::pair<int, int>> together;
    std::vector<std::pair<int, int>> separated;
    std::vector<double> solution;
    bool feasible;
    double value;

    Node(){}

    Node(Node *father){
        together = father->together;
        separated = father->separated;
        solution = father->solution;
        feasible = father->feasible;
        value = father->value;
    }
}Node;

std::vector<double> GCMinknap(Instance *instance, MasterProblem *master, SubProblem *sub);
void GC(Instance *instance, MasterProblem *master, SubProblem *sub, Node *n);

#endif