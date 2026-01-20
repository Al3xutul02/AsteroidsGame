#include "../inputManager.h"
#include "../../application.h"

Application* InputManager::mApp = nullptr;
uint8_t* InputManager::mCurrentState = nullptr;
uint8_t* InputManager::mLastState = nullptr;

Math::Vector2 InputManager::mMousePosition = Math::Vector2::Zero;

void InputManager::Initialize(Application* app) {
    mApp = app;
    mCurrentState = (uint8_t*)SDL_GetKeyboardState(NULL);
    mLastState = (uint8_t*)SDL_GetKeyboardState(NULL);
}

void InputManager::Shutdown() {
    mCurrentState = nullptr;
    mLastState = nullptr;
}

void InputManager::ProcessInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                mApp->Notify(AppEvent::Exit);
                break;
        }
    }

    mLastState = mCurrentState;
    mCurrentState = (uint8_t*)SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mMousePosition.x, &mMousePosition.y);

    if (IsKeyPressed(Key::SDL_SCANCODE_ESCAPE)) {
        mApp->Notify(AppEvent::Exit);
    }
}

bool InputManager::IsKeyPressed(Key key) {
    return (bool)mCurrentState[key];
}