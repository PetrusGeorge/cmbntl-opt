#include "GC.hpp"

std::vector<double>* GCMinknap(Instance *binPack, MasterProblem *master){

    std::cout << master->solve() << std::endl;
    
    IloNumArray *pi; //pointer to dual variables
    SubProblem *sub = new SubProblem(binPack);
    std::pair<double, std::vector<bool>* > result;
    std::vector<double> *solution;

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

    delete sub;

    return solution;
}

std::vector<double>* GC(Instance *binPack, MasterProblem *master, SubProblem *sub){

    std::cout << master->solve() << std::endl;
    
    IloNumArray *pi; //pointer to dual variables
    std::vector<double> *solution;
    double result;

    while(true){

        pi = master->getDual();

        sub->changeObjective(pi);

        result = sub->solve();

        delete pi;

        if(1 - result >= 0 - EPS){

            break;
        }
        else{

            std::vector<bool> *c = sub->getSolution();
            master->addCollumn(c);
            delete c;
        }

        std::cout << master->solve() << std::endl;
    }

    solution = master->getLambdas();

    return solution;
}