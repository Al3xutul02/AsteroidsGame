#pragma once
#include <vector>
#include <cstdlib>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "abstractions/component.h"
#include "../../managers/timeManager.h"

struct Spawner : Component {
    SDL_Window* Window;
    Math::Vector2 SpawnSpeed;
    Math::Vector2 DirectionOffsetAngle;
    Math::Vector2 RotationSpeed;
    float SpawnOffset;
    float SpawnTime;
    float DestroyTime;
    uint32_t TimerId;

    inline Spawner(uint32_t ownerId,
        SDL_Window* window,
        const Math::Vector2& spawnSpeed,
        const Math::Vector2& directionOffsetAngle,
        const Math::Vector2& rotationSpeed,
        float spawnOffset, float spawnTime, float destroyTime) : Component(ownerId),
        Window(window),
        SpawnSpeed(spawnSpeed), DirectionOffsetAngle(directionOffsetAngle),
        RotationSpeed(rotationSpeed),
        SpawnOffset(spawnOffset), SpawnTime(spawnTime), DestroyTime(destroyTime),
        TimerId(TimeManager::CreateTimer(spawnTime)) {}

    static void Update(std::vector<Spawner>& spawners, float deltaTime);
};