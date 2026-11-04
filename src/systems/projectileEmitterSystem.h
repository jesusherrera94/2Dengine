#ifndef PROJECTILE_EMITER_SYSTEM_H
#define PROJECTILE_EMITER_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/projectileEmitterComponent.h"
#include "../components/transformComponent.h"
#include "../components/rigidBodyComponent.h"
#include "../components/spriteComponent.h"
#include "../components/boxColliderComponent.h"


class ProjectileEmitterSystem: public System {
    public:
        ProjectileEmitterSystem() {
            requiredComponent<ProjectileEmitterComponent>();
            requiredComponent<TransformComponent>();
        }
        void update(std::unique_ptr<Registry>& registry) {
            for (auto entity: getSystemEntities()) {
                auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
                const auto transform = entity.getComponent<TransformComponent>();

                if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
                    glm::vec2 projectilePosition = transform.position;
                    if (entity.hasComponent<SpriteComponent>()) {
                        auto sprite = entity.getComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }
                    Entity projectile = registry->createEntity();
                    projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                    projectile.addComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                    projectile.addComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                    projectile.addComponent<BoxColliderComponent>(4, 4);
                    projectileEmitter.lastEmissionTime = SDL_GetTicks();
                }
            }
        }
};

#endif