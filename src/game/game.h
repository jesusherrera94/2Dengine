#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ecs/ecs.h"
#include "../assetStore/assetStore.h"
#include "../eventBus/eventBus.h"
#include <sol/sol.hpp>

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
    private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 millisecondsPreviousFrame = 0;
    bool isDebugEnabled = false;
    SDL_Rect camera;
    sol::state lua;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

    public:
        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;
        
        void setup();
        Game();
        ~Game();
        void initialize();
        void run();
        void destroy();
        // game loop methods
        void processInput();
        void update();
        void render();
        
};

#endif