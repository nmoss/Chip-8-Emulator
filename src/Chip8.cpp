#include <iostream>
#include "Memory.h"

int main(int argc, char** argv){

    std::cout << argv[1] << std::endl;
    Memory mem(argv[1]);

    return 0;
}
