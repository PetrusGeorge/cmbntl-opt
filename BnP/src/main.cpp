#include "MasterProblem.hpp"
#include "SubProblem.hpp"

#define EPS 0.00001

int main(int argc, char **argv){

    Instance *binPack = new Instance(argv[1]);
    MasterProblem *master = new MasterProblem(binPack);

    std::cout << master->solve() << std::endl;

    while(true){
        IloNumArray *pi = master->getDual();

        SubProblem *sub = new SubProblem(binPack, pi);

        delete pi;

        if(sub->solve() >= 0 - EPS){

            delete sub;
            break;
        }
        else{

            std::vector<bool> *c = sub->getSolution();
            master->addCollumn(c);
            delete sub;
            delete c;
        }
        std::cout << master->solve() << std::endl;
    }

    delete binPack;
    delete master;

    return 0;
}