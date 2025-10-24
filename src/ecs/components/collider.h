#pragma once
#include <vector>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "abstractions/component.h"

struct Collider : Component {
    enum Tag : uint8_t {
        Player, Enemy, Bullet
    };

    std::vector<Tag> Tags;
    uint8_t Layer;
    float Radius;

    inline Collider(uint32_t ownerId,
        std::vector<Tag>& tags,
        uint8_t layer,
        float radius) : Component(ownerId),
        Tags(tags), Layer(layer), Radius(radius) {}

    static void Update(std::vector<Collider>& colliders, float deltaTime);
};
