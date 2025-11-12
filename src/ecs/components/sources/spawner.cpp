#include "../spawner.h"
#include <iostream>

void Spawner::Update(std::vector<Spawner>& spawners, float deltaTime) {
    for (Spawner spawner : spawners) {
        std::optional<TimerStatus> statusFetch = TimeManager::GetTimerStatus(spawner.TimerId);
        if (!statusFetch.has_value()) {
            std::cout << "Error: Spawner component of entity " << spawner.OwnerId
                      << " does not have a timer...";
            continue;
        }

        TimerStatus status = statusFetch.value();
        if (status == TimerStatus::Finished) {
            
        }

        if (status == TimerStatus::Stopped ||
            status == TimerStatus::Finished)
        {
            TimeManager::StartTimer(spawner.TimerId);
        }
    }
}