#pragma once
#include "EntityBehavior.h"
#include "Entity.h"
#include "SwarmIO.h"
#include "Field.h"
#include "Simulation.h"
#include <set>
#include <vector>
#include <functional>

class SimulationFactory{
public:

	using compute_behav_t = bool();

	virtual std::shared_ptr<Simulation> makeSimulation(std::vector<SwarmIO::VertexData> vertexDatas, std::vector<SwarmIO::EntityData> entityDatas) {
		std::vector<std::shared_ptr<Entity>> entities;
		entities.reserve(entityDatas.size());

		for (size_t i = 0; i < entityDatas.size(); i++) {
			entities.emplace_back(parseEntityData(entityDatas[i]));
		}

		std::vector<std::shared_ptr<Field>> fields;
		fields.reserve(vertexDatas.size());
		for (int i = 1; i <= vertexDatas.size(); i++) {
			fields.emplace_back(std::make_shared<Field>(i));
		}

		for (auto vdata : vertexDatas) {
			for (auto neighbor : vdata.neighbors) {
				fields[vdata.id-1]->addNeighbor(fields[neighbor-1]);
			}
		}

		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->setField(fields[i].get());
			fields[i].get()->acceptEntity(entities[i].get());
		}

		std::shared_ptr<Simulation> simPtr = std::make_shared<Simulation>(std::move(entities), std::move(fields));
		return std::make_shared<Simulation>(std::move(entities), std::move(fields));
	}

	//parses entity data into an Entity class
	std::shared_ptr<Entity> parseEntityData(SwarmIO::EntityData data) {
		auto newEntitySh = std::make_shared<Entity>(data.id, data.viewDistance);
		
		EntityBehavior* behav = &(newEntitySh->getBehavior());

		behav->look = [behav]() {
			auto entity = behav->entity;
			behav->opInfo.fields.clear();

			auto field = entity->getField();
			auto fields = field->rescursiveGetNeighbors(entity->getViewDistance());
			auto fieldsBegin = fields.begin();
			auto fieldsEnd = fields.end();
			for (;fieldsBegin != fieldsEnd; fieldsBegin++) {
				behav->opInfo.fields.push_back(*fieldsBegin);
			}
		};

		behav->compute = ([behav]() {
			for (auto moveBehav : behav->moves) {
				if (moveBehav()) return;
			}
		});

		behav->moves = parseMoveBehaviors(behav, data);

		behav->move = [behav]() {
			auto entity = behav->entity;
			auto moveIntention = behav->opInfo.moveIntention;
			if (moveIntention != entity->getField()) {
				if(moveIntention->isOccupied()) return;
				else {
					entity->getField()->ejectEntity();
					moveIntention->acceptEntity(entity);
					return;
				}
			}
			else {
				return;
			}
		};

		return newEntitySh;
	}


	std::vector<std::function<bool()>> parseMoveBehaviors(EntityBehavior* behav, SwarmIO::EntityData data) {
		std::vector<std::function<bool()>> computeBehavs;
		for (int i = 0; i < data.behaviors.size(); i++) {
			
			auto behavString = data.behaviors[i];
			if (behavString.compare("NotMoving") == 0) {
				addNotMovingBehav(behav, computeBehavs);
			}

			else if (behavString.compare("RandomMoving") == 0) {
				addRandomMovingBehav(behav, computeBehavs);
			}

			else if (behavString.compare("SequentialMove") == 0) {
				addSequentialMovingBehav(behav, computeBehavs);
			}

			else if (behavString.compare("MoveFromEntity") == 0) {
				addMoveFromEntityBehav(behav, computeBehavs);
			}

			else {
				addSequentialMovingBehav(behav, computeBehavs);
			}
		}
		if (data.behaviors.size() == 0) {
			addSequentialMovingBehav(behav, computeBehavs);
		}
		return computeBehavs;
	}

	void addNotMovingBehav(EntityBehavior* behav, std::vector<std::function<compute_behav_t>>& computeBehavs) {
		computeBehavs.push_back([behav]() {
			auto entityShared = behav->entity;
			behav->opInfo.moveIntention = entityShared->getField();
			return true;
		});
	}

	void addRandomMovingBehav(EntityBehavior* behav, std::vector<std::function<compute_behav_t>>& computeBehavs) {
		computeBehavs.push_back([behav]() {
			auto entityShared = behav->entity;

			auto neighbors = entityShared->getField()->getNeighbors();

			std::vector<Field*> emptyNeighbors;

			for (int i = 0; i < neighbors.size(); i++) {
				auto neighborSh = neighbors[i];
				if (!neighborSh->isOccupied()) emptyNeighbors.push_back(neighborSh);
			}

			if (emptyNeighbors.size() == 0) {
				behav->opInfo.moveIntention = entityShared->getField();
				return true;
			}

			int randIndex = rand() % emptyNeighbors.size();
	
			behav->opInfo.moveIntention = emptyNeighbors[randIndex];
			return true;
		});
	}

	struct VertexData {
		int distance;
		Field* prev;
	};

	void addSequentialMovingBehav(EntityBehavior* behav, std::vector<std::function<compute_behav_t>>& computeBehavs) {
		computeBehavs.push_back([behav]() {
			auto entityShared = behav->entity;

			
			std::vector<Field*> viewVertices = behav->opInfo.fields;

			auto source = entityShared->getField();
			std::unordered_map<Field*, VertexData> graph;
			std::set<Field*> q;


			for (auto fieldPtr : viewVertices) {
				graph.insert({fieldPtr,  VertexData{INT32_MAX, nullptr}});
				q.insert(fieldPtr);
			}
			graph[source].distance = 0;
			graph[source].prev = source;

			bool sorrounded = true;
			for (auto neighbor : source->getNeighbors()) {
				if(!neighbor->isOccupied()){
					sorrounded = false;
				}
			}
			if(sorrounded) {
				behav->opInfo.moveIntention = source;
				return true;
			}

			while (q.size() > 0) {

				int min = INT32_MAX;
				Field* u = nullptr;
				for (auto v : q) {
					auto distance = graph[v].distance;
					if (distance < min) {
						min = graph[v].distance;
						u = v;
					}
				}

				if(u == nullptr) break;
				q.erase(u);

				for (auto v : u->getNeighbors()) {
					auto neighbor = v;
					if (q.contains(neighbor)) {
						
						int alt;
						if(neighbor->isOccupied()) alt = INT_MAX;
						else alt = graph[u].distance + 1;

						if (alt < graph[neighbor].distance) {
							graph[neighbor].distance = alt;
							graph[neighbor].prev = u;
						}
					}
				}
			}

			int maxId = -1;
			Field* maxField = viewVertices[0];
			for(auto v : viewVertices){
				if (v->getId() > maxId && !v->isOccupied()) {
					maxId = v->getId();
					maxField = v;
				}
			}
			if(maxId == -1) behav->opInfo.moveIntention = entityShared->getField();
			else {
				Field* moveIntention = maxField;
				for (int i = 0; i < graph[maxField].distance - 2; i++) {
					moveIntention = graph[moveIntention].prev;
				}
				behav->opInfo.moveIntention = moveIntention;
			}

			return true;
		});
	}

	void addMoveFromEntityBehav(EntityBehavior* behav, std::vector<std::function<compute_behav_t>>& computeBehavs) {
		computeBehavs.push_back([behav]() {
			auto entityShared = behav->entity;
			auto field = entityShared->getField();
			auto neighbors = field->getNeighbors();

			int min = INT_MAX;
			Field* minField = neighbors[0];

			for (int i = 0; i < neighbors.size(); i++) {
				auto neighbor = neighbors[i];
				auto neighborShared = neighbor;
				auto occCount = neighborShared->getNeighborOccupiedCount();
				if (!neighborShared->isOccupied() && occCount < min) {
					minField = neighborShared;
					min = occCount;
				}
			}
			behav->opInfo.moveIntention = minField;
			return true;
		});
	}
};

