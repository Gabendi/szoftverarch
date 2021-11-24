#include <iostream>
#include "Simulator.h"
#include "TxtScriptReader.h"

int main(int argc, char* argv[]) {
    
    if (argc == 2) {
        Swarm::Reader::TxtScriptReader reader;
        std::string scriptPath = argv[1];
        Simulator sim(scriptPath, reader);
        sim.run();
    }
    else {
        std::cout << "Example usage: SWARM.exe <path_to_script>";
    }    

    return 0;
}