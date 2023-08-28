#include "GC.hpp"
#include <chrono>

typedef struct{

    std::vector<std::pair<int, int>> together;
    std::vector<std::pair<int, int>> separated;
    bool feasible;
    double value;
}Node;

int main(int argc, char **argv){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    Instance *binPack = new Instance(argv[1]);
    MasterProblem *master = new MasterProblem(binPack);
    SubProblem *sub = new SubProblem(binPack);
    std::vector<double> *solution; 

    solution = GCMinknap(binPack, master);

    for(int i = 0; i < solution->size(); i++){

        std::cout << (*solution)[i] << " ";
    }std::cout << std::endl;

    delete sub;
    delete master;
    delete solution;
    delete binPack;

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}