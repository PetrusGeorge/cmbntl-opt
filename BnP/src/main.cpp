#include "GC.hpp"
#include <chrono>
#include <list>
#include <numeric>

bool isFeasible(std::vector<double> *solution){

    for(int i = 0; i < solution->size(); i++){
        if(not ((*solution)[i] - 1 >= 0 - EPS || (*solution)[i] + 1 <= 1 + EPS)){
 
            return false;
        }
    }

    return true;
}

double BnB(Instance *binPack){
    
    MasterProblem *master = new MasterProblem(binPack);
    SubProblem *sub = new SubProblem(binPack);
    std::list<Node*> tree;

    Node *root = new Node;
    double UB;

    root->solution = GCMinknap(binPack, master, sub);
    UB = root->value = std::accumulate(root->solution.begin(), root->solution.end(), 0.0);//Solving Root Node
    root->feasible = isFeasible(&root->solution);

    if(root->feasible){
        delete master;
        delete sub;
        delete root;
        
        return UB;
    }

    Node *newNodeTogether = new Node(root);
    Node *newNodeSeparated = new Node(root);
    std::pair<int,int> fractionedPair = master->getMostFractional();

    tree.push_back(newNodeTogether);
    tree.back()->together.push_back(fractionedPair);//Setting first node;
   
    tree.push_back(newNodeSeparated);
    tree.back()->separated.push_back(fractionedPair);//Setting second node;

    delete root;

    while(!tree.empty()){

        Node *a = tree.back();
        tree.pop_back();

        GC(binPack, master, sub, a);

        delete a;
    }

    delete master;
    delete sub;
    for(std::list<Node*>::iterator it = tree.begin(); it != tree.end(); it++){

        delete *it;
    }

    return UB;
}

int main(int argc, char **argv){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    Instance *binPack = new Instance(argv[1]);

    BnB(binPack);

    delete binPack;

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}