#include "instance.hpp"

Instance::Instance(char *argv){

    std::ifstream file(argv);

    if(!file){

        std::string e = "File couldn't be read";
        throw(e);
    }

    file >> dimension;
    file >> capacity;

    itemsWeight = new std::vector<int> (dimension);

    int read;

    for(int i = 0; i < dimension; i++){

        file >> read;

        (*itemsWeight)[i] = read;
    }

    file >> read;//skip line

    if(!file.eof()){

        std::string e = "Instance wasn't read properly";
        throw (e);
    }
}

int Instance::getCapacity(){

    return this->capacity;
}

int Instance::getDimension(){

    return this->dimension;
}

int Instance::getWeight(int pos){

    if(pos > dimension or pos < 1){

        std::string e = "Item " + std::to_string(pos) + " is out of bound";
        throw(e);
    }

    return (*itemsWeight)[pos - 1];
}