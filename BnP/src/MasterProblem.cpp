#include "MasterProblem.hpp"

MasterProblem::MasterProblem(Instance *instance){
 
    IloExpr sum(env);

    dimension = instance->getDimension();

    model = IloModel(env);
    solver = IloCplex(model);
    constraints = IloRangeArray(env);
    lambda = IloNumVarArray(env, dimension, 0, IloInfinity);

    arrangements = std::vector<std::vector<bool>>(dimension, std::vector<bool>(dimension, false));

    for(int i = 0; i < dimension; i++){

        char varName[50];
        sprintf(varName, "λ_%d", i + 1);

        lambda[i].setName(varName);
        sum += lambda[i];
        constraints.add(lambda[i] == 1);
        arrangements[i][i] = true;
    }

    obj = IloMinimize(env, sum);
    
    model.add(obj);

    model.add(constraints);
}

MasterProblem::~MasterProblem(){

    model.end();
    solver.end();
    constraints.end();
    lambda.end();
    obj.end();
    env.end();
}

double MasterProblem::solve(){

    solver.setOut(env.getNullStream());
    solver.solve();

    return solver.getObjValue();
}

void MasterProblem::addCollumn(const std::vector<bool> * const c){

    IloNumColumn collumn = obj(1);

    for(int i = 0; i < dimension; i++){

        collumn += constraints[i](c->at(i));
    }

    arrangements.push_back(*c);

    char varName[50];
    sprintf(varName, "λ_%d", (int)(lambda.getSize() + 1));

    lambda.add(IloNumVar(collumn, 0, IloInfinity));
    lambda[lambda.getSize() - 1].setName(varName);
}

IloNumArray* MasterProblem::getDual(){

    IloNumArray *dual = new IloNumArray(env, dimension);

    solver.getDuals(*dual, constraints);

    return dual;
}

std::vector<double>* MasterProblem::getLambdas(){

    IloNumArray solution(env);

    solver.getValues(solution, lambda);

    std::vector<double> *l = new std::vector<double>(solution.getSize());

    for(int i = 0; i < solution.getSize(); i++){

        (*l)[i] = solution[i];
    }

    return l;
}