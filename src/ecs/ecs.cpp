#include "ecs.h"
#include "../logger/logger.h"

int IComponent::nextId = 0;

int Entity::getId() const {
    return id;
}

void Entity::kill() {
    registry->killEntity(*this);
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
    int entityId;

    if (freeIds.empty()){
        // if there are no freeIds waiting to be reused
        entityId = numEntities++;
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        // reuse an id from the list of previously removed entities
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this; // setting the entity to itself
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id: " + std::to_string(entityId));
    return entity;
}

void Registry::killEntity(Entity entity) {
    entitiesToBeRemoved.insert(entity);
}

void Registry::update() {

    // Add the entities that are waiting to be created to the active system
    for(auto entity: entitiesToBeAdded) {
        addEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
    
    // remove the entities that are waiting to be removed in the active system
    for (auto entity: entitiesToBeRemoved) {
        removeEntitiesFromSystems(entity);
        entityComponentSignatures[entity.getId()].reset();
        // make the entity id available to reuse
        freeIds.push_back(entity.getId());
    }
    entitiesToBeRemoved.clear();
}

void Registry::addEntityToSystems(Entity entity) {
    const auto entityId = entity.getId();

    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    //loop all the systems
    for (auto& system : systems) {
        const auto& systemComponentSignature = system.second->getComponentSignature();
        // bitwise and to get if the signatures match to see if the component is interested on the component
        bool isInterest = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
        if (isInterest) {
            // TODO add entity to to the system
            system.second->addEntityToSystem(entity);
        }
    }

}

void Registry::removeEntitiesFromSystems(Entity entity) {
    for(auto system: systems) {
        system.second->removeEntityFromSystem(entity);
    }
}