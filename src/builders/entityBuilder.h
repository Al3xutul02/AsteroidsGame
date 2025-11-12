#pragma once
#include <iostream>
#include "../managers/entityManager.h"

class EntityBuilder {
public:
    static uint32_t CreateShip();
    static uint32_t CreateAsteroid(Math::Vector2 initialPosition, Math::Vector2 initialVelocity);
};