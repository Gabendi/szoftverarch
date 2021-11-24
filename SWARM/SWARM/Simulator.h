#pragma once
#include "IOHandler.h"
#include "Simulation.h"
#include "SimulationFactory.h"
#include "ScriptReaderBase.h"
#include <iostream>
#include <memory>

using namespace Swarm;

class Simulator{
private:
	std::shared_ptr<Simulation> sim;
	UI::IOHandler handler;
	int currIterNum = 0;
public:
	Simulator(std::string scriptPath, Reader::ScriptReaderBase& reader) {
		auto simData = reader.readInitFile(scriptPath);
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
						currIterNum++;
						print();
					}
				}
			}
			sim->lcm();
			currIterNum++;
			print();
		}
	}

	void print() {
		std::cout << "Current iteration: " << currIterNum;
		auto map = sim->getMap();
		for (auto& node : map) {
			int nodeId = node->getId();
			auto entity = node->getEntity();
			int entityId = -1;
			if (entity != nullptr) {
				entityId = entity->getId();
			}
			std::string output = std::to_string(nodeId) + ": " + (entityId == -1 ? "X" : std::to_string(entityId));
			std::cout << output << std::endl;
		}
		std::cout << std::endl;
	}
};

