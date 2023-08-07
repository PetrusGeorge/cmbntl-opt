#include "MasterProblem.hpp"
#include "SubProblem.hpp"
#include <chrono>

#define EPS 0.00001

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

        std::cout << master->solve() << std::endl;
    }

    solution = master->getLambdas();

    delete sub;
    delete master;

    return solution;
}

int main(int argc, char **argv){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    Instance *binPack = new Instance(argv[1]);
    MasterProblem *master = new MasterProblem(binPack);
    std::vector<double> *solution; 

    solution = GCMinknap(binPack, master);

    for(int i = 0; i < solution->size(); i++){

        std::cout << (*solution)[i] << " ";
    }std::cout << std::endl;

    delete solution;
    delete binPack;

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}