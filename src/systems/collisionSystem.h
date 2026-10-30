#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/boxColliderComponent.h"
#include "../components/transformComponent.h"

class CollisionSystem: public System {
    public:
        CollisionSystem() {
            requiredComponent<BoxColliderComponent>();
            requiredComponent<TransformComponent>();
        }
        void update() {
            for (auto entity: getSystemEntities()) {
                if (!animation.hasFinished) {
                    // This ensures the frame be between 0 to the number of frames
                    animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameRateSpeed / 1000) % animation.numFrames;
                    // this will select which "block" will be displayed, if the animation is vertical, should use Y insted
                    sprite.srcRect.x = animation.currentFrame * sprite.width;
                    if (!animation.isLoop && animation.currentFrame == (animation.numFrames - 1)) {
                        animation.hasFinished = true;
                    }
                }

            }
        }
};

#endif