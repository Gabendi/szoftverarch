#pragma once
#include "IOHandler.h"
#include "Simulation.h"
#include "SimulationFactory.h"
#include "ScriptReaderBase.h"
#include "Logger.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <omp.h>

using namespace Swarm;

class Simulator{
private:
	std::shared_ptr<Simulation> sim;
	UI::IOHandler handler;
	std::shared_ptr<UI::Logger> logger;
	int currIterNum = 0;

public:
	Simulator(std::string scriptPath, Reader::ScriptReaderBase& reader) {
		auto simData = reader.readInitFile(scriptPath);
		SimulationFactory simFactory;
		sim = simFactory.makeSimulation(simData.first, simData.second);
		logger = std::make_shared<UI::Logger>("output.txt", sim->getMap());
	}

	void run() {
		while (!handler.getSimulationShouldExit()) {
			if (handler.getSimulationShouldPause()) {
				logger->print();
				while (!handler.getSimulationShouldContinue()) {
					if (handler.getSimulationShouldIterate()) {
						sim->lcm();
						currIterNum++;
						logger->print();
					}
				}
			}
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			sim->lcm();
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			std::cout << "Current iteration: " << currIterNum << ". (" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms)" << "\n";
			currIterNum++;
			logger->log();
		}
	}

};

