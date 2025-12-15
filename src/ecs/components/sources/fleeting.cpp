#include "../fleeting.h"
#include <iostream>
#include "../../../managers/entityManager.h"

void Fleeting::Update(std::vector<Fleeting>& fleetings, float deltaTime) {
    for (Fleeting fleeting : fleetings) {
        std::optional<TimerStatus> statusFetch = TimeManager::GetTimerStatus(fleeting.TimerId);

        if (!statusFetch.has_value()) {
            std::cout << "Error: Fleeting component of entity " << fleeting.OwnerId
                      << " does not have a timer...";
            continue;
        }

        TimerStatus status = statusFetch.value();
        if (status == TimerStatus::Stopped) {
            TimeManager::StartTimer(fleeting.TimerId);
        }

        if (status == TimerStatus::Finished) {
            TimeManager::DestroyTimer(fleeting.TimerId);
            EntityManager::DestroyEntity(fleeting.OwnerId);
        }
    }
}