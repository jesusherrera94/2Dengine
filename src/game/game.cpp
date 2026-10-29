#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include "../ecs/ecs.h"
#include "game.h"
#include "../logger/logger.h"
#include "../components/transformComponent.h"
#include "../components/rigidBodyComponent.h"
#include "../components/spriteComponent.h"
#include "../systems/movementSystem.h"
#include "../systems/renderSystem.h"

using namespace std;



void Game::setup() {
    // add system
    registry->addSystem<MovementSystem>();
    registry->addSystem<RenderSystem>();

    // adding assets to the store
    assetStore->addTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->addTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

    // Create some entity
    Entity tank = registry->createEntity();
    
    // add some components
    // registry->addComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    // registry->addComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
    tank.addComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    tank.addComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
    tank.addComponent<SpriteComponent>("tank-image", 32,32);

    Entity truck = registry->createEntity();
    truck.addComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0));
    truck.addComponent<SpriteComponent>("truck-image", 32,32);
}

Game::Game() {
    isRunning = false;
    registry = std::make_unique<Registry>(); // intanciate a registry object
    assetStore = std::make_unique<AssetStore>();
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
    // Getting the current display mode to set the window size to the display size
    // Fake fullscreen
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800; // displayMode.w;
    windowHeight = 600; // displayMode.h;

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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::processInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT: // the user clicked the close button on the window
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
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


   // Update the registry to process the entities that are waiting to be create or deleted
   registry->update();

   // Invoke all the systems that need to update
   registry->getSystem<MovementSystem>().update(deltaTime);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

   // Invoke all the system that need to render
   registry->getSystem<RenderSystem>().update(renderer, assetStore);

    SDL_RenderPresent(renderer);
}
