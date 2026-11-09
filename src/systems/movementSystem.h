#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/rigidBodyComponent.h"
#include "../components/transformComponent.h"
#include "../components/spriteComponent.h"
#include "../eventBus/eventBus.h"
#include "../events/collisionEvent.h"

class MovementSystem: public System {
    private:
        void onEnemyHitsObstacle(Entity a, Entity b) {
            if (a.hasComponent<RigidBodyComponent>() && a.hasComponent<SpriteComponent>()) {
                auto& rigidBody = a.getComponent<RigidBodyComponent>();
                auto& sprite = a.getComponent<SpriteComponent>();

                if (rigidBody.velocity.x !=0) {
                    rigidBody.velocity.x *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                }
                if (rigidBody.velocity.y !=0) {
                    rigidBody.velocity.y *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;

                }
            }
        }
    public:
        MovementSystem() {
            requiredComponent<TransformComponent>();
            requiredComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
        }

        void onCollision(CollisionEvent& event) {
            Entity b = event.b;
            Entity a = event.a;
            Logger::Log("The damage system recieved an event collision between entities " + std::to_string(a.getId()) + " and " + std::to_string(b.getId()));

            if (a.belongsToGroup("enemies") && b.belongsToGroup("obstacles")) {
                onEnemyHitsObstacle(a,b);
            }
            if (b.belongsToGroup("obstacles") && a.hasTag("enemies")) {
                onEnemyHitsObstacle(b,a);
            }

        }

        void update(double deltaTime) {
            // TODO: Implement movement logic here
            for (auto entity : getSystemEntities()) {
                TransformComponent& transform = entity.getComponent<TransformComponent>();
                RigidBodyComponent& rigidBodyComponent = entity.getComponent<RigidBodyComponent>();

                transform.position.x += rigidBodyComponent.velocity.x * deltaTime;
                transform.position.y += rigidBodyComponent.velocity.y * deltaTime;
                bool isEntityOutsideMap = (
                    transform.position.x < 0 ||
                    transform.position.x > Game::mapWidth ||
                    transform.position.y < 0 ||
                    transform.position.y > Game::mapHeight
                    );
                if (isEntityOutsideMap && entity.hasTag("player")) {
                    rigidBodyComponent.velocity.x = 0;
                    rigidBodyComponent.velocity.y = 0;
                }
                // kill all the entities that move outside the map boundaries
                if (isEntityOutsideMap && !entity.hasTag("player")) {
                    entity.kill();
                }
            }
        }
};

#endif