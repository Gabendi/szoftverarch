#include "Entity.h"

void Entity::look() {
	behav.look();
}
void Entity::compute() {
	behav.compute();
}
void Entity::move() {
	behav.move();
}

Field* Entity::getField() {
	return field;
}

void Entity::setField(Field* field) { this->field = field; }

int Entity::getViewDistance() { return viewDistance; }

std::vector<Field*> Entity::getSeenFields() { return field->rescursiveGetNeighbors(viewDistance); }

EntityBehavior& Entity::getBehavior() { return behav; }
Entity::Entity(int id, int viewDistance) :id(id), viewDistance(viewDistance) {
	behav.entity = this;
	field = nullptr;
}