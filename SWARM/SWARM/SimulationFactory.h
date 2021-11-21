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

	virtual std::shared_ptr<Simulation> makeSimulation(std::vector<SwarmIO::EntityData> entityDatas, std::vector<SwarmIO::VertexData> vertexDatas) {
		std::vector<std::shared_ptr<Entity>> entities;
		entities.reserve(entityDatas.size());

		for (size_t i = 0; i < entityDatas.size(); i++) {
			entities.emplace_back(parseEntityData(entityDatas[i]));
		}

		std::vector<std::shared_ptr<Field>> fields;
		fields.reserve(vertexDatas.size());
		for (int i = 0; i < vertexDatas.size(); i++) {
			fields.emplace_back(std::make_shared<Field>(i));
		}

		for (auto vdata : vertexDatas) {
			for (auto neighbor : vdata.neighbors) {
				fields[vdata.id]->addNeighbor(fields[neighbor]);
			}
		}

		return std::make_shared<Simulation>(std::move(entities), std::move(fields));
	}

	//parses entity data into an Entity class
	std::shared_ptr<Entity> parseEntityData(SwarmIO::EntityData data) {
		auto newEntitySh = std::make_shared<Entity>(data.id, data.viewDistance);
		
		EntityBehavior* behav = &(newEntitySh->getBehavior());

		behav->look = [behav]() {
			auto entity = behav->entity.lock();
			behav->opInfo.fields.clear();

			auto fields = entity->getField().rescursiveGetNeighbors(entity->getViewDistance());
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
			auto entity = behav->entity.lock();
			auto moveIntention = behav->opInfo.moveIntention.lock();
			if (*moveIntention != entity->getField()) {
				if(moveIntention->isOccupied()) return;
				else {
					entity->getField().ejectEntity();
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
			auto entityShared = behav->entity.lock();
			behav->opInfo.moveIntention = entityShared->getFieldShared();
			return true;
		});
	}

	void addRandomMovingBehav(EntityBehavior* behav, std::vector<std::function<compute_behav_t>>& computeBehavs) {
		computeBehavs.push_back([behav]() {
			auto entityShared = behav->entity.lock();

			auto neighbors = entityShared->getFieldShared()->getNeighbors();

			std::vector<std::shared_ptr<Field>> emptyNeighbors;

			for (int i = 0; i < neighbors.size(); i++) {
				auto neighborSh = neighbors[i].lock();
				if (!neighborSh->isOccupied()) emptyNeighbors.push_back(neighborSh);
			}

			if (emptyNeighbors.size() == 0) {
				behav->opInfo.moveIntention = entityShared->getFieldShared();
			}

			int randIndex = rand() % emptyNeighbors.size() + 1;
	
			behav->opInfo.moveIntention = emptyNeighbors[randIndex];
			return true;
		});
	}

	struct VertexData {
		int distance;
		std::shared_ptr<Field> prev;
	};

	void addSequentialMovingBehav(EntityBehavior* behav, std::vector<std::function<compute_behav_t>>& computeBehavs) {
		computeBehavs.push_back([behav]() {
			auto entityShared = behav->entity.lock();

			
			std::vector<std::weak_ptr<Field>> seenFieldsWps = behav->opInfo.fields;
			std::vector<std::shared_ptr<Field>> viewVertices;
			for(auto wp : seenFieldsWps) viewVertices.push_back(wp.lock());

			auto source = entityShared->getFieldShared();
			std::unordered_map<std::shared_ptr<Field>, VertexData> graph;
			std::set<std::shared_ptr<Field>> q;


			for (auto fieldPtr : viewVertices) {
				graph.at(fieldPtr).distance = INT32_MAX;
				q.insert(fieldPtr);
			}
			graph[source].distance = 0;
			graph[source].prev = source;

			while (q.size() > 0) {

				int min = INT32_MAX;
				std::shared_ptr<Field> u;
				for (auto v : q) {
					if (graph[v].distance < min) {
						min = graph[v].distance;
						u = v;
					}
				}

				q.erase(u);

				for (auto v : u->getNeighbors()) {
					auto neighborSh = v.lock();
					if (q. contains(neighborSh)) {
						
						int alt;
						if(neighborSh->isOccupied()) alt = INT_MAX;
						else alt = graph[u].distance + 1;

						if (alt < graph[neighborSh].distance) {
							graph[neighborSh].distance = alt;
							graph[neighborSh].prev = u;
						}
					}
				}
			}

			int maxId = -1;
			std::shared_ptr<Field> maxField;
			for(auto v : viewVertices){
				if (v->getId() > maxId && !v->isOccupied()) {
					maxId = v->getId();
					maxField = v;
				}
			}
			if(maxId == -1) behav->opInfo.moveIntention = entityShared->getFieldShared();
			else {
				std::shared_ptr<Field> moveIntention = maxField;
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
			auto entityShared = behav->entity.lock();
			auto field = entityShared->getFieldShared();
			auto neighbors = field->getNeighbors();

			int min = INT_MAX;
			std::weak_ptr<Field> minField;

			for (int i = 0; i < neighbors.size(); i++) {
				auto neighbor = neighbors[i];
				auto neighborShared = neighbor.lock();
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

