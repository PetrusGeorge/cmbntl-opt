#include <ilcplex/ilocplex.h>
#include <vector>

#define INF 1000000000

class MasterProblem {

    IloEnv env;
    IloModel model;
    IloCplex solver;
    IloObjective obj;
    IloRangeArray constraints;
    IloNumVarArray lambda;

    std::vector<std::vector<bool>> a;
    int dimension;

    public:
        MasterProblem(const int dimension);    
        double solve();
        void addCollumn(const std::vector<bool> * const c);
};