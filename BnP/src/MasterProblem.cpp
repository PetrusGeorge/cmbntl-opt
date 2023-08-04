#include "MasterProblem.hpp"

MasterProblem::MasterProblem(int size){
 
    IloExpr sum(env);

    dimension = size;

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

    delete dual;
}

double MasterProblem::solve(){

    //static int number = 0;
    solver.setOut(env.getNullStream());
    solver.solve();

    //std::string fileName = "master" + std::to_string(number) + ".lp";
    //number++;

    //solver.exportModel(fileName.c_str());

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

    delete dual;

    dual = new IloNumArray(env, dimension);

    solver.getDuals(*dual, constraints);

    return dual;
}