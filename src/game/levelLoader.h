#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <SDL2/SDL.h>
#include <memory>

#include "../ecs/ecs.h"
#include "../assetStore/assetStore.h"

class LevelLoader {
    private:
        void loadMap(const std::unique_ptr<Registry>& registry);
    public: 
        LevelLoader();
        ~LevelLoader();

        void loadLevel(
            const std::unique_ptr<Registry>& registry,
            const std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer, 
            int level);
};


#endif