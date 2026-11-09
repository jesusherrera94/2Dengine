#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl2.h>

#include "../ecs/ecs.h"
#include "game.h"
#include "../logger/logger.h"
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

#include "../systems/movementSystem.h"
#include "../systems/renderSystem.h"
#include "../systems/animationSystem.h"
#include "../systems/collisionSystem.h"
#include "../systems/debugColliderRectangle.h"
#include "../systems/damageSystem.h"
#include "../systems/keyboardMovementSystem.h"
#include "../systems/cameraMovementSystem.h"
#include "../systems/projectileEmitterSystem.h"
#include "../systems/projectileLifecycleSystem.h"
#include "../systems/renderTextSystem.h"
#include "../systems/renderHealthBarSystem.h"

#include "../events/keyPressedEvent.h"

using namespace std;

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

void Game::loadMap() {
    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 29;
    int mapNumRows = 30;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

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

    mapWidth = mapNumCols * tileSize * tileScale;
    mapHeight = mapNumRows * tileSize * tileScale;

}


void Game::loadLevel(int level) {
    // add system
    registry->addSystem<MovementSystem>();
    registry->addSystem<RenderSystem>();
    registry->addSystem<AnimationSystem>();
    registry->addSystem<CollisionSystem>();
    registry->addSystem<DebugColliderRectangle>();
    registry->addSystem<DamageSystem>();
    registry->addSystem<KeyboardMovementSystem>();
    registry->addSystem<CameraMovementSystem>();
    registry->addSystem<ProjectileEmitterSystem>();
    registry->addSystem<ProjectileLifeCycleSystem>();
    registry->addSystem<RenderTextSystem>();
    registry->addSystem<RenderHealthBarSystem>();

    // adding assets to the store
    assetStore->addTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->addTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->addTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
    assetStore->addTexture(renderer, "radar-image", "./assets/images/radar.png");
    assetStore->addTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->addTexture(renderer, "bullet-image", "./assets/images/bullet.png");
    assetStore->addFont("charriot-font", "./assets/fonts/charriot.ttf", 28);
    assetStore->addFont("pico8-font-5", "./assets/fonts/pico8.ttf", 5);
    assetStore->addFont("pico8-font-10", "./assets/fonts/pico8.ttf", 10);


    // load the tile map
    loadMap();


    // Create some entity
    Entity tank = registry->createEntity();
    tank.group("enemies");
    
    // add some components
    // registry->addComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    // registry->addComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
    tank.addComponent<TransformComponent>(glm::vec2(500.0, 250.0), glm::vec2(1.0, 1.0), 0.0);
    tank.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.addComponent<SpriteComponent>("tank-image", 32,32, 2);
    tank.addComponent<BoxColliderComponent>(32, 32);
    tank.addComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
    tank.addComponent<HealthComponent>(100);

    Entity truck = registry->createEntity();
    truck.group("enemies");
    truck.addComponent<TransformComponent>(glm::vec2(390.0, 250.0), glm::vec2(1.0, 1.0), 0.0);
    truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.addComponent<SpriteComponent>("truck-image", 32,32, 1);
    truck.addComponent<BoxColliderComponent>(32, 32);
    truck.addComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 10, false);
    truck.addComponent<HealthComponent>(100);
    


    Entity choppper = registry->createEntity();
    choppper.tag("player");
    choppper.addComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    choppper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    choppper.addComponent<SpriteComponent>("chopper-image", 32,32, 1);
    choppper.addComponent<AnimationComponent>(2, 15, true);
    choppper.addComponent<BoxColliderComponent>(32, 32);
    choppper.addComponent<KeyboardControlComponent>(
            glm::vec2(0.0, -80.0),
            glm::vec2(80.0, 0.0),
            glm::vec2(0.0, 80.0),
            glm::vec2(-80.0, 0.0)
        );
    choppper.addComponent<CameraFollowComponent>();
    choppper.addComponent<HealthComponent>(100);
    choppper.addComponent<ProjectileEmitterComponent>(
        glm::vec2(150.0, 150.0),
        0, // don't repeat only when space is pressed
        10000,
        10,
        true
    );

    Entity radar = registry->createEntity();
    radar.addComponent<TransformComponent>(glm::vec2(windowWidth - 184, 10.0), glm::vec2(1.0, 1.0), 0.0);
    radar.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.addComponent<SpriteComponent>("radar-image", 64, 64, 2, true);
    radar.addComponent<AnimationComponent>(8, 5, false);

    Entity title = registry->createEntity();
    SDL_Color greenColor = {0,255,0};
    title.addComponent<TextLabelComponent>(glm::vec2((windowWidth / 2) - 120, 45), "Chopper Game 1.0", "charriot-font", greenColor, true);

}



void Game::setup() {
    loadLevel(1);
}

Game::Game() {
    isRunning = false;
    registry = std::make_unique<Registry>(); // intanciate a registry object
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
    Logger::Log("Game constructor called");
}

Game::~Game() {
    Logger::Log("Game destructor called");
}

void Game::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error: failed to initialize SDL: " + std::string(SDL_GetError()));
        return;
    };
    if (TTF_Init() != 0){
        Logger::Err("Error: failed to initialize TTF: " + std::string(SDL_GetError()));
        return;
    }
    // Getting the current display mode to set the window size to the display size
    // Fake fullscreen
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;

    window = SDL_CreateWindow(
        NULL,
        //position: x, y
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        //size: Width, height,
        windowWidth, windowHeight,
        // flags
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        Logger::Err("Error: failed to create window: " + std::string(SDL_GetError()));
        return;
    }

    renderer = SDL_CreateRenderer(
        // window object
        window,
        // index of the rendering driver to initialize, -1 means first one supporting the requested flags
        -1,
        // flags
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        Logger::Err("Error: failed to create renderer: " + std::string(SDL_GetError()));
        return;
    }

    // real fullscreen
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // initialize imgui context
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    isRunning = true;
}

void Game::run() {
    setup();
    while (isRunning) {
        processInput();
        update();
        render();
    }
}

void Game::destroy() {
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::processInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        // ImGui SDL input
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        //handling core SDL events
        switch (sdlEvent.type) {
            case SDL_QUIT: // the user clicked the close button on the window
                isRunning = false;
                break;
            case SDL_KEYDOWN:

                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    isDebugEnabled = !isDebugEnabled;
                }
                // trigger a keyboard event
                eventBus->emitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
                break;
            default:
                break;
        }
    }
}

void Game::update() {
    // TODO: If the update is running too fast
    // waste thime until we reach the MILLISECONDS_PER_FRAME
    Uint32 timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
    if (timeToWait > 0  && timeToWait <= MILLISECONDS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }   

    // the difference in ticks since the last frame, converted to seconds.
    double deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;

    // store the current frame time
    millisecondsPreviousFrame = SDL_GetTicks();

    // Resset all event handlers for the current frame
    eventBus->reset();

    // Perform the subscription of the events for all systems
    registry->getSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->getSystem<KeyboardMovementSystem>().subscribeToEvents(eventBus);
    registry->getSystem<ProjectileEmitterSystem>().subscribeToEvents(eventBus);


   // Update the registry to process the entities that are waiting to be create or deleted
   registry->update();

   // Invoke all the systems that need to update
   registry->getSystem<MovementSystem>().update(deltaTime);
   registry->getSystem<AnimationSystem>().update();
   registry->getSystem<CollisionSystem>().update(eventBus);
   registry->getSystem<ProjectileEmitterSystem>().update(registry);
   registry->getSystem<CameraMovementSystem>().update(camera);
   registry->getSystem<ProjectileLifeCycleSystem>().update();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

   // Invoke all the system that need to render
   registry->getSystem<RenderSystem>().update(renderer, assetStore, camera);
   registry->getSystem<DebugColliderRectangle>().update(renderer, isDebugEnabled, camera);
   if (isDebugEnabled) {
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
   }
   registry->getSystem<RenderTextSystem>().update(renderer, assetStore, camera);
   registry->getSystem<RenderHealthBarSystem>().update(renderer, assetStore, camera);

    SDL_RenderPresent(renderer);
}
