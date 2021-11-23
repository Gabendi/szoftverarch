#pragma once
#include "Field.h"
#include "EntityBehavior.h"

class Entity{
private:
	int id;
	int viewDistance;
	EntityBehavior behav;
	std::shared_ptr<Field> field;

public:
	void look();
	void compute();
	void move();

	Field& getField();

	std::shared_ptr<Field> getFieldShared();

	void setField(Field& field);

	int getViewDistance();

	std::vector<std::weak_ptr<Field>> getSeenFields();

	int getId() { return id; }
	EntityBehavior& getBehavior();
	Entity(int id, int viewDistance);
};

