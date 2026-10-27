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

struct IComponent {
    protected:
        static int nextId;
};

/*
* Used to assign a unique id to a component type
*/
template<typename T>
class Component: public IComponent {
    /*
    * Returns the unique id of Component<T>
    */
    static int getId() {
        static auto id = nextId++;
        return id;
    }
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
        /*
        * Defines the component type that entities must have to be considered by the system
        */
        template <typename TComponent> void requiredComponent();
};

// Class that handles everything
class Registry {

};

template <typename TComponent>
void System::requiredComponent() {
    const auto componentId = Component<TComponent>::getId();
    componentSignature.set(componentId);
}

#endif