#include "MasterProblem.hpp"
#include "SubProblem.hpp"

int main(int argc, char **argv){


    Instance *binPack = new Instance(argv[1]);       
    MasterProblem *master = new MasterProblem(binPack->getDimension());

    std::cout << master->solve() << std::endl;

    SubProblem *sub = new SubProblem(binPack, master->getDual());

    std::cout << sub->solve() << std::endl;

    delete binPack;
    delete master;

    return 0;
}