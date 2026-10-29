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

                SDL_Rect obRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    sprite.width,
                    sprite.height
                };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &obRect);

            };
        }
};


#endif