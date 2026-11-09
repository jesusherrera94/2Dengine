#ifndef PROJECTILE_EMITER_SYSTEM_H
#define PROJECTILE_EMITER_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/projectileEmitterComponent.h"
#include "../components/transformComponent.h"
#include "../components/rigidBodyComponent.h"
#include "../components/spriteComponent.h"
#include "../components/boxColliderComponent.h"
#include "../components/projectileComponent.h"
#include "../components/cameraFollowComponent.h"
#include "../eventBus/eventBus.h"
#include "../events/keyPressedEvent.h"


class ProjectileEmitterSystem: public System {
    public:
        ProjectileEmitterSystem() {
            requiredComponent<ProjectileEmitterComponent>();
            requiredComponent<TransformComponent>();
        }

        void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitterSystem::onKeyPressed);
        }

        void onKeyPressed(KeyPressedEvent& event) {
            if (event.keyCode == SDLK_SPACE) {
                for (auto entity: getSystemEntities()) {
                    if (entity.hasComponent<CameraFollowComponent>()) {
                        const auto projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
                        const auto transform = entity.getComponent<TransformComponent>();
                        const auto rigidBody = entity.getComponent<RigidBodyComponent>();

                        glm::vec2 projectilePosition = transform.position;
                        if (entity.hasComponent<SpriteComponent>()) {
                            auto sprite = entity.getComponent<SpriteComponent>();
                            projectilePosition.x += (transform.scale.x * sprite.width / 2);
                            projectilePosition.y += (transform.scale.y * sprite.height / 2);
                        }

                        glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
                        int directionX = 0;
                        int directionY = 0;
                        if(rigidBody.velocity.x > 0) directionX = +1;
                        if(rigidBody.velocity.x < 0) directionX = -1;
                        if(rigidBody.velocity.y > 0) directionY = +1;
                        if(rigidBody.velocity.y < 0) directionY = -1;
                        projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
                        projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

                        Entity projectile = entity.registry->createEntity();
                        projectile.group("projectiles");
                        projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                        projectile.addComponent<RigidBodyComponent>(projectileVelocity);
                        projectile.addComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                        projectile.addComponent<BoxColliderComponent>(4, 4);
                        projectile.addComponent<ProjectileComponent>(
                            projectileEmitter.isFriendly,
                            projectileEmitter.hitPercentageDamage,
                            projectileEmitter.projectileDuration
                        );
                    }
                }
            }
        }

        void update(std::unique_ptr<Registry>& registry) {
            for (auto entity: getSystemEntities()) {
                auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
                const auto transform = entity.getComponent<TransformComponent>();

                // if the repeat is zero, bypass the re-emission logic
                if (projectileEmitter.repeatFrequency == 0) {
                    continue;
                }

                if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
                    glm::vec2 projectilePosition = transform.position;
                    if (entity.hasComponent<SpriteComponent>()) {
                        auto sprite = entity.getComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
                    if (entity.hasComponent<RigidBodyComponent>()) {
                        const auto rigidBody = entity.getComponent<RigidBodyComponent>();
                        int directionX = 0;
                        int directionY = 0;
                        if(rigidBody.velocity.x > 0) directionX = +1;
                        if(rigidBody.velocity.x < 0) directionX = -1;
                        if(rigidBody.velocity.y > 0) directionY = +1;
                        if(rigidBody.velocity.y < 0) directionY = -1;
                        projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
                        projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
                    }

                    Entity projectile = registry->createEntity();
                    projectile.group("projectiles");
                    projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                    projectile.addComponent<RigidBodyComponent>(projectileVelocity);
                    projectile.addComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                    projectile.addComponent<BoxColliderComponent>(4, 4);
                    projectile.addComponent<ProjectileComponent>(
                        projectileEmitter.isFriendly,
                        projectileEmitter.hitPercentageDamage,
                        projectileEmitter.projectileDuration
                    );
                    projectileEmitter.lastEmissionTime = SDL_GetTicks();
                }
            }
        }
};

#endif