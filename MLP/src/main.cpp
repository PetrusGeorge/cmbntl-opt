#include "Data.h"
#include "Structures.h"
#include "ILS.h"

#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>

using namespace std;

int main(int argc, char** argv){
 
	chrono::time_point<std::chrono::system_clock> start, end;
    srand((unsigned) time(NULL));

    auto data = Data(argc, argv[1]);
    data.read();
    int dimension = data.getDimension();

    Solution result;
    double sumCost = 0;

    start = chrono::system_clock::now();

    for(int i = 0; i < 10; i++){

        result = ILS(10, min(100, dimension), data);
        sumCost += result.latency; 
    }

    end = chrono::system_clock::now();
 
	chrono::duration<double> time = end - start;

    sumCost /= 10;

    cout << (time.count())/10 << " " << sumCost << "\n\n";
    
    return 0;
}
