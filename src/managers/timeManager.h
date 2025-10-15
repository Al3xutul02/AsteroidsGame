#pragma once
#include <SDL3/SDL.h>

class Application;

class TimeManager {
public:
    static void Initialize(Application* app, float targetFPS);
    static void Update();
    static void Shutdown();

    static inline float GetDeltaTime() { return mDeltaTime; }
    static inline float GetCurrentFPS() { return 1 / mDeltaTime; }

    static inline void SetTargetFPS(float targetFPS) { mTargetFPS = targetFPS; }

private:
    static Application* mApp;
    static float mTargetFPS;
    static float mDeltaTime;
    static uint64_t mTicksCount;
};