#include "SubProblem.hpp"
#include "minknap.c"

SubProblem::SubProblem(Instance *instance){

    IloExpr sumWeight(env);

    dimension = instance->getDimension();

    model = IloModel(env);
    solver = IloCplex(model);
    constraints = IloRangeArray(env);
    variableConstraints = IloRangeArray(env);
    x = IloBoolVarArray(env, dimension);

    for(int i = 0; i < dimension; i++){

        char varName[50];
        sprintf(varName, "X_%d", i + 1);
        x[i].setName(varName);

        sumWeight += instance->getWeight(i) * x[i];
    }

    constraints.add(sumWeight <= instance->getCapacity());
    obj = IloMaximize(env);

    model.add(obj);
    model.add(constraints);
    model.add(variableConstraints);
}

SubProblem::~SubProblem(){

    model.end();
    solver.end();
    constraints.end();
    variableConstraints.end();
    x.end();
    obj.end();
    env.end();
}

double SubProblem::solve(){

    //solver.setOut(env.getNullStream());
    solver.solve();

    return solver.getObjValue();
}

std::pair<double, std::vector<bool>* > SubProblem::solveMinknap(IloNumArray *pi, Instance *instance){

    long int *p = new long int[dimension];

    for(int i = 0; i < dimension; i++){
        
        p[i] = (*pi)[i] * BIG_M;
    }

    int *w = new int[dimension];

    for(int i = 0; i < dimension; i++){

        w[i] = instance->getWeight(i);
    }

    int *x = new int[dimension];

    double result = minknap(dimension, p, w, x, instance->getCapacity()) / BIG_M;
    std::vector<bool> *solution = new std::vector<bool>(dimension);

    for(int i = 0; i < dimension; i++){
        (*solution)[i] = x[i];
    }

    delete [] p;
    delete [] x;
    delete [] w;

    return std::make_pair(result, solution);
}

void SubProblem::changeObjective(IloNumArray *pi){

    IloExpr sumObj(env);

    for(int i = 0; i < dimension; i++){

        sumObj += (*pi)[i] * x[i];
    }

    obj.setExpr(sumObj);
}

void SubProblem::changeConstraints(std::vector<std::pair<int, int>> *together, std::vector<std::pair<int, int>> *separated){

    model.remove(variableConstraints);
    variableConstraints.end();//Remove old constraints

    variableConstraints = IloRangeArray(env);
    IloExpr force(env);

    if(separated != NULL){
        for(int i = 0; i < separated->size(); i++){

            force = x[(*separated)[i].first] + x[(*separated)[i].second];
            variableConstraints.add(force <= 1); //x_i + x_j <= 1 forcing x_i and x_j be 1 at the same time
        }
    }
    if(together != NULL){
        for(int i = 0; i < together->size(); i++){

            force = x[(*together)[i].first] - x[(*together)[i].second];
            variableConstraints.add(force == 0); //x_i - x_j == 0 forcing x_i == x_j
        }
    }

    model.add(variableConstraints);
}

std::vector<bool>* SubProblem::getSolution(){

    std::vector<bool> *solution = new std::vector<bool>(dimension);

    IloNumArray values(env, dimension);

    solver.getValues(values, x);

    for(int i = 0; i < dimension; i++){
        (*solution)[i] = values[i];
    }

    return solution;
}