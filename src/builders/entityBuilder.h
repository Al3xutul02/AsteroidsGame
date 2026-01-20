#pragma once
#include <iostream>
#include <algorithm>
#include "../managers/entityManager.h"

class EntityBuilder {
public:
    static uint32_t CreateShip(int ownerId = -1);
    static uint32_t CreateAsteroid(Math::Vector2 initialPosition, Math::Vector2 initialVelocity,
        float initialRotationVelocity, float destroyTime, int ownerId = -1);
    static uint32_t CreateBullet(Math::Vector2 initialPosition, Math::Vector2 initialVelocity,
        float initialRotation, float destroyTime, int ownerId = -1);
};