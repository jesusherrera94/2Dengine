#include "levelLoader.h"
#include <fstream>
#include <sol/sol.hpp>
#include <string>

#include "../ecs/ecs.h"
#include "game.h"
#include "../components/transformComponent.h"
#include "../components/rigidBodyComponent.h"
#include "../components/spriteComponent.h"
#include "../components/animationComponent.h"
#include "../components/boxColliderComponent.h"
#include "../components/keyboardControlComponent.h"
#include "../components/cameraFollowComponent.h"
#include "../components/projectileEmitterComponent.h"
#include "../components/healthComponent.h"
#include "../components/textLabelComponent.h"

#include "../logger/logger.h"

void LevelLoader::loadMap(const std::unique_ptr<Registry>& registry, sol::table map) {
    std::string mapFilePath = map["map_file"];
    std::string mapTextureAssetId = map["texture_asset_id"];
    int tileSize = map["tile_size"];
    double tileScale = map["scale"];;
    int mapNumCols = map["num_cols"];
    int mapNumRows = map["num_rows"];
    std::fstream mapFile;
    mapFile.open(mapFilePath);

    for (int y=0; y< mapNumRows; y++) {
        for (int x=0; x<mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->createEntity();
            tile.group("tile");
            tile.addComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.addComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();

    Game::mapWidth = mapNumCols * tileSize * tileScale;
    Game::mapHeight = mapNumRows * tileSize * tileScale;

}


LevelLoader::LevelLoader(){
     Logger::Log("LevelLoader constructor called");
}
LevelLoader::~LevelLoader(){
    Logger::Log("LevelLoader destructor called");
}

void LevelLoader::loadLevel(
            sol::state& lua, 
            const std::unique_ptr<Registry>& registry,
            const std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer, 
            int levelNumber){
    
    sol::load_result script = lua.load_file("./assets/scripts/level" + std::to_string(levelNumber) + ".lua");
    // this checks the syntax of our script, but it does not execute the script
    if (!script.valid()) {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: "+errorMessage);
        return;
    }

    lua.script_file("./assets/scripts/level" + std::to_string(levelNumber) + ".lua");
    
    sol::table level = lua["Level"];

    // read the level assets
    sol::table assets = level["assets"];

    int i = 0;
    while (true) {
        sol::optional<sol::table> hasAsset = assets[i];
        if (hasAsset == sol::nullopt) {
            break;
        }
        sol::table asset = asset[i];
        std::string assetType = asset["type"];
        std::string assetId = asset["id"];
        if (assetType == "texture") {
            assetStore->addTexture(renderer, assetId, asset["file"]);
        } 
        if (assetType == "font") {
            assetStore->addFont(assetId, asset["file"], asset["font_size"]);
        }
        i++;
    }


    // load the tile map
    sol::table map = level["tilemap"];
    loadMap(registry, map);


    // Load the entities
    // Entity tank = registry->createEntity();
    // tank.group("enemies");
    
    // // add some components
    // // registry->addComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    // // registry->addComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
    // tank.addComponent<TransformComponent>(glm::vec2(773.0, 126.0), glm::vec2(1.0, 1.0), 0.0);
    // tank.addComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    // tank.addComponent<SpriteComponent>("tank-image", 32,32, 2);
    // tank.addComponent<BoxColliderComponent>(32, 32);
    // tank.addComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 1000, 5000, 10, false);
    // tank.addComponent<HealthComponent>(100);

    // Entity truck = registry->createEntity();
    // truck.group("enemies");
    // truck.addComponent<TransformComponent>(glm::vec2(460.0, 395.0), glm::vec2(1.0, 1.0), 0.0);
    // truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // truck.addComponent<SpriteComponent>("truck-image", 32,32, 1);
    // truck.addComponent<BoxColliderComponent>(32, 32);
    // truck.addComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 10, false);
    // truck.addComponent<HealthComponent>(100);
    


    // Entity choppper = registry->createEntity();
    // choppper.tag("player");
    // choppper.addComponent<TransformComponent>(glm::vec2(243.0, 115.0), glm::vec2(1.0, 1.0), 0.0);
    // choppper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // choppper.addComponent<SpriteComponent>("chopper-image", 32,32, 1);
    // choppper.addComponent<AnimationComponent>(2, 15, true);
    // choppper.addComponent<BoxColliderComponent>(32, 32);
    // choppper.addComponent<KeyboardControlComponent>(
    //         glm::vec2(0.0, -80.0),
    //         glm::vec2(80.0, 0.0),
    //         glm::vec2(0.0, 80.0),
    //         glm::vec2(-80.0, 0.0)
    //     );
    // choppper.addComponent<CameraFollowComponent>();
    // choppper.addComponent<HealthComponent>(100);
    // choppper.addComponent<ProjectileEmitterComponent>(
    //     glm::vec2(150.0, 150.0),
    //     0, // don't repeat only when space is pressed
    //     10000,
    //     10,
    //     true
    // );

    // Entity radar = registry->createEntity();
    // radar.addComponent<TransformComponent>(glm::vec2(Game::windowWidth - 184, 10.0), glm::vec2(1.0, 1.0), 0.0);
    // radar.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // radar.addComponent<SpriteComponent>("radar-image", 64, 64, 2, true);
    // radar.addComponent<AnimationComponent>(8, 5, false);

    // Entity title = registry->createEntity();
    // SDL_Color greenColor = {0,255,0};
    // title.addComponent<TextLabelComponent>(glm::vec2((Game::windowWidth / 2) - 120, 45), "Chopper Game 1.0", "charriot-font", greenColor, true);

    // Entity treeA = registry->createEntity();
    // treeA.group("obstacles");
    // treeA.addComponent<TransformComponent>(glm::vec2(873.0, 131.0), glm::vec2(1.0, 1.0), 0.0);
    // treeA.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // treeA.addComponent<SpriteComponent>("tree-image", 16,32, 2);
    // treeA.addComponent<BoxColliderComponent>(16, 32);

    // Entity treeB = registry->createEntity();
    // treeB.group("obstacles");
    // treeB.addComponent<TransformComponent>(glm::vec2(673.0, 131.0), glm::vec2(1.0, 1.0), 0.0);
    // treeB.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // treeB.addComponent<SpriteComponent>("tree-image", 16,32, 2);
    // treeB.addComponent<BoxColliderComponent>(16, 32);
}