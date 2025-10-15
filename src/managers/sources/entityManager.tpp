#pragma once
#include "../entityManager.h"

template <class T>
void EntityManager::AddComponent(uint64_t entityId, const T& component) {
    ComponentPool<T>* pool = GetPool<T>();
    if (pool == nullptr) { return; }
    pool->Add(entityId, component);
}

template <class T>
T* EntityManager::GetComponent(uint64_t entityId) {
    ComponentPool<T>* pool = GetPool<T>();
    if (pool == nullptr) { return nullptr; }
    return pool->Get(entityId);
}

template <class T>
void EntityManager::DestroyComponent(uint64_t entityId) {
    ComponentPool<T>* pool = GetPool<T>();
    if (pool == nullptr) { return; }
    pool->Remove(entityId);
}

template <class T>
void EntityManager::RegisterComponentType() {
    size_t typeHash = typeid(T).hash_code();
    if (mComponentPools.find(typeHash) == mComponentPools.end()) {
        mComponentPools[typeHash] = std::make_unique<ComponentPool<T>>();
    }
}

template <class T>
ComponentPool<T>* EntityManager::GetPool() {
    size_t typeHash = typeid(T).hash_code();
    auto it = mComponentPools.find(typeHash);
    if (it == mComponentPools.end() || !it->second) { return nullptr; }

    IComponentPool* genericPool = it->second.get();
    return static_cast<ComponentPool<T>*>(genericPool);
}