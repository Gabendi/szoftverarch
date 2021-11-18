#pragma once
#include <vector>
#include "Field.h"

class Simulation{
private:
	std::vector<Field> map;
	std::vector<Entity> entities;

public:
	//perform and iteration of look compute move for all entities
	void lcm() {
		for (int i = 0 ; i < entities.size(); i++) {
			entities[i].look();
			entities[i].compute();
			entities[i].move();
		}
	}

	auto getMap() -> decltype(map) {
		return map;
	}

	auto getEntities() -> decltype(entities) {
		return entities;
	}
};

