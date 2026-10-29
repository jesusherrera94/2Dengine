#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/transformComponent.h"
#include "../components/spriteComponent.h"
#include "../assetStore/assetStore.h"

class RenderSystem: public System {
    public:
        RenderSystem() {
            requiredComponent<TransformComponent>();
            requiredComponent<SpriteComponent>();
        }
        void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            // TODO: Implement movement logic here
            for (auto entity : getSystemEntities()) {
                TransformComponent transform = entity.getComponent<TransformComponent>();
                const SpriteComponent sprite = entity.getComponent<SpriteComponent>();

                // set the source rectangle of our original sprite texture
                SDL_Rect srcRect = sprite.srcRect;

                // set the destination rectangle with the X,Y position to be rendered
                SDL_Rect destRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.width * transform.scale.y),
                };

                // Draw the PNG texture in the screen
                SDL_RenderCopyEx(
                    renderer,
                    assetStore->getTexture(sprite.assetId),
                    &srcRect,
                    &destRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE
                );
            };
        }
};


#endif