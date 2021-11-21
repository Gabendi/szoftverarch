#pragma once
#include "Entity.h"
#include <functional>
#include <tuple>
#include <vector>

struct OperationInfo {
	std::weak_ptr<Field> moveIntention;
	// [0] neighbor towards [2] with distance of [1]
	std::vector<std::weak_ptr<Field>> fields;
};

class EntityBehavior{
public:
	std::weak_ptr<Entity> entity;
	OperationInfo opInfo;
	std::function<void()> look;
	std::function<void()> compute;
	std::function<void()> move;
	std::vector<std::function<bool()>> moves;

	EntityBehavior(std::shared_ptr<Entity> _entity){
		entity = _entity;
	}

	EntityBehavior(){}

	void setEntity(std::shared_ptr<Entity> entity){ this->entity = entity; }
};

