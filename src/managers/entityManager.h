#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <optional>
#include "../ecs/iComponentPool.h"
#include "../ecs/componentPool.h"
#include "../ecs/components/transform.h"
#include "../ecs/components/sprite.h"
#include "../ecs/components/controller.h"
#include "../ecs/components/abstractions/component.h"

class Application;

class EntityManager {
public:
    static void Initialize(Application* app);
    static void Update(float deltaTime);
    static void Shutdown();

    static uint64_t CreateEntity();
    static uint64_t CreateEntity(uint64_t parentEntityId);
    static void DestroyEntity(uint64_t entityId);

    static std::optional<uint64_t> GetParentEntity(uint64_t entityId);
    static std::set<uint64_t>* GetChildrenEntities(uint64_t entityId);

    template <class T>
    static void AddComponent(uint64_t entityId, const T& component);
    template <class T>
    static T* GetComponent(uint64_t entityId);
    template <class T>
    static void DestroyComponent(uint64_t entityId);

private:
    static Application* mApp;
    static uint64_t mEntityCount;
    static std::unordered_map<uint64_t, uint64_t> mEntityParentMap;
    static std::unordered_map<uint64_t, std::set<uint64_t>> mEntityChildMap;

    static std::unordered_set<uint64_t> mDestroyBuffer;
    static std::unordered_map<size_t, std::unique_ptr<IComponentPool>> mComponentPools;

    static void ClearBuffer();

    template <class T>
    static void RegisterComponentType();
    template <class T>
    static ComponentPool<T>* GetPool();
};

#include "sources/entityManager.tpp"