#pragma once
#include <unordered_map>
#include <vector>
#include <optional>
#include <SDL3/SDL.h>

enum TimerStatus : uint8_t {
        Stopped, Running, Paused, Finished
};

class Application;

class TimeManager {
public:
    static void Initialize(Application* app, float targetFPS);
    static void Update();
    static void Shutdown();

    static inline float GetDeltaTime() { return mDeltaTime; }
    static inline float GetCurrentFPS() { return 1 / mDeltaTime; }
    static inline void SetTargetFPS(float targetFPS) { mTargetFPS = targetFPS; }

    static uint32_t CreateTimer(float limit);
    static bool TimerExists(uint32_t timerId);
    static std::optional<TimerStatus> GetTimerStatus(uint32_t timerId);
    static void StartTimer(uint32_t timerId);
    static void PauseTimer(uint32_t timerId);
    static void StopTimer(uint32_t timerId);
    static void SetTimerLimit(uint32_t timerId, float limit);
    static void SetTimerCurrentTime(uint32_t timerId, float currentTime);
    static void DestroyTimer(uint32_t timerId);

private:
    struct Timer {
        uint32_t Id;
        float Limit;
        float CurrentTime;
        TimerStatus Status;

        inline Timer(
            uint32_t id, float limit, float currentTime = 0.0f,
            TimerStatus status = TimerStatus::Stopped
        ) : Id(id), Limit(limit), CurrentTime(currentTime), Status(status) {}
    };

    static Application* mApp;
    static float mTargetFPS;
    static float mDeltaTime;
    static uint64_t mTicksCount;

    static uint32_t mTimerCount;
    static std::unordered_map<uint32_t, uint32_t> mTimerIdMap;
    static std::vector<Timer> mTimers;
};