#include "ecs.h"
#include "../logger/logger.h"

int Entity::getId() const {
    return id;
}

void System::addEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::removeEntityFromSystem(Entity entity) {
    entities.erase(std::remove_if(
        entities.begin(),
        entities.end(),
        // recieves the entity from the function arg
        [&entity](Entity other) {
            return entity == other;
        }),
        entities.end());
}

std::vector<Entity> System::getSystemEntities() const {
    return entities;
}

const Signature& System::getComponentSignature() const {
    return componentSignature;
};

Entity Registry::createEntity() {
    int entityId = numEntities++;

    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with id: " + std::to_string(entityId));
    return entity;
}

void Registry::update() {

    // Add the entities that are waiting to be created to the active system
    // remove the entities that are waiting to be removed in the active system

}