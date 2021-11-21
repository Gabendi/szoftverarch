#pragma once
#include "Field.h"
#include "EntityBehavior.h"
#include "EntityFactory.h"

class Entity{
private:
	int id;
	int viewDistance;
	EntityBehavior behav;
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

	inline std::shared_ptr<Field> getFieldShared(){ return field; }

	inline void setField(Field& field) { *this->field = field; }

	inline int getViewDistance() { return viewDistance; }

	inline std::set<std::weak_ptr<Field>> getSeenFields(){ return field->rescursiveGetNeighbors(viewDistance); }

	inline EntityBehavior& getBehavior() { return behav; }
	inline Entity(int id, int viewDistance):id(id), viewDistance(viewDistance) {
		behav.entity = std::make_shared<Entity>(this);
	}
};

