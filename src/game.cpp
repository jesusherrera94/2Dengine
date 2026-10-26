#include <iostream>
#include <SDL2/SDL.h>
#include "game.h"
using namespace std;

void Game::setup() {
    // TODO: setup game objects
}

Game::Game() {
    isRunning = false;
    cout << "Game constructor called" << endl;
}

Game::~Game() {
    cout << "Game destructor called" << endl;
}

void Game::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr << "Error: failed to initialize SDL: " << SDL_GetError() << endl;
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
        cerr << "Error: failed to create window: " << SDL_GetError() << endl;
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
        cerr << "Error: failed to create renderer: " << SDL_GetError() << endl;
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
    // TODO: update game objects
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // call this to assign the color to the object
    SDL_Rect playrRect = { 10, 10, 20, 20 };
    SDL_RenderFillRect(renderer, &playrRect);

    SDL_RenderPresent(renderer);
}
