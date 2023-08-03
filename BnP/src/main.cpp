#include "instance.hpp"
#include "masterProblem.hpp"
#include <ilcplex/ilocplex.h>


int main(int argc, char **argv){

    Instance *test = new Instance(argv[1]);       
    MasterProblem *master = new MasterProblem(test->getDimension());

    std::cout << master->solve() << std::endl;

    std::vector<bool> c = {true, true, false, false, false};

    master->addCollumn(&c);

    std::cout << master->solve() << std::endl;

    return 0;
}