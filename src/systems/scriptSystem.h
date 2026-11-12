#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "../ecs/ecs.h"
#include "../components/scriptComponent.h"

class ScriptSystem: public System {
    public:
        ScriptSystem() {
            requiredComponent<ScriptComponent>();
        }
        void update() {
            for(auto entity: getSystemEntities()) {
                auto script = entity.getComponent<ScriptComponent>();
                script.func(); // execute the lua function
            }
        }
};

#endif