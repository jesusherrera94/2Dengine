#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../eventBus/eventBus.h"
#include "../events/keyPressedEvent.h"


#include "../logger/logger.h"

class KeyboardMovementSystem: public System {
    public: 
        KeyboardMovementSystem() {}

        void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::onKeyPressed);
        }
        void onKeyPressed(KeyPressedEvent& event) {
            const char* keyName = SDL_GetKeyName(event.keyCode);
            std::string keyString(keyName);
            Logger::Log("Keyboard with letter:" + keyString + " was pressed");
        }

        void update() {
        }
};

#endif