#pragma once
#include <iterator>
#include "SwarmIO.h"
#include <windows.h>

namespace Swarm::UI {
	class IOHandler {
	public:
		bool getSimulationShouldPause();

		bool getSimulationShouldContinue();

		bool getSimulationShouldExit();

		bool getSimulationShouldIterate();
	};
}

