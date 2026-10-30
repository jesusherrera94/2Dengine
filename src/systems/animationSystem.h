#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/spriteComponent.h"
#include "../components/animationComponent.h"

class AnimationSystem: public System {
    public:
        AnimationSystem() {
            requiredComponent<SpriteComponent>();
            requiredComponent<AnimationComponent>();
        }
        void update() {
            for (auto entity: getSystemEntities()) {
                AnimationComponent& animation = entity.getComponent<AnimationComponent>();
                SpriteComponent& sprite = entity.getComponent<SpriteComponent>();
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