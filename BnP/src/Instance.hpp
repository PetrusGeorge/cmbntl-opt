#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

class Instance{

    int dimension;
    int capacity;

    std::vector<int> *itemsWeight;

    public:

        Instance(char *argv);
        ~Instance();
        int getDimension();
        int getCapacity();

        int getWeight(int pos);
};

#endif