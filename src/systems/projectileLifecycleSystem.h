#ifndef PROJECTILE_LIFECYCLE_SYSTEM_H
#define PROJECTILE_LIFECYCLE_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/projectileComponent.h"

class ProjectileLifeCycleSystem: public System {
    public:
        ProjectileLifeCycleSystem() {
            requiredComponent<ProjectComponent>();
        }

        void update() {
            for(auto entity: getSystemEntities()) {
                auto projectileComponent = entity.getComponent<ProjectComponent>();
                if (SDL_GetTicks() - projectileComponent.startTime > projectileComponent.duration) {
                    entity.kill();
                }
            }
        }
};

#endif