#include "ecs.h"

int Entity::getId() const {
    return id;
}

void System::addEntityToSystem(Entity entity) {
    entities.push_back(entity);
}
void System::removeEntityFromSystem(Entity entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    entities.erase(it);
}
std::vector<Entity> System::getSystemEntities() const {
    return entities;
}

const Signature& System::getComponentSignature() const {
    return componentSignature;
};