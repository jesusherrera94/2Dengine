#ifndef KEY_PRESSED_EVENT_H
#define KEY_PRESSED_EVENT_H

#include <SDL2/SDL.h>
#include "../eventBus/event.h"


class KeyPressedEvent: public Event {
    public:
        SDL_Keycode keyCode;
        KeyPressedEvent(SDL_Keycode keyCode): keyCode(keyCode) {};
};

#endif