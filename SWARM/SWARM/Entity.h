#pragma once
#include "Field.h"
#include "EntityBehavior.h"

class Entity{
private:
	int id;
	int viewDistance;
	EntityBehavior behav;
	Field* field;

public:
	void look();
	void compute();
	void move();

	Field* getField();

	void setField(Field* field);

	int getViewDistance();

	std::vector<Field*> getSeenFields();

	EntityBehavior& getBehavior();
	Entity(int id, int viewDistance);
};

