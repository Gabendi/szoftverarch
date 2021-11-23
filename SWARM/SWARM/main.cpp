#include <iostream>
#include "Simulator.h"


int main(int argc, char* argv[]) {
    std::string scriptPath = "input.txt";
    Simulator sim(scriptPath);
    sim.run();

    return 0;
}