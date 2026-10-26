#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
    private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    int millisecondsPreviousFrame = 0;

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