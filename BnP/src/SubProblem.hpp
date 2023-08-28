#ifndef SUBPROBLEM_HPP
#define SUBPROBLEM_HPP

#define BIG_M 10000000.0

#include <ilcplex/ilocplex.h>
#include "Instance.hpp"
#include <vector>

class SubProblem{

    IloEnv env;
    IloModel model;
    IloCplex solver;
    IloObjective obj;
    IloRangeArray constraints;
    IloRangeArray variableConstraints;
    IloBoolVarArray x;
    
    int dimension;

    public:
        SubProblem(Instance *instance);
        ~SubProblem();
        double solve();
        std::pair<double, std::vector<bool>*> solveMinknap(IloNumArray *pi, Instance *instance);
        void changeObjective(IloNumArray *pi);
        std::vector<bool>* getSolution();
        void changeConstraints(std::vector<std::pair<int, int>> *together, std::vector<std::pair<int, int>> *separated);
};

#endif