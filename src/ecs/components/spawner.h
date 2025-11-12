#pragma once
#include <vector>
#include <cstdlib>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "abstractions/component.h"
#include "../../managers/timeManager.h"

struct Spawner : Component {
    uint32_t ScreenWidth;
    uint32_t ScreenHeight;
    uint32_t SpawnOffset;
    float SpawnTime;
    uint32_t TimerId;

    inline Spawner(uint32_t ownerId,
        uint32_t screenWidth, uint32_t screenHeight,
        uint32_t spawnOffset, float spawnTime) : Component(ownerId),
        ScreenWidth(screenWidth), ScreenHeight(screenHeight),
        SpawnOffset(spawnOffset), SpawnTime(spawnTime), TimerId(TimeManager::CreateTimer(spawnTime)) {}

    static void Update(std::vector<Spawner>& spawners, float deltaTime);
};