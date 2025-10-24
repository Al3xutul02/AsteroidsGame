#pragma once
#include <vector>
#include <functional>
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
    std::function<void(Collider* self, Collider* other)> OnCollision;

    inline Collider(uint32_t ownerId,
        std::vector<Tag>& tags,
        uint8_t layer,
        float radius,
        std::function<void(Collider* self, Collider* other)> onCollision) : Component(ownerId),
        Tags(tags), Layer(layer), Radius(radius), OnCollision(onCollision) {}

    static void Update(std::vector<Collider>& colliders, float deltaTime);
};
