#ifndef MASTERPROBLEM_HPP
#define MASTERPROBLEM_HPP

#include <ilcplex/ilocplex.h>
#include "Instance.hpp"
#include <vector>
#include <algorithm>

#define INF 1000000000

class MasterProblem {

    IloEnv env;
    IloModel model;
    IloCplex solver;
    IloObjective obj;
    IloRangeArray constraints;
    IloNumVarArray lambda;

    int dimension;
    std::vector<std::vector<bool>> arrangements;

    public:
        
        MasterProblem(Instance *instance);
        ~MasterProblem();
        double solve();
        void addCollumn(std::vector<bool> *c);
        IloNumArray* getDual();
        std::vector<double>* getLambdas();
        std::vector<int> *getBannedLambdas(std::vector<std::pair<int, int>> *together, std::vector<std::pair<int, int>> *separated);
        void forceLambdas(std::vector<std::pair<int, int>> *together, std::vector<std::pair<int, int>> *separated);
        std::pair<int, int> getMostFractional();
        
};

#endif