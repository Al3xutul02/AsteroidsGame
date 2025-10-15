#pragma once

struct Component {
public:
    uint64_t OwnerId;
    bool Enabled;

    inline Component(uint64_t ownerId) : OwnerId(ownerId), Enabled(true) {}
    virtual ~Component() = default;
};
