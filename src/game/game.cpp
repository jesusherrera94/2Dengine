#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

#include "../ecs/ecs.h"
#include "game.h"

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
#include "../systems/renderGUISystem.h"

#include "../events/keyPressedEvent.h"
#include "levelLoader.h"

#include "../logger/logger.h"

using namespace std;

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;



void Game::setup() {
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
    registry->addSystem<RenderGUISystem>();

    LevelLoader loader;
    loader.loadLevel(registry, assetStore, renderer, 1);
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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

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
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
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
                if (sdlEvent.key.keysym.sym == SDLK_F1) {
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
    registry->getSystem<MovementSystem>().SubscribeToEvents(eventBus);
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
       registry->getSystem<RenderGUISystem>().update(renderer, registry, camera);
   }
   registry->getSystem<RenderTextSystem>().update(renderer, assetStore, camera);
   registry->getSystem<RenderHealthBarSystem>().update(renderer, assetStore, camera);

    SDL_RenderPresent(renderer);
}
