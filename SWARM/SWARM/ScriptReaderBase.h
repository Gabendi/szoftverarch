#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "SwarmIO.h"

class ScriptReaderBase
{
public:
	virtual std::pair<std::vector<SwarmIO::VertexData>, std::vector<SwarmIO::EntityData>> readInitFile(std::string filename) = 0;
};

