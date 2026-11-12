#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

/*
* For some strange reason this file had to be named script_System, instad of scriptSystem
* breaking the consistency because the intellisense and auto-complete breaks with the camel-case name
*/

#include <sol/sol.hpp>

#include "../ecs/ecs.h"
#include "../components/scriptComponent.h"
#include "../components/transformComponent.h"

#include "../logger/logger.h"


// declare some native c++ functions that we will bind with Lua Function
// int getEntityPosition(Entity entity) {

// }

void setEntityPosition(Entity entity, double x, double y) {
    if (entity.hasComponent<TransformComponent>()) {
        auto& transform = entity.getComponent<TransformComponent>();
        transform.position.x = x;
        transform.position.y = y;
    } else {
        Logger::Err("Trying to set the position of an entity that has no TransformComponent");
    }
}

class ScriptSystem: public System {
    public:
        ScriptSystem() {
            requiredComponent<ScriptComponent>();
        }

        void createLuaBinding(sol::state& lua) {
            // create the entity type so Lua knows what an entity is
            lua.new_usertype<Entity>(
                "entity",
                "get_id", &Entity::getId,
                "destroy", &Entity::kill,
                "has_tag", &Entity::hasTag,
                "belongs_to_group", &Entity::belongsToGroup
            );
            // create the binding function
            lua.set_function("set_position", setEntityPosition);
        }

        void update(double deltaTime, int ellapsedTime) {
            for(auto entity: getSystemEntities()) {
                auto script = entity.getComponent<ScriptComponent>();
                script.func(entity, deltaTime, ellapsedTime); // execute the lua function
            }
        }
};

#endif