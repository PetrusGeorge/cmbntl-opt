#include "instance.hpp"

int main(int argc, char **argv){

    Instance *test;
    try{

        test = new Instance(argv[1]);        
    }catch(std::string e){

        std::cout << e << std::endl;
    }

    for(int i = 1; true; i++){

        try{
            std::cout << test->getWeight(i) << std::endl;
        }catch(std::string e){

            std::cout << e << std::endl;
            return 1;
        }
    }

    return 0;
}