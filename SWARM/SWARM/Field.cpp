#include "Field.h"

std::vector<Field*>& Field::getNeighbors() {
	return neighbors;
}

Field::Field(int id) :id(id) {
	entity = nullptr;
}

bool Field::isOccupied() {
	return entity != nullptr;
}

void Field::ejectEntity() {
	entity = nullptr;
}

void Field::acceptEntity(Entity* entityShared) {
	if (isOccupied()) throw std::runtime_error("field is already occupied");
	entity = entityShared;
}

void Field::addNeighbor(std::shared_ptr<Field> fieldShared) {
	neighbors.push_back(fieldShared.get());
}

int Field::getId() { return id; }

#include <iostream>

std::vector<Field*> Field::rescursiveGetNeighbors(int depth) {
	std::vector<Field*> retArr;
	if (depth == 1) {
		for (auto wkPtr : neighbors) retArr.push_back(wkPtr);
		return retArr;
	}
	
	for (auto neighbor : neighbors) {
		auto currArr = neighbor->rescursiveGetNeighbors(depth - 1);

		for (auto ptr : currArr) {
			
			bool contains = false;
			for(auto otherPtr : retArr) {
				if (otherPtr == ptr) {
					contains = true;
					break;
				}
			}
			if(contains) continue;
			
			retArr.push_back(ptr);
		}
	}
	return retArr;
}

int Field::getNeighborOccupiedCount() {
	int accu = 0;
	for (auto v : neighbors) {
		if (v->isOccupied()) accu++;
	}
	return accu;
}

bool Field::operator!=(Field& field) { return id != field.id; }