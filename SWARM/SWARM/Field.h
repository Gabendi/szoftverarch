#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <set>
#include <stdexcept>

class Entity;

class Field{
	int id;
	std::vector<std::weak_ptr<Field>> neighbors;
	Entity* entity;
public:
	std::vector<std::weak_ptr<Field>>& getNeighbors();

	Field(int id);

	bool isOccupied();

	void ejectEntity();

	void acceptEntity(Entity* entityShared);

	void addNeighbor(std::shared_ptr<Field> fieldShared);

	int getId();

	std::vector<std::weak_ptr<Field>> rescursiveGetNeighbors(int depth = 1);

	int getNeighborOccupiedCount();

	bool operator!=(Field& field);
};

