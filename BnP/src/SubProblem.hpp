#ifndef SUBPROBLEM_HPP
#define SUBPROBLEM_HPP

#include <ilcplex/ilocplex.h>
#include "Instance.hpp"
#include <vector>

class SubProblem{

    IloEnv env;
    IloModel model;
    IloCplex solver;
    IloObjective obj;
    IloRangeArray constraints;
    IloBoolVarArray x;
    
    int dimension;

    public:
        SubProblem(Instance *instance, IloNumArray *pi);
        ~SubProblem();
        double solve();
        std::vector<bool>* getSolution();

};

#endif