#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/boxColliderComponent.h"
#include "../eventBus/eventBus.h"
#include "../events/collisionEvent.h"

#include "../logger/logger.h"

class DamageSystem: public System {
    public:
        DamageSystem() {
            requiredComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        }

        void onCollision(CollisionEvent& event) {
            Logger::Log("The damage system recieved an event collision between entities " + std::to_string(event.a.getId()) + " and " + std::to_string(event.b.getId()));
            event.a.kill();
            event.b.kill();
        }

        void update() {
            // TODO:
        }
};

#endif