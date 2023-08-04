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

    constraints.add(sumWeight <= dimension);
    obj = IloMaximize(env, sumWeight);

    model.add(obj);
    model.add(constraints);
}

double SubProblem::solve(){

    static int number = 0;
    solver.setOut(env.getNullStream());
    solver.solve();

    std::string fileName = "sub" + std::to_string(number) + ".lp";
    number++;

    solver.exportModel(fileName.c_str());

    return 1 - solver.getObjValue();
}

std::vector<double> SubProblem::getSolution(){

    std::vector<double> solution(dimension);

}