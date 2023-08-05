#include "SubProblem.hpp"

SubProblem::SubProblem(Instance *instance, IloNumArray *pi){

    IloExpr sumWeight(env);
    IloExpr sumObj(env);

    dimension = instance->getDimension();

    model = IloModel(env);
    solver = IloCplex(model);
    constraints = IloRangeArray(env);
    x = IloBoolVarArray(env, dimension);


    for(int i = 0; i < dimension; i++){

        char varName[50];
        sprintf(varName, "X_%d", i + 1);

        sumWeight += instance->getWeight(i) * x[i];
        sumObj += (*pi)[i] * x[i];
    }

    constraints.add(sumWeight <= instance->getCapacity());
    obj = IloMaximize(env, sumObj);

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

std::vector<bool>* SubProblem::getSolution(){

    std::vector<bool> *solution = new std::vector<bool>(dimension);

    IloNumArray values(env, dimension);

    solver.getValues(values, x);

    for(int i = 0; i < dimension; i++){
        (*solution)[i] = values[i];
    }

    return solution;
}