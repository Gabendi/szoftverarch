#pragma once
#include <algorithm>
#include <vector>
#include "Field.h"
#include "Entity.h"

class Simulation{
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Field>> map;

public:

	Simulation(std::vector<std::shared_ptr<Entity>>&& entities, std::vector<std::shared_ptr<Field>>&& map) {
		this->entities = entities;
		this-> map = map;
	}
	//perform an iteration of look compute move for all entities
	void lcm() {
	#pragma omp parallel num_threads(8)
		{
			#pragma omp for
			for (int i = 0; i < entities.size(); i++) {
				Entity* entity = entities[i].get();
				entity->look();
			}
			#pragma omp for
			for (int i = 0; i < entities.size(); i++) {
				Entity* entity = entities[i].get();
				entity->compute();
			}
		}
		for (int i = 0; i < entities.size(); i++) {
			Entity* entity = entities[i].get();
			entity->move();
		}
	}
	
	void removeEntity(std::shared_ptr<Entity> entity) {
		entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());			
	}

	std::vector<std::shared_ptr<Field>> getMap() {
		return map;
	}

	std::vector<std::shared_ptr<Entity>>& getEntities() {
		return entities;
	}
};

