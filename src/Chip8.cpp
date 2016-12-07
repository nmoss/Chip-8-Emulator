#include <iostream>
#include "Memory.h"
#include "Interpreter.h"

int main(int argc, char** argv){

    std::cout << argv[1] << std::endl;
    Memory mem(argv[1]);
    Memory *m = &mem;
    Interpreter p(m);

    for(int i = 0; i < 10; i++){
        p.execute();
    }

    return 0;
}
