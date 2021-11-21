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

Field& Entity::getField() {
	return *field;
}

std::shared_ptr<Field> Entity::getFieldShared() { return field; }

void Entity::setField(Field& field) { *this->field = field; }

int Entity::getViewDistance() { return viewDistance; }

std::vector<std::weak_ptr<Field>> Entity::getSeenFields() { return field->rescursiveGetNeighbors(viewDistance); }

EntityBehavior& Entity::getBehavior() { return behav; }
Entity::Entity(int id, int viewDistance) :id(id), viewDistance(viewDistance) {
	behav.entity = std::shared_ptr<Entity>(this);
}