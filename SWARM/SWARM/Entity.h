#pragma once
#include "Field.h"
#include "EntityBehavior.h"

class Entity{
private:
	int id;
	int viewDistance;
	EntityBehavior& behav;
	std::shared_ptr<Field> field;

public:
	inline void look() {
		behav.look();
	}
	inline void compute() {
		behav.compute();
	}
	inline void move() {
		behav.move();
	}

	inline Field& getField() {
		return *field;
	}

	inline void setField(Field& field) { *this->field = field; }

	inline int getViewDistance() { return viewDistance; }

	inline Entity(int id, int viewDistance, EntityBehavior& behav):id(id), behav(behav), viewDistance(viewDistance) {}
};

