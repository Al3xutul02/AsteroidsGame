#pragma once
#include <vector>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "abstractions/component.h"

struct Transform : Component {
    bool InheritsParentTransform;

    struct Location {
        Math::Vector2 Position;
        Math::Vector2 Velocity;
        Math::Vector2 Acceleration;

        inline Location(
            const Math::Vector2& position = Math::Vector2::Zero,
            const Math::Vector2& velocity = Math::Vector2::Zero,
            const Math::Vector2& acceleration = Math::Vector2::Zero
        ) : Position(position), Velocity(velocity), Acceleration(acceleration) {}
    } Location, GlobalLocation;
    
    struct Rotation {
        float Angle;
        float Velocity;
        float Acceleration;

        inline Rotation(
            float angle = 0.0f,
            float velocity = 0.0f,
            float acceleration = 0.0f) :
        Angle(angle), Velocity(velocity), Acceleration(acceleration) {}
    } Rotation, GlobalRotation;

    Transform(uint32_t ownerId, bool inheritsParentTransform = true,
        const Math::Vector2& locationPosition = Math::Vector2::Zero,
        const Math::Vector2& locationVelocity = Math::Vector2::Zero,
        const Math::Vector2& locationAcceleration = Math::Vector2::Zero,
        float rotationAngle = 0.0f,
        float rotationVelocity = 0.0f,
        float rotationAcceleration = 0.0f);

    static void Update(std::vector<Transform>& transforms, float deltaTime);
};
