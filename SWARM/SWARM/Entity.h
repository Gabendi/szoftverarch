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
	void remove();
	int getViewDistance();

	std::vector<Field*> getSeenFields();

	int getId() { return id; }
	EntityBehavior& getBehavior();
	Entity(int id, int viewDistance);
};

