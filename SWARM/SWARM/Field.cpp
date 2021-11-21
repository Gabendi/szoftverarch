#include "Field.h"

std::vector<std::weak_ptr<Field>>& Field::getNeighbors() {
	return neighbors;
}

Field::Field(int id) :id(id) {}

bool Field::isOccupied() {
	return entity.lock().get() != nullptr;
}

void Field::ejectEntity() {
	entity.reset();
}

void Field::acceptEntity(std::shared_ptr<Entity> entityShared) {
	if (isOccupied()) throw std::runtime_error("field is already occupied");
	entity = entityShared;
}

void Field::addNeighbor(std::shared_ptr<Field> fieldShared) {
	neighbors.push_back(fieldShared);
}

int Field::getId() { return id; }

std::vector<std::weak_ptr<Field>> Field::rescursiveGetNeighbors(int depth) {
	std::vector<std::weak_ptr<Field>> retArr;
	if (depth == 1) {
		for (auto wkPtr : neighbors) retArr.push_back(wkPtr);
		return retArr;
	}
	
	for (auto neighbor : neighbors) {
		auto neighborSh = neighbor.lock();
		auto currArr = neighborSh->rescursiveGetNeighbors(depth - 1);

		for (auto wkPtr : currArr) {
			
			bool contains = false;
			for(auto otherWkPtr : retArr) {
				if (otherWkPtr.lock().get() == wkPtr.lock().get()) {
					contains = true;
					break;
				}
			}
			if(contains) continue;
			
			retArr.push_back(wkPtr);
		}
	}
}

int Field::getNeighborOccupiedCount() {
	int accu = 0;
	for (auto v : neighbors) {
		if (v.lock()->isOccupied()) accu++;
	}
	return accu;
}

bool Field::operator!=(Field& field) { return id != field.id; }