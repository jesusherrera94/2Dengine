#ifndef GAME_H
#define GAME_H

class Game {
    private:

    public:
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