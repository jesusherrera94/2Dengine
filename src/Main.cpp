#include <iostream>
#include "./game/game.h"

int main(int argc, char* argv[]) {
   Game game;

   game.initialize();
   game.run();
   game.destroy();

    return 0;
}
