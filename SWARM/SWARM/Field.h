#pragma once
#include <memory>
#include <vector>
#include "Entity.h"
#include <functional>
#include <set>
#include <stdexcept>

class Field{
	int id;
	std::vector<std::weak_ptr<Field>> neighbors;
	std::weak_ptr<Entity> entity;
public:
	std::vector<std::weak_ptr<Field>> getNeighbors() {
		return neighbors;
	}

	Field(int id):id(id){}

	bool isOccupied() {
		return entity.lock().get() != nullptr;
	}

	void ejectEntity() {
		entity.reset();
	}

	void acceptEntity(std::shared_ptr<Entity> entityShared) {
		if(isOccupied()) throw std::runtime_error("field is already occupied");
		entity = entityShared;
	}

	void addNeighbor(std::shared_ptr<Field> fieldShared) {
		neighbors.push_back(fieldShared);
	}

	inline int getId(){ return id; }

	std::set<std::weak_ptr<Field>> rescursiveGetNeighbors(int depth = 1) {
		std::set<std::weak_ptr<Field>> retArr;
		if (depth == 1) {
			for (auto wkPtr : neighbors) retArr.insert(wkPtr);
			return retArr;
		}
		for (auto neighbor : neighbors) {
			auto neighborSh = neighbor.lock();
			auto currArr = neighborSh->rescursiveGetNeighbors(depth - 1);

			for (auto wkPtr : currArr) retArr.insert(wkPtr);
		}
	}

	int getNeighborOccupiedCount() {
		int accu = 0;
		for (auto v : neighbors) {
			if(v.lock()->isOccupied()) accu++;
		}
		return accu;
	}

	bool operator!=(Field& field){ return id != field.id; }
};

