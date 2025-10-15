#pragma once
#include <unordered_map>
#include <SDL3/SDL.h>
#include "../math/vector2.h"

class Application;

typedef SDL_Scancode Key;

class InputManager {
public:
    static void Initialize(Application* app);
    static void Shutdown();

    static void ProcessInput();
    static bool IsKeyPressed(Key key);
    static inline Math::Vector2 GetMousePosition() { return mMousePosition; }

private:
    static Application* mApp;
    static uint8_t* mCurrentState;
    static uint8_t* mLastState;
    static Math::Vector2 mMousePosition;
};