#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

/*
* For some strange reason this file had to be named script_System, instad of scriptSystem
* breaking the consistency because the intellisense and auto-complete breaks with the camel-case name
*/

#include <sol/sol.hpp>
#include <tuple>

#include "../ecs/ecs.h"
#include "../components/scriptComponent.h"
#include "../components/transformComponent.h"
#include "../components/rigidBodyComponent.h"
#include "../components/animationComponent.h"
#include "../components/projectileEmitterComponent.h"

#include "../logger/logger.h"


// declare some native c++ functions that we will bind with Lua Function
std::tuple<double, double> getEntityPosition(Entity entity) {
    if (entity.hasComponent<TransformComponent>()) {
        const auto transform = entity.getComponent<TransformComponent>();
        return std::make_tuple(transform.position.x, transform.position.y);
    } else {
        Logger::Err("Trying to set the position of an entity that has no TransformComponent");
        return std::make_tuple(0.0,0.0);
    }

}

std::tuple<double, double> getEntityVelocity(Entity entity) {
    if (entity.hasComponent<RigidBodyComponent>()) {
        const auto rigidBody = entity.getComponent<RigidBodyComponent>();
        return std::make_tuple(rigidBody.velocity.x, rigidBody.velocity.y);
    } else {
        Logger::Err("Trying to set the position of an entity that has no RigidBodyComponent");
        return std::make_tuple(0.0,0.0);
    }

}

void setEntityPosition(Entity entity, double x, double y) {
    if (entity.hasComponent<TransformComponent>()) {
        auto& transform = entity.getComponent<TransformComponent>();
        transform.position.x = x;
        transform.position.y = y;
    } else {
        Logger::Err("Trying to set the position of an entity that has no TransformComponent");
    }
}

void setEntityVelocity(Entity entity, double x, double y) {
    if (entity.hasComponent<RigidBodyComponent>()) {
        auto& rigidBody = entity.getComponent<RigidBodyComponent>();
        rigidBody.velocity.x = x;
        rigidBody.velocity.y = y;
    } else {
        Logger::Err("Trying to set the position of an entity that has no RigidBodyComponent");
    }
}

void setEntityRotation(Entity entity, double angle) {
    if (entity.hasComponent<TransformComponent>()) {
        auto& transform = entity.getComponent<TransformComponent>();
        transform.rotation = angle;
    } else {
        Logger::Err("Trying to set the position of an entity that has no TransformComponent");
    }
}

void setEntityAnimationFrame(Entity entity, int frame) {
    if (entity.hasComponent<AnimationComponent>()) {
        auto& animation = entity.getComponent<AnimationComponent>();
        animation.currentFrame = frame;
    } else {
        Logger::Err("Trying to set the position of an entity that has no AnimationComponent");
    }
}

void setProjectileVelocity(Entity entity, double x, double y) {
    if (entity.hasComponent<ProjectileEmitterComponent>()) {
        auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
        projectileEmitter.projectileVelocity.x = x;
        projectileEmitter.projectileVelocity.y = y;
    } else {
        Logger::Err("Trying to set the position of an entity that has no ProjectileEmitterComponent");
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
            lua.set_function("get_position", getEntityPosition);
            lua.set_function("set_velocity", setEntityVelocity);
            lua.set_function("get_velocity", getEntityVelocity);
            lua.set_function("set_rotation", setEntityRotation);
            lua.set_function("set_projectile_velocity", setProjectileVelocity);
            lua.set_function("set_animation_frame", setEntityAnimationFrame);
        }

        void update(double deltaTime, int ellapsedTime) {
            for(auto entity: getSystemEntities()) {
                auto script = entity.getComponent<ScriptComponent>();
                script.func(entity, deltaTime, ellapsedTime); // execute the lua function
            }
        }
};

#endif