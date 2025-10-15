#include "../timeManager.h"
#include "../../application.h"

Application* TimeManager::mApp = nullptr;
float TimeManager::mTargetFPS = 0.0;
float TimeManager::mDeltaTime = 0.0;
uint64_t TimeManager::mTicksCount = 0;


void TimeManager::Initialize(Application* app, float targetFPS) {
    mApp = app;
    mTargetFPS = targetFPS;
}

void TimeManager::Update() {
    while(SDL_GetTicks() < (mTicksCount + 1 / mTargetFPS * 1000.0f));

    mDeltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mDeltaTime = (mDeltaTime > 0.05f) ? 0.05f : mDeltaTime;
    mTicksCount = SDL_GetTicks();
}

void TimeManager::Shutdown() {
    mApp = nullptr;
}