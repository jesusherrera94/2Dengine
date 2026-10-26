#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Game {
    private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    public:
        int windowWidth;
        int windowHeight;
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