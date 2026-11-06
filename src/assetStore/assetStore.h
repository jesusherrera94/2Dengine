#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AssetStore {
    private:
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
        // TODO: create a map for audio
    public:
        AssetStore();
        ~AssetStore();
        void clearAssets();

        
        void addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
        SDL_Texture* getTexture(const std::string& assetId);

        void addFont(const std::string& assetId, const std::string& filePath, int fontSize);
        TTF_Font* getFont(const std::string& assetId);
};


#endif