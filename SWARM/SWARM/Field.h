#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <set>
#include <stdexcept>
#include "Entity.h"

class Entity;

class Field{
	int id;
	std::vector<Field*> neighbors;
	Entity* entity;
public:
	std::vector<Field*>& getNeighbors();

	Field(int id);

	bool isOccupied();

	void ejectEntity();

	void acceptEntity(Entity* entityShared);

	void addNeighbor(std::shared_ptr<Field> fieldShared);

	inline Entity* getEntity(){ return entity; }

	int getId();

	std::vector<Field*> rescursiveGetNeighbors(int depth = 1);

	int getNeighborOccupiedCount();

	bool operator!=(Field& field);
};

