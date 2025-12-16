#pragma once
#include <vector>
#include <unordered_map>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "../../managers/inputManager.h"
#include "../../managers/timeManager.h"
#include "abstractions/component.h"

enum Control : uint8_t {
    Up, Down, Left, Right, Fire
};

struct Controller : Component {
    std::unordered_map<Control, Key> KeyMap;
    float MaxMovementSpeed;
    float Acceleration;
    float DragCoefficient;
    float BulletSpeed;
    float BulletCooldown;
    uint32_t BulletTimerId;

    inline Controller(uint32_t ownerId,
        const std::unordered_map<Control, Key>& keyMap,
        float maxMovementSpeed,
        float acceleration,
        float dragCoefficient,
        float bulletSpeed, float bulletCooldown) : Component(ownerId),
        KeyMap(keyMap), MaxMovementSpeed(maxMovementSpeed),
        Acceleration(acceleration), DragCoefficient(dragCoefficient),
        BulletSpeed(bulletSpeed), BulletCooldown(bulletCooldown),
        BulletTimerId(TimeManager::CreateTimer(bulletCooldown)) {}

    static void Update(std::vector<Controller>& controllers, float deltaTime);
};