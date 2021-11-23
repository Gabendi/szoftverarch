#include "IOHandler.h"


std::vector<SwarmIO::VertexData> IOHandler::readVertexData(std::ifstream& stream) {
	std::vector<SwarmIO::VertexData> vertexDatas;
	vertexDatas.reserve(noVertices);

	std::string c_VERTICES;
	stream >> c_VERTICES;
	if ("VERTICES" <=> c_VERTICES != 0) throw SwarmIO::IOException("VERTICES magic missing");
	stream.get();
	for (int i = 0; i < noVertices; i++) {
		SwarmIO::VertexData vd{};

		std::stringstream line;

		char bufferChar = stream.get();
		while (bufferChar != '\n') {
			line << bufferChar;
			bufferChar = stream.get();
		}

		int bufferInt;
		//reading id
		line >> vd.id;
		//reading neighbor ids
		while (line >> bufferInt) {
			vd.neighbors.push_back(bufferInt);
		}
		vertexDatas.emplace_back(vd);
	}
	return vertexDatas;
}

std::vector<SwarmIO::EntityData> IOHandler::readEntityData(std::istream& stream) {
	std::vector<SwarmIO::EntityData> entityDatas;
	entityDatas.reserve(noEntities);

	std::string c_ENTITIES;
	stream >> c_ENTITIES;
	if ("ENTITIES" <=> c_ENTITIES != 0) throw SwarmIO::IOException("ENTITIES magic missing");
	stream.get();
	for (int i = 0; i < noEntities; i++) {
		SwarmIO::EntityData ed{};

		std::stringstream line;

		char bufferChar = stream.get();
		while (bufferChar != '\n' && !stream.eof()) {
			line << bufferChar;
			bufferChar = stream.get();
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

void IOHandler::readHeader(std::istream& istream) {
	char c;
	istream >> c;
	if (c != 'v') throw SwarmIO::IOException{ "First data should be number of vertices" };
	istream >> noVertices;

	istream >> c;
	if (c != 'e') throw SwarmIO::IOException{ "Second data should be number of entities" };
	istream >> noEntities;

	if (noEntities > noVertices) throw SwarmIO::IOException{ "Number of entities is larger than number of vertices" };
}

std::pair<std::vector<SwarmIO::VertexData>, std::vector<SwarmIO::EntityData>> IOHandler::readInitFile(std::string filename) {
	std::ifstream initDataStream(filename);

	readHeader(initDataStream);
	auto vds = readVertexData(initDataStream);
	auto eds = readEntityData(initDataStream);

	return { vds, eds };
}

bool IOHandler::getSimulationShouldPause() {
	return GetAsyncKeyState(VK_CONTROL) > 1;
}

bool IOHandler::getSimulationShouldContinue() {
	return GetAsyncKeyState(VK_SPACE) > 1;
}

bool IOHandler::getSimulationShouldExit() {
	return GetAsyncKeyState(VK_ESCAPE) > 1;
}

bool IOHandler::getSimulationShouldIterate() {
	return GetAsyncKeyState(VK_RIGHT) > 1;
}