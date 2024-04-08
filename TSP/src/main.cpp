#include "Data.h"
#include <iostream>
#include <time.h>
#include <chrono>

#include "Construction.hpp"
#include "ILS.hpp"

using namespace std;

int main(int argc, char** argv) {

    chrono::time_point<std::chrono::system_clock> start, end;
    srand((unsigned) time(NULL));
    Solution result;
    int maxIterILS;
    double sumCost = 0;

    auto data = Data(argc, argv[1]);
    data.read();
    int dimension = data.getDimension();

    if(dimension < 150){
        maxIterILS = dimension;
    }
    else{
        maxIterILS = dimension / 2;
    }

    start = chrono::system_clock::now();

    for(int i = 0; i < 10; i++){

        result = ILS(50, maxIterILS, data);
        sumCost += result.total; 
    }

    end = chrono::system_clock::now();
    chrono::duration<double> time = end - start;
    cout << (time.count())/10 << " " << sumCost/10 << "\n\n";
    
    return 0;
}
