#ifndef PROJECTILE_EMITER_COMPONENT_H
#define PROJECTILE_EMITER_COMPONENT_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct ProjectileEmitterComponent {

    glm::vec2 projectileVelocity;
    int repeatFrequency;
    int projectileDuration;
    int hitPercentageDamage;
    bool isFriendly;
    int lastEmissionTime;

    ProjectileEmitterComponent(
        glm::vec2 projectileVelocity = glm::vec2(0,0),
        int repeatFrequency = 0,
        int projectileDuration = 100000,
        int hitPercentageDamage = 10,
        bool isFriendly = false
    ) {
        this->projectileVelocity = projectileVelocity;
        this->repeatFrequency = repeatFrequency;
        this->projectileDuration = projectileDuration;
        this->hitPercentageDamage = hitPercentageDamage;
        this->isFriendly = isFriendly;
        this->lastEmissionTime = SDL_GetTicks();
    }
};

#endif