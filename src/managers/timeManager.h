#pragma once
#include <unordered_map>
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

    static uint64_t CreateTimer(float limit);
    static bool TimerExists(uint64_t timerId);
    static std::optional<TimerStatus> GetTimerStatus(uint64_t timerId);
    static void StartTimer(uint64_t timerId);
    static void PauseTimer(uint64_t timerId);
    static void StopTimer(uint64_t timerId);
    static void SetTimerLimit(uint64_t timerId, float limit);
    static void SetTimerCurrentTime(uint64_t timerId, float currentTime);
    static void DestroyTimer(uint64_t timerId);

private:
    struct Timer {
        float Limit;
        float CurrentTime;
        TimerStatus Status;

        inline Timer(
            float limit, float currentTime = 0.0f,
            TimerStatus status = TimerStatus::Stopped
        ) : Limit(limit), CurrentTime(currentTime), Status(status) {}
    };

    static Application* mApp;
    static float mTargetFPS;
    static float mDeltaTime;
    static uint64_t mTicksCount;

    static uint64_t mTimerCount;
    static uint64_t mLastId;
    static std::unordered_map<uint64_t, uint64_t> mTimerIdMap;
    static std::vector<Timer> mTimers;
};