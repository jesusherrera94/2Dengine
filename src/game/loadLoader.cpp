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

void LevelLoader::loadMap(const std::unique_ptr<Registry>& registry, sol::table& map) {
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
            tile.addComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();

    Game::mapWidth = mapNumCols * tileSize * tileScale;
    Game::mapHeight = mapNumRows * tileSize * tileScale;

}

void LevelLoader::loadEntities(const std::unique_ptr<Registry>& registry, const sol::table& entities) {
    int i = 0;
    while (true) {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt) {
            break;
        }
        sol::table entity = entities[i];

        Entity newEntity = registry->createEntity();

        sol::optional<std::string> tag = entity["tag"];
        if (tag != sol::nullopt) {
            newEntity.tag(entity["tag"]);
        }

        sol::optional<std::string> group = entity["group"];
        if (group != sol::nullopt) {
            newEntity.group(entity["group"]);
        }

        sol::optional<sol::table> hasComponents = entity["components"];
        if (hasComponents != sol::nullopt) {
            // transform
            sol::optional<sol::table> transform = entity["components"]["transform"];
            if (transform != sol::nullopt) {
                newEntity.addComponent<TransformComponent>(
                    glm::vec2(
                        entity["components"]["transform"]["position"]["x"],
                        entity["components"]["transform"]["position"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["transform"]["scale"]["x"].get_or(0.0),
                        entity["components"]["transform"]["scale"]["y"].get_or(0.0)
                    ),
                    entity["components"]["transform"]["rotation"].get_or(0.0)
                );
            }
            // rigid body
            sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
            if (rigidbody != sol::nullopt) {
                newEntity.addComponent<RigidBodyComponent>(glm::vec2(
                    entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                    entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                ));
            }
        }
        // Sprite
            sol::optional<sol::table> sprite = entity["components"]["sprite"];
            if (sprite != sol::nullopt) {
                newEntity.addComponent<SpriteComponent>(
                    entity["components"]["sprite"]["texture_asset_id"],
                    entity["components"]["sprite"]["width"],
                    entity["components"]["sprite"]["height"],
                    entity["components"]["sprite"]["z_index"].get_or(1),
                    entity["components"]["sprite"]["fixed"].get_or(false),
                    entity["components"]["sprite"]["src_rect_x"].get_or(0),
                    entity["components"]["sprite"]["src_rect_y"].get_or(0)
                );
            }

            // Animation
            sol::optional<sol::table> animation = entity["components"]["animation"];
            if (animation != sol::nullopt) {
                newEntity.addComponent<AnimationComponent>(
                    entity["components"]["animation"]["num_frames"].get_or(1),
                    entity["components"]["animation"]["speed_rate"].get_or(1)
                );
            }

            // BoxCollider
            sol::optional<sol::table> collider = entity["components"]["boxcollider"];
            if (collider != sol::nullopt) {
                newEntity.addComponent<BoxColliderComponent>(
                    entity["components"]["boxcollider"]["width"],
                    entity["components"]["boxcollider"]["height"],
                    glm::vec2(
                        entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                        entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
                    )
                );
            }
            
            // Health
            sol::optional<sol::table> health = entity["components"]["health"];
            if (health != sol::nullopt) {
                newEntity.addComponent<HealthComponent>(
                    static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
                );
            }
            
            // ProjectileEmitter
            sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
            if (projectileEmitter != sol::nullopt) {
                newEntity.addComponent<ProjectileEmitterComponent>(
                    glm::vec2(
                        entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                        entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
                    ),
                    static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
                    static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
                    static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
                    entity["components"]["projectile_emitter"]["friendly"].get_or(false)
                );
            }

            // CameraFollow
            sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
            if (cameraFollow != sol::nullopt) {
                newEntity.addComponent<CameraFollowComponent>();
            }

            // KeyboardControlled
            sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
            if (keyboardControlled != sol::nullopt) {
                newEntity.addComponent<KeyboardControlComponent>(
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["up_velocity"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["right_velocity"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["down_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["down_velocity"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["left_velocity"]["y"]
                    )
                );
            }
        
        i++;
    }
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
        sol::table asset = assets[i];
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
    sol::table entities = level["entities"];
    loadEntities(registry, entities);
}