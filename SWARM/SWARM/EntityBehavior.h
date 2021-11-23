#pragma once
#include <functional>
#include <tuple>
#include <vector>

class Entity;
class Field;

struct OperationInfo {
	std::weak_ptr<Field> moveIntention;
	std::vector<std::weak_ptr<Field>> fields;
};

class EntityBehavior{
public:
	Entity* entity;
	OperationInfo opInfo;
	std::function<void()> look;
	std::function<void()> compute;
	std::function<void()> move;
	std::vector<std::function<bool()>> moves;

	EntityBehavior(std::shared_ptr<Entity> _entity);

	EntityBehavior();

	void setEntity(std::shared_ptr<Entity> entity);
};

