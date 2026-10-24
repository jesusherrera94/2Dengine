#include <iostream>
#include <SDL2/SDL.h>
#include "game.h"
using namespace std;

Game::Game() {
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

    SDL_Window* window = SDL_CreateWindow(
        NULL,
        //position: x, y
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        //size: Width, height,
        800, 600,
        // flags
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        cerr << "Error: failed to create window: " << SDL_GetError() << endl;
        return;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!window) {
        cerr << "Error: failed to create renderer: " << SDL_GetError() << endl;
        return;
    }
}

void Game::run() {
    // while (true) {
    //     processInput();
    //     update();
    //     render();
    // }
}

void Game::destroy() {

}

void Game::processInput() {

}

void Game::update() {

}

void Game::render() {

}
