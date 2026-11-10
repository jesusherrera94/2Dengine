#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../components/transformComponent.h"
#include "../components/spriteComponent.h"
#include "../assetStore/assetStore.h"
#include "../logger/logger.h"

class RenderSystem: public System {
    public:
        RenderSystem() {
            requiredComponent<TransformComponent>();
            requiredComponent<SpriteComponent>();
        }
        void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {

            // sort all the entities of our system by z-index
            std::vector<Entity> entities = getSystemEntities();

            // remove all the entities that are outside the camera view
            entities.erase(
                std::remove_if(entities.begin(), entities.end(), [&camera](Entity& entity) {
                    const auto transformComponent = entity.getComponent<TransformComponent>();
                    const auto spriteComponent = entity.getComponent<SpriteComponent>();

                    bool isEntityOutsideCameraView = (
                        transformComponent.position.x + ( transformComponent.scale.x * spriteComponent.width) < camera.x ||
                        transformComponent.position.x  > camera.x  + camera.w ||
                        transformComponent.position.y + ( transformComponent.scale.y * spriteComponent.height) < camera.y ||
                        transformComponent.position.y  > camera.y  + camera.h
                    );
                    return isEntityOutsideCameraView && !spriteComponent.isFixed;
                }),
                entities.end()
            );

            std::sort(entities.begin(), entities.end(), [](Entity& a, Entity& b) {
                auto spriteA = a.getComponent<SpriteComponent>();
                auto spriteB = b.getComponent<SpriteComponent>();
                return spriteA.zIndex < spriteB.zIndex;
            });

            // TODO: Implement movement logic here
            for (auto entity : entities) {
                TransformComponent transform = entity.getComponent<TransformComponent>();
                const SpriteComponent sprite = entity.getComponent<SpriteComponent>();

                // set the source rectangle of our original sprite texture
                SDL_Rect srcRect = sprite.srcRect;

                // set the destination rectangle with the X,Y position to be rendered
                SDL_Rect destRect = {
                    static_cast<int>(transform.position.x - (sprite.isFixed? 0: camera.x)),
                    static_cast<int>(transform.position.y - (sprite.isFixed? 0: camera.y)),
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
                    sprite.flip
                );
            };
        }
};


#endif