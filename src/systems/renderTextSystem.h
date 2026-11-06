#ifndef RENDER_TEXT_SYSTEM_H
#define RENDER_TEXT_SYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../ecs/ecs.h"
#include "../components/textLabelComponent.h"
#include "../assetStore/assetStore.h"

class RenderTextSystem: public System {
    public:
        RenderTextSystem() {
            requiredComponent<TextLabelComponent>();
        }

        void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
            for(auto entity: getSystemEntities()) {
                const auto textLabelComponent = entity.getComponent<TextLabelComponent>();
                assetStore->getFont(textLabelComponent.assetId);
                SDL_Surface* surface = TTF_RenderText_Blended(
                    assetStore->getFont(textLabelComponent.assetId),
                    textLabelComponent.text.c_str(),
                    textLabelComponent.color
                );
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                int labelWidth = 0;
                int labelHight = 0;

                // obtain the width and height of the font
                SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHight);

                SDL_Rect distRect = {
                    static_cast<int>(textLabelComponent.position.x - (textLabelComponent.isFixed ? 0 : camera.x)),
                    static_cast<int>(textLabelComponent.position.y - (textLabelComponent.isFixed ? 0 : camera.y)),
                    labelWidth,
                    labelHight
                };

                SDL_RenderCopy(renderer, texture, NULL, &distRect);
                SDL_DestroyTexture(texture);
            }
        }
};


#endif