#ifndef MASTERPROBLEM_HPP
#define MASTERPROBLEM_HPP

#include <ilcplex/ilocplex.h>
#include "Instance.hpp"
#include <vector>

#define INF 1000000000

class MasterProblem {

    IloEnv env;
    IloModel model;
    IloCplex solver;
    IloObjective obj;
    IloRangeArray constraints;
    IloNumVarArray lambda;

    int dimension;

    public:
        std::vector<std::vector<bool>> arrangements;
        
        MasterProblem(Instance *instance);
        ~MasterProblem();
        double solve();
        void addCollumn(const std::vector<bool> * const c);
        IloNumArray* getDual();
        std::vector<double>* getLambdas();
        
};

#endif