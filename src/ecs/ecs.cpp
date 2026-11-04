#include "ecs.h"
#include "../logger/logger.h"

int IComponent::nextId = 0;

int Entity::getId() const {
    return id;
}

void Entity::kill() {
    registry->killEntity(*this);
}

void Entity::tag(const std::string& tag) {
    registry->tagEntity(*this, tag);
}
bool Entity::hasTag(const std::string& tag) const {
    return registry->entityHasTag(*this, tag);
}
void Entity::group(const std::string& groupName) {
    registry->groupEntity(*this, groupName);
}
bool Entity::belongsToGroup(const std::string& groupName) const {
    return registry->entityBelongsToGroup(*this, groupName);
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

        // remove any traces of that entity from the tag/group maps
        removeEntityTag(entity);
        removeEntityGroup(entity);
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

void Registry::tagEntity(Entity entity, const std::string& tag){
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.getId(), tag);
}

bool Registry::entityHasTag(Entity entity, const std::string& tag) const {
    if (tagPerEntity.find(entity.getId()) == tagPerEntity.end()) {
        return false;
    }
    return entityPerTag.find(tag) ->second == entity;
}
Entity Registry::getEntityByTag(const std::string& tag) const {
    return entityPerTag.at(tag);
}
void Registry::removeEntityTag(Entity entity) {
    auto taggedEntity = tagPerEntity.find(entity.getId());
    if (taggedEntity != tagPerEntity.end()) {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

void Registry::groupEntity(Entity entity, const std::string& group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.getId(), group);
}

bool Registry::entityBelongsToGroup(Entity entity, const std::string& group) const {
    auto groupEntities = entitiesPerGroup.at(group);
    return groupEntities.find(entity.getId()) != groupEntities.end();
}

std::vector<Entity> Registry::getEntitiesByGroup(const std::string& group) const {
    auto& setOfEntities = entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::removeEntityGroup(Entity entity) {
    auto groupedEntity = groupPerEntity.find(entity.getId());
    if (groupedEntity != groupPerEntity.end()) {
        auto group = entitiesPerGroup.find(groupedEntity->second);
        if (group != entitiesPerGroup.end()) {
            auto entityInGroup = group->second.find(entity);
            if (entityInGroup != group->second.end()) {
                group->second.erase(entityInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}