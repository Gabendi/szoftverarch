#pragma once
#include <fstream>
#include <sstream>
#include <iterator>
#include "SwarmIO.h"

class IOHandler{
private:
	int noVertices;
	int noEntities;
public:
	std::vector<SwarmIO::VertexData> readVertexData(std::ifstream& stream) {
		std::vector<SwarmIO::VertexData> vertexDatas;
		vertexDatas.reserve(noVertices);

		std::string c_VERTICES;
		stream >> c_VERTICES;
		if("VERTICES" <=> c_VERTICES != 0) throw SwarmIO::IOException("VERTICES magic missing");

		for (int i = 0; i < noVertices; i++) {
			SwarmIO::VertexData vd{};

			std::stringstream line;

			char bufferChar = stream.get();
			while (bufferChar != '\n') {
				bufferChar = stream.get();
				line << bufferChar;
			}

			int bufferInt;
			//reading id
			line >> vd.id;
			//reading neighbor ids
			while(line >> bufferInt){
				vd.neighbors.push_back(bufferInt);
			}
			vertexDatas.emplace_back(vd);
		}
		return vertexDatas;
	}

	std::vector<SwarmIO::EntityData> readEntityData(std::istream& stream) {
		std::vector<SwarmIO::EntityData> entityDatas;
		entityDatas.reserve(noEntities);
		
		std::string c_ENTITIES;
		stream >> c_ENTITIES;
		if ("ENTITIES" <=> c_ENTITIES!= 0) throw SwarmIO::IOException("ENTITIES magic missing");
		for (int i = 0; i < noEntities; i++) {
			SwarmIO::EntityData ed{};

			std::stringstream line;

			char bufferChar = stream.get();
			while (bufferChar != '\n') {
				bufferChar = stream.get();
				line << bufferChar;
			}

			std::string bufferString;
			//reading id
			line >> ed.id;
			line >> ed.viewDistance;
			//reading behaviors ids
			while (line >> bufferString) {
				ed.behaviors.push_back(bufferString);
			}
			entityDatas.emplace_back(ed);
		}

		return entityDatas;
	}

	void readHeader(std::istream& istream) {
		char c;
		istream >> c;
		if (c != 'v') throw SwarmIO::IOException{"First data should be number of vertices"};
		istream >> noVertices;

		istream >> c;
		if (c != 'e') throw SwarmIO::IOException{ "Second data should be number of entities" };
		istream >> noEntities;

		if(noEntities > noVertices) throw SwarmIO::IOException{"Number of entities is larger than number of vertices"};
	}

	virtual std::pair<std::vector<SwarmIO::VertexData>, std::vector<SwarmIO::EntityData>> readInitFile(std::string filename) {
		std::ifstream initDataStream(filename);

		readHeader(initDataStream);
		auto vds = readVertexData(initDataStream);
		auto eds = readEntityData(initDataStream);

		return {vds, eds};
	}
};

