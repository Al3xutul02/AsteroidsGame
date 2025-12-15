#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include "iComponentPool.h"

template <class T>
class ComponentPool : public IComponentPool {
public:
    void UpdateContents(std::function<void(std::vector<T>&, float)> updateFunction, float deltaTime);

    virtual bool Contains(uint32_t entityId) override;
    virtual void Add(uint32_t entityId, const Component& componentData) override;
    virtual T* Get(uint32_t entityId) override;
    virtual void Remove(uint32_t entityId) override;

    virtual inline const std::type_info& GetTypeInfo() const override { return typeid(T); }

    void Add(uint32_t entityId, const T& component);

private:
    uint64_t mLastId;

    std::unordered_map<uint32_t, uint32_t> mIdToIndexMap;
    std::vector<T> mComponents;
};

#include "componentPool.tpp"