#include "game.h"
#include <iostream>
using namespace std;

Game::Game() {
    cout << "Game constructor called" << endl;
}

Game::~Game() {
    cout << "Game destructor called" << endl;
}

void Game::initialize() {

}

void Game::run() {
    while (true) {
        processInput();
        update();
        render();
    }
}

void Game::destroy() {

}

void Game::processInput() {

}

void Game::update() {

}

void Game::render() {

}
