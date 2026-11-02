#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <map>
#include <typeindex>
#include <list>

#include "event.h"
#include "../logger/logger.h"




class IEventCallback {
    private:
        virtual void call(Event& e) = 0;
    public:
        virtual ~IEventCallback() = default;
        void execute(Event& e) {
            call(e);
        }
};

template<typename TEvent, typename TOwner>
class EventCallback: public IEventCallback {
    private:
        typedef void(TOwner::*CallbackFunction)(TEvent&);
        TOwner* ownerInstance;
        CallbackFunction callbackFunction;
        virtual void call(Event& e) override {
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        }
    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
            this->ownerInstance = ownerInstance;
            this->callbackFunction = callbackFunction;
        }
        virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
    private:
        std::map<std::type_index, std::unique_ptr<HandlerList>> sibscribers;

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
        * Example: eventBus->subscribeToEvent<CollisionEvent>(this, &Game::onCollision); 
        */
       template<typename TEvent, typename TOwner>
        void subscribeToEvent(TOwner* ownerInstance, void(TOwner::*callbackFunction)(TEvent&)) {
            if (!subscribers[typeid(TEvent)].get()) {
                subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
            subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
        }

        /*
        * Emit an event of type <T>
        * In our implementation, as soon as something happens, emits an event
        * we go ahead and execute all the listeners callbacks
        * Example: eventBus->emitEvent<CollisionEvent>(...args); 
        */
       template<typename TEvent, typename ...TArgs>
        void emitEvent(TArgs&&... args) {
            auto handlers = subscribers[typeid(TEvent)].get();
            if (handlers) {
                for (auto it = handlers->begin(); it != handlers->end(); it++){
                    auto handler = it->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->execute(event);
                }
            }
        }
};


#endif