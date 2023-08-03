#include "instance.hpp"

Instance::Instance(char *argv){

    std::ifstream file(argv);

    if(!file){

        std::cout << "File couldn't be read" << std::endl;
        exit(1);
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

        std::cout << "Item " << pos << " is out of bound";
        exit(2);
    }

    return (*itemsWeight)[pos - 1];
}