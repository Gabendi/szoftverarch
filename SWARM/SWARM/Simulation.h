#pragma once
#include <vector>
#include "Field.h"
#include "Entity.h"

class Simulation{
private:
	std::vector<std::shared_ptr<Field>> map;
	std::vector<std::shared_ptr<Entity>> entities;

public:

	Simulation(std::vector<std::shared_ptr<Entity>>&& entities, std::vector<std::shared_ptr<Field>>&& ma) {
		
	}
	//perform an iteration of look compute move for all entities
	void lcm() {
		for (int i = 0 ; i < entities.size(); i++) {
			Entity* entity = entities[i].get();
			entity->look();
		}
		for (int i = 0; i < entities.size(); i++) {
			Entity* entity = entities[i].get();
			entity->compute();
		}
		for (int i = 0; i < entities.size(); i++) {
			Entity* entity = entities[i].get();
			entity->move();
		}
	}

	auto getMap() -> decltype(map) {
		return map;
	}

	auto getEntities() -> decltype(entities) {
		return entities;
	}
};

