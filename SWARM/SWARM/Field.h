#pragma once
#include "Entity.h"

class Field{
	int id;
	std::vector<std::weak_ptr<Field>> neighbors;
public:
	std::vector<std::weak_ptr<Field>> getNeighbors() {
		return neighbors;
	}

	Field(int id):id(id){}


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

};

