#include <iostream>
#include "IOHandler.h"
#include "Simulator.h"

int main(int argc, char* argv[]){
    Simulator simulator("input.txt");
    simulator.run();
}