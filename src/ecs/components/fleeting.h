#pragma once
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "abstractions/component.h"
#include "../../managers/timeManager.h"

struct Fleeting : Component {
    uint32_t TimerId;

    inline Fleeting(uint32_t ownerId, float destroyTime) : Component(ownerId),
        TimerId(TimeManager::CreateTimer(destroyTime)) {}

    static void Update(std::vector<Fleeting>& fleetings, float deltaTime);
};