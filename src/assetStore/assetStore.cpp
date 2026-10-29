#include "assetStore.h"
#include "../logger/logger.h"
#include <SDL2/SDL_image.h>

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructore called!");
}


AssetStore::~AssetStore() {
    clearAssets();
    Logger::Log("AssetStore destructor called!");
}

void AssetStore::clearAssets() {
    // First destroy the elements in the memory, then empty the map
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetStore::addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    // add the texture to the map
    textures.emplace(assetId, texture);
}

SDL_Texture* AssetStore::getTexture(const std::string& assetId) {
    // Todo: validate if the element exits before returning
    return textures[assetId];
}