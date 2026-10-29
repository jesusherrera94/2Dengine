#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/transformComponent.h"
#include "../components/spriteComponent.h"

class RenderSystem: public System {
    public:
        RenderSystem() {
            requiredComponent<TransformComponent>();
            requiredComponent<SpriteComponent>();
        }
        void update(SDL_Renderer* renderer) {
            // TODO: Implement movement logic here
            for (auto entity : getSystemEntities()) {
                TransformComponent transform = entity.getComponent<TransformComponent>();
                const SpriteComponent sprite = entity.getComponent<SpriteComponent>();

                // Draw the PNG texture in the screen

            };
        }
};


#endif