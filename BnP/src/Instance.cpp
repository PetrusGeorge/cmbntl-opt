#include "Instance.hpp"

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

        std::cout << "Instance wasn't read properly" << std::endl;
        exit(2);
    }
}

int Instance::getCapacity(){

    return this->capacity;
}

int Instance::getDimension(){

    return this->dimension;
}

int Instance::getWeight(int pos){

    if(pos >= dimension or pos < 0){

        std::cout << "Item " << pos << " is out of bound";
        exit(3);
    }

    return (*itemsWeight)[pos];
}