#include "SubProblem.hpp"
#include "minknap.c"

SubProblem::SubProblem(Instance *instance){

    IloExpr sumWeight(env);

    dimension = instance->getDimension();

    model = IloModel(env);
    solver = IloCplex(model);
    constraints = IloRangeArray(env);
    x = IloBoolVarArray(env, dimension);

    for(int i = 0; i < dimension; i++){

        char varName[50];
        sprintf(varName, "X_%d", i + 1);

        sumWeight += instance->getWeight(i) * x[i];
    }

    constraints.add(sumWeight <= instance->getCapacity());
    obj = IloMaximize(env);

    model.add(obj);
    model.add(constraints);
}

SubProblem::~SubProblem(){

    model.end();
    solver.end();
    constraints.end();
    x.end();
    obj.end();
    env.end();
}

double SubProblem::solve(){

    solver.setOut(env.getNullStream());
    solver.solve();

    return 1 - solver.getObjValue();
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

void SubProblem::addEqualityConstraint(int index1, int index2){

    IloExpr sum(env);

    sum += x[index1] + x[index2];

    constraints.add(sum <= 1);
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