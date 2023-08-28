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
        sprintf(varName, "λ_%d", i);

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

void MasterProblem::addCollumn(std::vector<bool> *c){

    IloNumColumn collumn = obj(1);

    for(int i = 0; i < dimension; i++){

        collumn += constraints[i](c->at(i));
    }

    arrangements.push_back(*c);

    char varName[50];
    sprintf(varName, "λ_%d", (int)(lambda.getSize()));

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

std::vector<int>* MasterProblem::getBannedLambdas(std::vector<std::pair<int, int>> *together, std::vector<std::pair<int, int>> *separated){

    std::vector<int> *banned = new std::vector<int>();

    if(together != NULL){
        for(int i = 0; i < together->size(); i++){

            for(int j = 0; j < arrangements.size(); j++){

                if(arrangements[j][(*together)[i].first] != arrangements[j][(*together)[i].second]){//ban if they are separated

                    banned->push_back(j);
                }
            }
        }
    }

    if(separated != NULL){
        for(int i = 0; i < separated->size(); i++){

            for(int j = 0; j < arrangements.size(); j++){

                if(arrangements[j][(*separated)[i].first] && arrangements[j][(*separated)[i].second]){//ban if they are together

                    banned->push_back(j);
                }
            }
        }
    }

    std::sort(banned->begin(), banned->end());
    banned->erase(std::unique(banned->begin(), banned->end()), banned->end());

    return banned;
}

void MasterProblem::forceLambdas(std::vector<std::pair<int, int>> *together, std::vector<std::pair<int, int>> *separated){

    std::vector<int> *banned = getBannedLambdas(together, separated);

    for(int i = 0; i < lambda.getSize(); i++){

        if(std::find(banned->begin(), banned->end(), i) != banned->end()){

            lambda[i].setUB(0);
        }

        else{

            lambda[i].setUB(IloInfinity);
        }
    }

    delete banned;
}

std::pair<int, int> MasterProblem::getMostFractional(){

    std::pair<int, int> mostFractional = std::make_pair(-1, -1);
    double mostFractionalValue = INF;

    for(int i = 0; i < dimension; i++){//item i

        for(int j = i + 1; j < dimension; j++){//item j

            double fractionalValue = 0;
            bool fractional = false;

            for(int l = dimension; l < arrangements.size(); l++){//Lambda l

                if(arrangements[l][i] && arrangements[l][j]){

                    fractionalValue += solver.getValue(lambda[l]);
                    fractional = true;
                }
            }

            if(fabs(fractionalValue - 0.5) < mostFractionalValue && fractional){

                mostFractionalValue = fabs(fractionalValue - 0.5);
                mostFractional = std::make_pair(i, j);
            }

        }
    }
    
    return mostFractional;
}