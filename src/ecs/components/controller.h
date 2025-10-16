#pragma once
#include <vector>
#include <unordered_map>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "../../managers/inputManager.h"
#include "abstractions/component.h"

enum Control : uint8_t {
    Up, Down, Left, Right
};

struct Controller : Component {
    std::unordered_map<Key, Control> KeyMap;
    float MaxMovementSpeed;
    float Acceleration;
    float DragCoefficient;

    inline Controller(uint32_t ownerId,
        const std::unordered_map<Key, Control>& keyMap,
        float maxMovementSpeed,
        float acceleration,
        float dragCoefficient) : Component(ownerId),
        KeyMap(keyMap), MaxMovementSpeed(maxMovementSpeed),
        Acceleration(acceleration), DragCoefficient(dragCoefficient) {}

    static void Update(std::vector<Controller>& controllers, float deltaTime);
};