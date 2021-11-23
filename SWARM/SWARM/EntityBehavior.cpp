#include "EntityBehavior.h"

EntityBehavior::EntityBehavior(std::shared_ptr<Entity> _entity) {
	entity = _entity.get();
}

EntityBehavior::EntityBehavior() {}

void EntityBehavior::setEntity(std::shared_ptr<Entity> entity) { this->entity = entity.get(); }