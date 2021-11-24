#pragma once
#include <iterator>
#include "SwarmIO.h"
#include <windows.h>

class IOHandler{
public:
	bool getSimulationShouldPause();

	bool getSimulationShouldContinue();

	bool getSimulationShouldExit();

	bool getSimulationShouldIterate();
};

