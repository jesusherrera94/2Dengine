#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENT = 32;

/*
* We use a bitset (1,0)'s to keep track of which components an entity has,
* and also helps keep track of which entities a system is interested in.
*/
typedef std::bitset<MAX_COMPONENT> Signature;

class Component {

};

class Entity {
    private:
        int id;
    public:
        Entity(int id): id(id) {};
        int getId() const;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* The system processes entities that contain a specific signature
*/

class System {
    private:
        Signature componentSignature;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;
        void addEntityToSystem(Entity entity);
        void removeEntityFromSystem(Entity entity);
        std::vector<Entity> getSystemEntities() const;
        Signature& getComponentSignature() const;
};

// Class that handles everything
class Registry {

};

#endif