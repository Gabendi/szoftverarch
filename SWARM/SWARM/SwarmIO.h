#pragma once
#include <vector>
#include <string>

namespace SwarmIO {
	struct IOException {
		std::string what;
	};

	struct HeaderData {
		int noVertices;
		int noEntities;
	};

	struct VertexData {
		int id;
		int viewDistance;
		std::vector<int> neighbors;
	};
	
	struct EntityData {
		int id;
		//behaviors is a vector of strings, it contains the pipeline of behaviors read
		// from the input file
		int viewDistance;
		std::vector<std::string> behaviors;
	};
}