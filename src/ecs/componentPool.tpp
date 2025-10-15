#pragma once
#include "componentPool.h"

template <class T>
void ComponentPool<T>::Add(uint64_t entityId, const Component& componentData) {
    size_t typeHash = typeid(T).hash_code();
    if (typeHash != typeid(componentData).hash_code()) return;

    const T& component = static_cast<const T&>(componentData);
    Add(entityId, component);
}

template <class T>
void ComponentPool<T>::UpdateContents(std::function<void(std::vector<T>&, float)> updateFunction, float deltaTime) {
    updateFunction(mComponents, deltaTime);
}

template <class T>
bool ComponentPool<T>::Contains(uint64_t entityId) {
    return mIdToIndexMap.find(entityId) != mIdToIndexMap.end();
}

template <class T>
void ComponentPool<T>::Add(uint64_t entityId, const T& component) {
    if (Contains(entityId)) {
        mComponents[mIdToIndexMap[entityId]] = component;
        return;
    }

    mComponents.emplace_back(component);
    mIdToIndexMap[entityId] = mComponents.size() - 1;
    mLastId = entityId;
}

template <class T>
T* ComponentPool<T>::Get(uint64_t entityId) {
    if (!Contains(entityId)) { return nullptr; }
    return &mComponents[mIdToIndexMap[entityId]];
}

template <class T>
void ComponentPool<T>::Remove(uint64_t entityId) {
    if (!Contains(entityId)) return;

    uint64_t indexToRemove = mIdToIndexMap[entityId];
    uint64_t lastIndex = mComponents.size() - 1;

    if (indexToRemove != lastIndex) {
        mComponents[indexToRemove] = mComponents[lastIndex];
        mIdToIndexMap[mLastId] = indexToRemove;
    }

    mComponents.pop_back();
    mIdToIndexMap.erase(entityId);
}