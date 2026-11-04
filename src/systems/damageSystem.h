#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/boxColliderComponent.h"
#include "../eventBus/eventBus.h"
#include "../events/collisionEvent.h"
#include "../components/projectileComponent.h"
#include "../components/healthComponent.h"

#include "../logger/logger.h"

class DamageSystem: public System {
    private:
        void onProjectileHitsPlayer(Entity projectile, Entity player) {
            auto projectileComponent = projectile.getComponent<ProjectileComponent>();
            if (!projectileComponent.isFriendly) {
                auto& healthComponent = player.getComponent<HealthComponent>();
                healthComponent.healthPercentage -= projectileComponent.hitPercentDamage;
                if (healthComponent.healthPercentage <= 0) {
                    player.kill();
                }
                projectile.kill();
            }
        }

        void onProjectileHitsEnemy(Entity projectile, Entity enemy) {
            auto projectileComponent = projectile.getComponent<ProjectileComponent>();
            if (projectileComponent.isFriendly) {
                auto& healthComponent = enemy.getComponent<HealthComponent>();
                healthComponent.healthPercentage -= projectileComponent.hitPercentDamage;
                if (healthComponent.healthPercentage <= 0) {
                    enemy.kill();
                }
                projectile.kill();
            }
        }

    public:
        DamageSystem() {
            requiredComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        }

        void onCollision(CollisionEvent& event) {
            Entity b = event.b;
            Entity a = event.a;
            Logger::Log("The damage system recieved an event collision between entities " + std::to_string(a.getId()) + " and " + std::to_string(b.getId()));

            if (a.belongsToGroup("projectiles") && b.hasTag("player")) {
                onProjectileHitsPlayer(a,b);
            }
            if (b.belongsToGroup("projectiles") && a.hasTag("player")) {
                onProjectileHitsPlayer(b,a);
            }
            if (a.belongsToGroup("projectiles") && b.belongsToGroup("enemies")) {
                onProjectileHitsEnemy(a, b);
            }
            if (b.belongsToGroup("projectiles") && a.belongsToGroup("enemies")) {
                onProjectileHitsEnemy(b, a);
            }

        }

        void update() {
            // TODO:
        }
};

#endif