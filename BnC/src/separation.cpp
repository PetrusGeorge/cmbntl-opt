#include "separation.h"

pair<int,double> chooseBestMaxBack(vector<int> s, int n, double **x){

    int bestNode;
    double bestMaxBack = -1;//Ensure that it's lower than first maxBack

    for(int i = 0; i < n; i++){

        double maxBack = 0;

        if(find(s.begin(), s.end(), i) != s.end()){
          
            continue;
        }

        for(int j = 0; j < s.size(); j++){

            if(s[j] < i){

                maxBack += x[s[j]][i];
            }

            else{

                maxBack += x[i][s[j]];
            }
        }

        if(maxBack > bestMaxBack){

            bestMaxBack = maxBack;
            bestNode = i;
        }
    }

    return make_pair(bestNode, bestMaxBack);
}

double calculateCutMin(int node, int n, double **x){

    double value = 0;

    for(int i = 0; i < n; i++){

        if(i < node){

            value += x[i][node];
        }

        else if(i == node){

            continue;
        }

        else{

            value += x[node][i];
        }
    }

    return value;
}

vector<vector<int>> MaxBack(double **x, int n){

    vector<vector<int>> result;
    bool cont;

    for(int i = 0; i < n; i++){

        cont = false;

        for(int j = 0; j < result.size(); j++){

            if(find(result[j].begin(), result[j].end(), i) != result[j].end()){

                cont = true;
                continue;
            }
        }

        if(cont){

            continue;
        }

        vector<int> s = {i};
        vector<int> sMin;
        double cutMin = calculateCutMin(s[0], n, x);
        double cutVal = cutMin;

        while(s.size() < n){

            pair<int,double> bestNode = chooseBestMaxBack(s, n, x);

            s.push_back(bestNode.first);
            cutVal += 2 + - (bestNode.second * 2);

            if(cutVal < cutMin){

                cutMin = cutVal;
                sMin = s;
            }
        }

        if(sMin.size() != n){
            result.push_back(sMin);
        }
    }

    return result;
} 

vector<vector<int>> MinCut(double **x, int n){

}