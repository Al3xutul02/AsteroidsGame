#pragma once
#include <cstdint>

struct Component {
public:
    uint32_t OwnerId;
    bool Enabled;

    inline Component(uint32_t ownerId) : OwnerId(ownerId), Enabled(true) {}
    virtual ~Component() = default;
};
