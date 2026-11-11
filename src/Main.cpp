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
    // this is how easy we can gent the values from a lua Table
    bool isFullScreen = lua["config"]["fullscreen"];
    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];
    std::cout<<isFullScreen<< width << height <<endl;

}

int main(int argc, char* argv[]) {
//    Game game;

//    game.initialize();
//    game.run();
//    game.destroy();
    testLua();
    return 0;
}
