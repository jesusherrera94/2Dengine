#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/rigidBodyComponent.h"
#include "../components/transformComponent.h"

class MovementSystem: public System {
    public:
        MovementSystem() {
            requiredComponent<TransformComponent>();
            requiredComponent<RigidBodyComponent>();
        }
        void update(double deltaTime) {
            // TODO: Implement movement logic here
            for (auto entity : getSystemEntities()) {
                TransformComponent& transform = entity.getComponent<TransformComponent>();
                const RigidBodyComponent rigidBodyComponent = entity.getComponent<RigidBodyComponent>();

                transform.position.x += rigidBodyComponent.velocity.x * deltaTime;
                transform.position.y += rigidBodyComponent.velocity.y * deltaTime;
                bool isEntityOutsideMap = (
                    transform.position.x < 0 ||
                    transform.position.x > Game::mapWidth ||
                    transform.position.y < 0 ||
                    transform.position.y > Game::mapHeight
                    );
                // kill all the entities that move outside the map boundaries
                if (isEntityOutsideMap && !entity.hasTag("player")) {
                    entity.kill();
                }
            }
        }
};

#endif