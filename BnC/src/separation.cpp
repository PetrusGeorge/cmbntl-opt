#include "separation.h"

int chooseBestMaxBack(vector<int> s, int n, double **x){

    int bestNode = -1;
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

    return bestNode;
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

    for(int i  = 0; i < n; i++){

        int choosedNode = i;
    }

} 

vector<vector<int>> MinCut(double **x, int n){

}