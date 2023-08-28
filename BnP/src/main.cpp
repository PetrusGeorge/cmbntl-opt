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
    double UB = INF;

    root->solution = GCMinknap(binPack, master, sub);
    root->value = std::accumulate(root->solution.begin(), root->solution.end(), 0.0);//Solving Root Node
    root->feasible = isFeasible(&root->solution);

    if(root->feasible){
        delete master;
        delete sub;
        delete root;

        return root->value;
    }

    Node *newNodeTogether = new Node(root);
    Node *newNodeSeparated = new Node(root);
    std::pair<int,int> fractionedPair = master->getMostFractional();

    newNodeTogether->together.push_back(fractionedPair);//Setting first node;
    newNodeSeparated->separated.push_back(fractionedPair);//Setting second node;

    tree.push_back(newNodeTogether);
    tree.push_back(newNodeSeparated);

    delete root;

    while(!tree.empty()){

        Node *n = tree.back();
        tree.pop_back();

        GC(binPack, master, sub, n);
        
        if(n->feasible = isFeasible(&n->solution)){

            std::cout << "Entered:" << std::endl;

            if(n->value <= UB){

                UB = n->value;
            }
        }
        else{
                
            newNodeTogether = new Node(n);
            newNodeSeparated = new Node(n);
            fractionedPair = master->getMostFractional();
    
            newNodeTogether->together.push_back(fractionedPair);//Setting first node;
            newNodeSeparated->separated.push_back(fractionedPair);//Setting second node;

            tree.push_back(newNodeTogether);
            tree.push_back(newNodeSeparated);
        }

        delete n;
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

    std::cout << BnB(binPack) << std::endl;

    delete binPack;

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}