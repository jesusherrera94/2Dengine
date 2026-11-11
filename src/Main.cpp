#include <sol/sol.hpp>
#include <iostream>
#include "./game/game.h"

void testLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/myscript.lua");
    // read a variable form lua
    double someVariable = lua["some_variable"];
    std::cout<< someVariable << endl;
}

int main(int argc, char* argv[]) {
//    Game game;

//    game.initialize();
//    game.run();
//    game.destroy();
    testLua();
    return 0;
}
