#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <SDL2/SDL.h>
#include <memory>
#include <sol/sol.hpp>

#include "../ecs/ecs.h"
#include "../assetStore/assetStore.h"

class LevelLoader {
    private:
        void loadMap(const std::unique_ptr<Registry>& registry, sol::table& map);
        void loadEntities(const std::unique_ptr<Registry>& registry, const sol::table& entities);
    public: 
        LevelLoader();
        ~LevelLoader();

        void loadLevel(
            sol::state& lua,
            const std::unique_ptr<Registry>& registry,
            const std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer, 
            int level);
};


#endif