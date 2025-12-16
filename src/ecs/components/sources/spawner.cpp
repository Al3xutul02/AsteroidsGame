#include "../spawner.h"
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <numbers>
#include <SDL3/SDL.h>
#include "../../../builders/entityBuilder.h"

void Spawner::Update(std::vector<Spawner>& spawners, float deltaTime) {
    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 randGenerator(seed);

    for (Spawner spawner : spawners) {
        std::optional<TimerStatus> statusFetch = TimeManager::GetTimerStatus(spawner.TimerId);
        if (!statusFetch.has_value()) {
            std::cout << "Error: Spawner component of entity " << spawner.OwnerId
                      << " does not have a timer...";
            continue;
        }

        TimerStatus status = statusFetch.value();
        if (status == TimerStatus::Finished) {
            int width, height;
            SDL_GetWindowSize(spawner.Window, &width, &height);

            const float minSpawnWidth = -spawner.SpawnOffset;
            const float maxSpawnWidth = width + spawner.SpawnOffset;

            const float locationInterpolation = (randGenerator() % 100) / 100.0f;
            const float speedInterpolation = (randGenerator() % 100) / 100.0f;
            const float rotationInterpolation = (randGenerator() % 100) / 100.0f;
            const float rotationVelocityInterpolation = (randGenerator() % 100) / 100.0f;

            const float speed = Math::Lerp(spawner.SpawnSpeed.x, spawner.SpawnSpeed.y, speedInterpolation);
            const float rotation = Math::Lerp(spawner.DirectionOffsetAngle.x, spawner.DirectionOffsetAngle.y,
                rotationInterpolation);
            const float rotationVelocity = Math::Lerp(spawner.RotationSpeed.x, spawner.RotationSpeed.y,
                rotationVelocityInterpolation) * (int)((randGenerator() % 2 - 0.5f) * 2);
            const Math::Vector2 windowCenter = Math::Vector2(
                width / 2.0f,
                height / 2.0f
            );
            
            const Math::Vector2 spawnLocation = Math::Vector2(
                Math::Lerp(minSpawnWidth, maxSpawnWidth, locationInterpolation),
                -spawner.SpawnOffset
            );
            static constexpr float degToRad = std::numbers::pi_v<float> / 180.0f;
            const float rotationRad = rotation * degToRad;
            const float cosAngle = std::cosf(rotationRad);
            const float sinAngle = std::sinf(rotationRad);
            const Math::Vector2 centerSpawnVelocity = Math::Normalize(spawnLocation - windowCenter) * speed * -1.0f;
            const Math::Vector2 rotatedSpawnVelocity = Math::Vector2(
                centerSpawnVelocity.x * cosAngle - centerSpawnVelocity.y * sinAngle,
                centerSpawnVelocity.x * sinAngle + centerSpawnVelocity.y * cosAngle
            );

            uint32_t asteroidId = EntityBuilder::CreateAsteroid(spawnLocation, rotatedSpawnVelocity, rotationVelocity,
                spawner.DestroyTime, spawner.OwnerId);
        }

        if (status == TimerStatus::Stopped ||
            status == TimerStatus::Finished)
        {
            TimeManager::StartTimer(spawner.TimerId);
        }
    }
}