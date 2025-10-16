#include "../timeManager.h"
#include "../../application.h"

Application* TimeManager::mApp = nullptr;
float TimeManager::mTargetFPS = 0.0;
float TimeManager::mDeltaTime = 0.0;
uint64_t TimeManager::mTicksCount = 0;
uint32_t TimeManager::mTimerCount = 0;
uint32_t TimeManager::mLastId = 0;
std::unordered_map<uint32_t, uint32_t> TimeManager::mTimerIdMap = std::unordered_map<uint32_t, uint32_t>();
std::vector<TimeManager::Timer> TimeManager::mTimers = std::vector<TimeManager::Timer>();


void TimeManager::Initialize(Application* app, float targetFPS) {
    mApp = app;
    mTargetFPS = targetFPS;
}

void TimeManager::Update() {
    while(SDL_GetTicks() < (mTicksCount + 1 / mTargetFPS * 1000.0f));

    mDeltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mDeltaTime = (mDeltaTime > 0.05f) ? 0.05f : mDeltaTime;
    mTicksCount = SDL_GetTicks();

    for (Timer& timer : mTimers) {
        if (timer.Status != TimerStatus::Running) { continue; }

        timer.CurrentTime += mDeltaTime;
        if (timer.CurrentTime > timer.Limit) {
            timer.CurrentTime = timer.Limit;
            timer.Status = TimerStatus::Finished;
        }
    }
}

void TimeManager::Shutdown() {
    mApp = nullptr;
}

uint32_t TimeManager::CreateTimer(float limit) {
    uint32_t newTimer = mTimerCount++;

    mTimers.emplace_back(Timer(limit));
    mTimerIdMap[newTimer] = mTimers.size() - 1;
    mLastId = newTimer;

    return newTimer;
}

bool TimeManager::TimerExists(uint32_t timerId) {
    auto id = mTimerIdMap.find(timerId);
    if (id == mTimerIdMap.end()) { return false; }
    return true;
}

std::optional<TimerStatus> TimeManager::GetTimerStatus(uint32_t timerId) {
    if (!TimerExists(timerId)) { return std::nullopt; }

    return std::optional<TimerStatus>(mTimers[mTimerIdMap[timerId]].Status);
}

void TimeManager::StartTimer(uint32_t timerId) {
    if (!TimerExists(timerId)) { return; }

    mTimers[mTimerIdMap[timerId]].Status = TimerStatus::Running;
}

void TimeManager::PauseTimer(uint32_t timerId) {
    if (!TimerExists(timerId)) { return; }

    Timer& timer = mTimers[mTimerIdMap[timerId]];
    if (timer.Status != TimerStatus::Running) { return; }

    timer.Status = TimerStatus::Paused;
}

void TimeManager::StopTimer(uint32_t timerId) {
    if (!TimerExists(timerId)) { return; }

    Timer& timer = mTimers[mTimerIdMap[timerId]];
    if (timer.Status == TimerStatus::Finished) { return; }

    timer.Status = TimerStatus::Stopped;
    timer.CurrentTime = 0;
}

void TimeManager::SetTimerLimit(uint32_t timerId, float limit) {
    if (!TimerExists(timerId)) { return; }

    mTimers[mTimerIdMap[timerId]].Limit = limit;
}

void TimeManager::SetTimerCurrentTime(uint32_t timerId, float currentTime) {
    if (!TimerExists(timerId)) { return; }

    mTimers[mTimerIdMap[timerId]].Limit = currentTime;
}

void TimeManager::DestroyTimer(uint32_t timerId) {
    if (!TimerExists(timerId)) { return; }

    uint32_t indexToRemove = mTimerIdMap[timerId];
    uint32_t lastIndex = mTimers.size() - 1;

    if (indexToRemove != lastIndex) {
        mTimers[indexToRemove] = mTimers[lastIndex];
        mTimerIdMap[mLastId] = indexToRemove;
    }

    mTimers.pop_back();
    mTimerIdMap.erase(timerId);
}