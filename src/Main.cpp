#include <sol/sol.hpp>
#include <iostream>
#include "./game/game.h"


int nativeCppCubeFunc(int n) {
    return n*n*n;
}

void testLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    // this is how we invoke a function from cpp to lua
    // this has to be defined before loading the script
    lua["cube"] = nativeCppCubeFunc;

    lua.script_file("./assets/scripts/myscript.lua");
    // read a variable form lua
    double someVariable = lua["some_variable"];
    std::cout<< someVariable << std::endl;
    // this is how easy we can gent the values from a lua Table
    bool isFullScreen = lua["config"]["fullscreen"];
    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];
    std::cout<<isFullScreen<< width << height <<std::endl;

    // invoking a lua fuction from c++
    sol::function functionFactorial = lua["factorial"];
    int result = functionFactorial(5);
    std::cout<<result<<std::endl;

}

int main(int argc, char* argv[]) {
//    Game game;

//    game.initialize();
//    game.run();
//    game.destroy();
    testLua();
    return 0;
}
