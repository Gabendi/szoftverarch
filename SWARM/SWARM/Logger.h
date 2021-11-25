#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Field.h"
#include <string>


namespace Swarm::UI{
	class Logger
	{
		std::ofstream outFile;
		std::vector<std::shared_ptr<Field>> map;

	public:
		Logger(std::string outputPath, std::vector<std::shared_ptr<Field>> _map) {
			outFile.open(outputPath);
			map = _map;
		}
		void print() {
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

		void log() {
			for (auto& node : map) {
				int nodeId = node->getId();
				auto entity = node->getEntity();
				int entityId = -1;
				if (entity != nullptr) {
					entityId = entity->getId();
				}
				std::string output = std::to_string(nodeId) + ": " + (entityId == -1 ? "X" : std::to_string(entityId));
				outFile << output << std::endl;
			}
			outFile << std::endl;
		}

		~Logger() {
			outFile.close();
		}
	};
}


