#pragma once
#include "Entity.h"
#include "SwarmIO.h"
#include "Field.h"
#include <set>

class SimulationFactory{
public:
	virtual std::vector<Entity> makeSimulation(std::vector<SwarmIO::EntityData> entityDatas, std::vector<SwarmIO::VertexData> vertexDatas) {
		std::vector<Entity> entities;
		entities.reserve(entityDatas.size());

		for (size_t i = 0; i < entityDatas.size(); i++) {
			
		}
	}

	//parses entity data into an Entity class
	Entity parseEntityData(SwarmIO::EntityData data) {
		EntityBehavior behav;
		Entity newEntity(data.id, data.viewDistance, behav);
		auto newEntityPtr = std::make_shared<Entity>(newEntity);
		
		behav.setEntity(newEntityPtr);
		Field field(data.id);

		behav.look = [&behav]() {
			auto sh = behav.entity.lock();
			auto entity = *sh;
			

			auto fields = entity.getField().rescursiveGetNeighbors(entity.getViewDistance());
			for (size_t i = 0; i < entity.getField().size(); i++) {
				behav.opInfo.fields.push_back()
			}
			
			
		};

	}
};

