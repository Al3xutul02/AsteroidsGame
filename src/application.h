#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

enum AppEvent : uint8_t {
    Exit, Restart
};

class Application {
public:
    bool Initialize(float targetFPS);
    void RunLoop();
    void Shutdown();

    void Notify(AppEvent event);

    static inline Application* GetInstance() { return mInstance; }

private:
    static Application* mInstance;
    SDL_Window* mWindow;
    bool isRunning;

    std::unordered_map<AppEvent, std::function<void()>> eventMap;

    void InitializeEvents();
    bool InitializeSDL();

    void ProcessInput();
    void Update();
    void GenerateOutput();
};