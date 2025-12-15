#pragma once
#include "componentPool.h"

template <class T>
void ComponentPool<T>::Add(uint32_t entityId, const Component& componentData) {
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
bool ComponentPool<T>::Contains(uint32_t entityId) {
    return mIdToIndexMap.find(entityId) != mIdToIndexMap.end();
}

template <class T>
void ComponentPool<T>::Add(uint32_t entityId, const T& component) {
    if (Contains(entityId)) {
        mComponents[mIdToIndexMap[entityId]] = component;
        return;
    }

    mComponents.emplace_back(component);
    mIdToIndexMap[entityId] = mComponents.size() - 1;
    mLastId = entityId;
}

template <class T>
T* ComponentPool<T>::Get(uint32_t entityId) {
    if (!Contains(entityId)) { return nullptr; }
    return &mComponents[mIdToIndexMap[entityId]];
}

template <class T>
void ComponentPool<T>::Remove(uint32_t entityId) {
    if (!Contains(entityId)) return;

    uint32_t indexToRemove = mIdToIndexMap[entityId];
    uint32_t lastIndex = mComponents.size() - 1;

    uint32_t entityIdOfMovedComponent = mComponents[lastIndex].OwnerId;

    if (indexToRemove != lastIndex) {
        mComponents[indexToRemove] = std::move(mComponents[lastIndex]); 
        mComponents[indexToRemove].OwnerId = entityIdOfMovedComponent;
        mIdToIndexMap[entityIdOfMovedComponent] = indexToRemove;
    }


    mComponents.pop_back();
    mIdToIndexMap.erase(entityId);
}