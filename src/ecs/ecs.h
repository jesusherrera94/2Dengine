#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <set>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
        Entity(const Entity& entity) = default;
        int getId() const;
        // operator overloading function
        Entity& operator = (const Entity& other) = default;
        bool operator == (const Entity& other) const { return id == other.id; }
        bool operator != (const Entity& other) const {return id != other.id; }
        bool operator > (const Entity& other) const {return id > other.id; }
        bool operator < (const Entity& other) const {return id < other.id; }
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
       const Signature& getComponentSignature() const;
        /*
        * Defines the component type that entities must have to be considered by the system
        */
        template <typename TComponent> void requiredComponent();
};

class IPool {
    public:
        virtual ~IPool(){}
};

/**
 * Pool is just a vector (continous data) ob objects of type T
 */
 template <typename T>
 class Pool: IPool{
    private:
        std::vector<T> data;
    public:
        Pool(int size=100){
            data.resize(size);
        }
        virtual ~Pool() = default;
        bool isEmpty() const {
            return data.empty();
        }
        int getSize() const {
            return data.size();
        }
        void resize(int n) {
            data.resize(n);
        }
        void clear() {
            data.clear();
        }
        void add(T object) {
            data.push_back(object);
        }
        void set(int index, T object) {
            data[index] = object;
        }
        T& get(int index) {
            return static_cast<T&>(data[index]);
        }
        T& operator [] (unsigned int index) {
            return data[index];
        }
 };


/*
* Registry:
* <Manager, World, Coordinator, Orchastor> Manages the creation and destruction of entities, as well as
* adding systems and adding components to entities
*/
class Registry {
    private:
        int numEntities = 0;
        // Vector component tools, each pool contains all the data for a certain component type
        // [Vector index = component type id]
        // [Pool index = entity id]
        std::vector<std::shared_ptr<IPool>> componentPools;
        // Vector of components signatures.
        // The signature let us know which components are turned "on" for an entity
        // [vector index = entity id]
        std::vector<Signature> entityComponentSignatures;
        // Map of active systems [index = system typeid]
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

        // Set of entities that are flagged to be added or removed on the next registry update;
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeRemoved;
    public:
        Registry() = default;
        ~Registry() = default;
        void update();

        // Entity management
        Entity createEntity();

        // Checks the component signature of an entity and add the entity to the systems
        // that are interested in it.
        void addEntityToSystems(Entity entity);

        // component management
        template<typename TComponent, typename ...TArgs> void addComponent(Entity entity, TArgs&& ...args);
        template<typename TComponent> void removeComponent(Entity entity);
        template<typename TComponent> bool hasComponent(Entity entity) const;
    
        // System management
        template<typename TSystem, typename ...TArgs> void addSystem(TArgs&& ...args);
        template<typename TSystem> void removeSystem();
        template<typename TSystem> bool hasSystem() const;
        template<typename TSystem> TSystem& getSystem() const;

};

template <typename TComponent>
void System::requiredComponent() {
    const auto componentId = Component<TComponent>::getId();
    componentSignature.set(componentId);
}

// component management
template<typename TComponent, typename ...TArgs>
void Registry::addComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }
    // get the pool of component values for that component type
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    if(entityId >= componentPool->getSize()) {
        componentPool->resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::removeComponent(Entity entity) {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();
    entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::hasComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();
    return entityComponentSignatures[entityId].test(componentId);
}

// System management
template<typename TSystem, typename ...TArgs>
void Registry::addSystem(TArgs&& ...args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));

}

template<typename TSystem>
void Registry::removeSystem() {
    auto systemKey = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(systemKey);
}

template<typename TSystem>
bool Registry::hasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& Registry::getSystem() const {
    auto systemKey = systems.find(std::type_index(typeid(TSystem)));
    // converts the systemsKey that has the key value pair in 
    // the first -> key , second -> value
    return *(std::static_pointer_cast<TSystem>(systemKey->second));
}

#endif
