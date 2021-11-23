#pragma once
#include "IOHandler.h"
#include "Simulation.h"
#include "SimulationFactory.h"
#include <iostream>
#include <memory>

class Simulator{
private:
	std::shared_ptr<Simulation> sim;
	IOHandler handler;
public:
	Simulator(std::string scriptPath) {
		auto simData = handler.readInitFile(scriptPath);
		SimulationFactory simFactory;
		sim = simFactory.makeSimulation(simData.first, simData.second);
	}

	void run() {
		while (!handler.getSimulationShouldExit()) {
			if (handler.getSimulationShouldPause()) {
				print();
				while (!handler.getSimulationShouldContinue()) {
					if (handler.getSimulationShouldIterate()) {
						sim->lcm();
						print();
					}
				}
			}
			sim->lcm();
		}
	}

	void print() {
		auto map = sim->getMap();
		for (auto& node : map) {
			node->getId();
		}
		std::cout << "asd";
	}
};

