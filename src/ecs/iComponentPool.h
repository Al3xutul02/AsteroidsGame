#pragma once
#include <typeinfo>
#include "components/abstractions/component.h"

class IComponentPool {
public:
    virtual ~IComponentPool() = default;

    virtual bool Contains(uint32_t entityId) = 0;
    virtual void Add(uint32_t entityId, const Component& componentData) = 0;
    virtual Component* Get(uint32_t entityId) = 0;
    virtual void Remove(uint32_t entityId) = 0;

    virtual const std::type_info& GetTypeInfo() const = 0;
};