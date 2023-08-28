#include "GC.hpp"
#include <chrono>

std::vector<double> GCMinknap(Instance *binPack, MasterProblem *master, SubProblem *sub){

    master->solve();
    
    IloNumArray *pi; //pointer to dual variables
    std::pair<double, std::vector<bool>* > result;
    std::vector<double> solution;

    while(true){

        pi = master->getDual();

        result = sub->solveMinknap(pi, binPack);

        delete pi;

        if(1 - result.first >= 0 - EPS){

            delete result.second;
            break;
        }
        else{

            master->addCollumn(result.second);
            delete result.second;
        }

        master->solve();
    }

    solution = master->getLambdas();

    return solution;
}

void GC(Instance *binPack, MasterProblem *master, SubProblem *sub, Node *n){

    master->forceLambdas(&n->together, &n->separated);
    sub->changeConstraints(&n->together, &n->separated);
    
    IloNumArray *pi; //pointer to dual variables
    double subResult;
    double masterResult;

    masterResult = master->solve();

    while(true){

        pi = master->getDual();

        sub->changeObjective(pi);

        subResult = sub->solve();

        delete pi;

        if(1 - subResult >= 0 - EPS){

            break;
        }
        else{

            std::vector<bool> *c = sub->getSolution();
            master->addCollumn(c);
            delete c;
        }

        masterResult = master->solve();
    }
    n->solution = master->getLambdas();
    n->value = masterResult;
}