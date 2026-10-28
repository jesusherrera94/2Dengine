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
        std::vector<IPool*> componentPool;
        // Vector of components signatures.
        // The signature let us know which components are turned "on" for an entity
        // [vector index = entity id]
        std::vector<Signature> entityComponentSignature;
        // Map of active systems [index = system typeid]
        std::unordered_map<std::type_index, System*> systems;

        // Set of entities that are flagged to be added or removed on the next registry update;
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeRemoved;
    public:
        Registry() = default;
        void update();
        Entity createEntity();
        void addEntityToSystem(Entity entity);

};

template <typename TComponent>
void System::requiredComponent() {
    const auto componentId = Component<TComponent>::getId();
    componentSignature.set(componentId);
}

#endif