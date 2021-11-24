#include <iostream>
#include "Simulator.h"
#include "TxtScriptReader.h"

int main(int argc, char* argv[]) {
    TxtScriptReader reader;
    std::string scriptPath = "input.txt";
    Simulator sim(scriptPath, reader);
    sim.run();

    return 0;
}