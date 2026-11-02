#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "../logger/logger.h"

class EventBus {
    public:
        EventBus() {
            Logger::Log("EventBus constructor called!");
        }
        ~EventBus() {
            Logger::Log("EventBus destructor called!");
        }

        /*
        * Subscribe to an event type
        * In our implementation, a listener  subscribes to an event
        * Example: eventBus->subscribeToEvent<CollisionEvent>(&Game::onCollision); 
        */
        void subscribeToEvent<>() {}

        /*
        * Emit an event of type <T>
        * In our implementation, as soon as something happens, emits an event
        * we go ahead and execute all the listeners callbacks
        * Example: eventBus->emitEvent<CollisionEvent>(...args); 
        */
        void emitEvent<>() {
            
        }
};


#endif