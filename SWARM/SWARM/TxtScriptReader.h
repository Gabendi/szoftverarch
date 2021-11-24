#pragma once
#include "ScriptReaderBase.h"

namespace Swarm::Reader {
	class TxtScriptReader : public ScriptReaderBase
	{
	private:
		int noVertices;
		int noEntities;
		std::vector<SwarmIO::VertexData> readVertexData(std::ifstream& stream);
		std::vector<SwarmIO::EntityData> readEntityData(std::istream& stream);
		void readHeader(std::istream& istream);
	public:
		std::pair<std::vector<SwarmIO::VertexData>, std::vector<SwarmIO::EntityData>> readInitFile(std::string filename);
	};
}

