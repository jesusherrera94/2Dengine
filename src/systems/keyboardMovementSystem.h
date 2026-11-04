#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include <SDL2/SDL.h>

#include "../ecs/ecs.h"
#include "../eventBus/eventBus.h"
#include "../events/keyPressedEvent.h"
#include "../components/keyboardControlComponent.h"
#include "../components/spriteComponent.h"
#include "../components/rigidBodyComponent.h"


// #include "../logger/logger.h"

class KeyboardMovementSystem: public System {
    public: 
        KeyboardMovementSystem() {
            requiredComponent<KeyboardControlComponent>();
            requiredComponent<SpriteComponent>();
            requiredComponent<RigidBodyComponent>();
        }

        void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::onKeyPressed);
        }
        void onKeyPressed(KeyPressedEvent& event) {
            // change the sprite an the velocity of my entity
            for (auto entity: getSystemEntities()) {
                const auto keyboardControlComponent = entity.getComponent<KeyboardControlComponent>();
                auto& spriteComponent = entity.getComponent<SpriteComponent>();
                auto& rigidBodyComponent = entity.getComponent<RigidBodyComponent>();
                switch (event.keyCode)
                {
                    case SDLK_UP:
                        rigidBodyComponent.velocity = keyboardControlComponent.upVelocity;
                        spriteComponent.srcRect.y = spriteComponent.height * 0;
                        break;
                    case SDLK_RIGHT:
                        rigidBodyComponent.velocity = keyboardControlComponent.rightVelocity;
                        spriteComponent.srcRect.y = spriteComponent.height * 1;
                        break;
                    case SDLK_DOWN:
                        rigidBodyComponent.velocity = keyboardControlComponent.downVelocity;
                        spriteComponent.srcRect.y = spriteComponent.height * 2;
                        break;
                    case SDLK_LEFT:
                        rigidBodyComponent.velocity = keyboardControlComponent.leftVelocity;
                        spriteComponent.srcRect.y = spriteComponent.height * 3;
                        break;              
                    default:
                        break;
                }
            }
        }

        void update() {
        }
};

#endif