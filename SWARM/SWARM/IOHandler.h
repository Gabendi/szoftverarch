#pragma once
#include <fstream>
#include <sstream>
#include <iterator>
#include "SwarmIO.h"
#include <WinUser.h>

class IOHandler{
private:
	int noVertices;
	int noEntities;
public:
	std::vector<SwarmIO::VertexData> readVertexData(std::ifstream& stream);

	std::vector<SwarmIO::EntityData> readEntityData(std::istream& stream);

	void readHeader(std::istream& istream);

	virtual std::pair<std::vector<SwarmIO::VertexData>, std::vector<SwarmIO::EntityData>> readInitFile(std::string filename);

	bool getSimulationShouldPause();

	bool getSimulationShouldContinue();

	bool getSimulationShouldExit();

	bool getSimulationShouldIterate();
};

