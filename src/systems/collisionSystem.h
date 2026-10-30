#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/boxColliderComponent.h"
#include "../components/transformComponent.h"
#include "../logger/logger.h"

class CollisionSystem: public System {
    public:
        CollisionSystem() {
            requiredComponent<BoxColliderComponent>();
            requiredComponent<TransformComponent>();
        }
        void update() {
            auto entities = getSystemEntities();
            // Loop all the entitis that the system is interested in
            // and make a "right-side" validation to avoid repetition
            // and reduce the complexity from O(n^2) to something more acceptable.
            for (auto i = entities.begin(); i != entities.end(); i++) {
                Entity  a = *i;
                auto aTransform = a.getComponent<TransformComponent>();
                auto aCollider = a.getComponent<BoxColliderComponent>();

                for (auto j = i; j != entities.end(); j++) {
                    Entity b = *j;
                    if (a==b) {
                        //bypass if it is the same entity
                        continue;
                    }
                        auto bTransform = b.getComponent<TransformComponent>();
                        auto bCollider = b.getComponent<BoxColliderComponent>();
                        // check for the collision between a and b
                        bool collisionHappened =checkAABBCollision(
                            aTransform.position.x + aCollider.offset.x,
                            aTransform.position.y + aCollider.offset.y,
                            aCollider.width,
                            aCollider.height,
                            bTransform.position.x + bCollider.offset.x,
                            bTransform.position.y + + bCollider.offset.y,
                            bCollider.width,
                            bCollider.height
                        );

                        if (collisionHappened) {
                            Logger::Log("Entity " + std::to_string(a.getId()) + " is colliding with entity " + std::to_string(b.getId()));
                            // Emit an event
                        }
                }
            }
        }
        // This must be cleaned later
        bool checkAABBCollision(double aX, double aY, double aWidth, double aHeight, double bX, double bY, double bWidth, double bHeight){
            return (
                aX < bX + bWidth
                && aX + aWidth > bX
                && aY < bY + bHeight
                && aY + aHeight > bY
            );
        }
};

#endif