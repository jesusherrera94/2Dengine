#ifndef DEBUG_COLLIDER_RECTANGLE_SYSTEM_H
#define DEBUG_COLLIDER_RECTANGLE_SYSTEM_H

#include <SDL2/SDL.h>
#include "../ecs/ecs.h"
#include "../components/boxColliderComponent.h"
#include "../components/transformComponent.h"

class DebugColliderRectangle: public System{
    public:
        DebugColliderRectangle() {
            requiredComponent<BoxColliderComponent>();
            requiredComponent<TransformComponent>();
        }
        
        void update(SDL_Renderer* renderer, const bool& isEnabled) {
            if (!isEnabled) {
                return;
            }
            for (auto entity: getSystemEntities()) {
                auto transform = entity.getComponent<TransformComponent>();
                auto boxCollider = entity.getComponent<BoxColliderComponent>();

                SDL_Rect outlineRect = {
                    static_cast<int>(transform.position.x + boxCollider.offset.x),
                    static_cast<int>(transform.position.y + boxCollider.offset.y),
                    boxCollider.width,
                    boxCollider.height
                };
                SDL_SetRenderDrawColor(renderer, 255, 0,0, 255);
                SDL_RenderDrawRect(renderer, &outlineRect);
            }

        }

};

#endif