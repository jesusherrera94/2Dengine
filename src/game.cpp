#include <iostream>
#include <SDL2/SDL.h>
#include "game.h"
using namespace std;

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
        cerr << "Error: failed to create window: " << SDL_GetError() << endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!window) {
        cerr << "Error: failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}

void Game::run() {
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
    SDL_SetRenderDrawColor(renderer, 255, 64, 32, 255);
    SDL_RenderClear(renderer);

    // TODO: render game objects...

    SDL_RenderPresent(renderer);
}
