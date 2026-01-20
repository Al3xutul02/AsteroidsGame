#include "application.h"
#include "managers/entityManager.h"
#include "managers/inputManager.h"
#include "managers/renderManager.h"
#include "managers/timeManager.h"
#include "ecs/components/transform.h"
#include "ecs/components/sprite.h"
#include "ecs/components/controller.h"
#include "ecs/components/collider.h"
#include "builders/entityBuilder.h"

Application* Application::mInstance = nullptr;

static void CreateGameObjects(SDL_Window* window) {
    // Player ship
    uint32_t ship = EntityBuilder::CreateShip();

    // Asteroid Spawner
    uint32_t asteroidsSpawner = EntityManager::CreateEntity();

    Spawner spawner = Spawner(asteroidsSpawner, window,
        Math::Vector2(100, 200), Math::Vector2(-30, 30), Math::Vector2(10, 30),
        100.0f, 0.5f, 15.0f
    );

    EntityManager::AddComponent<Spawner>(asteroidsSpawner, spawner);
}

bool Application::Initialize(float targetFPS) {
    this->InitializeEvents();
    bool success = this->InitializeSDL();
    if (!success) {
        return false;
    }

    EntityManager::Initialize(this);
    TimeManager::Initialize(this, targetFPS);
    InputManager::Initialize(this);
    RenderManager::Initialize(this, this->mWindow);

    CreateGameObjects(this->mWindow);

    RenderManager::LoadTextures({
        "./resources/Ship.png",
        "./resources/DebugCircle.png",
        "./resources/Asteroids.png",
        "./resources/Bullet.png"
    });

    Application::mInstance = this;
    this->isRunning = success;
    return success;
}

void Application::Shutdown() {
    EntityManager::Shutdown();
    TimeManager::Shutdown();
    InputManager::Shutdown();
    RenderManager::Shutdown();

    SDL_DestroyWindow(this->mWindow);
    SDL_Quit();
}

void Application::RunLoop() {
    while(this->isRunning) {
        this->ProcessInput();
        this->Update();
        this->GenerateOutput();
    }
}

void Application::Notify(AppEvent event) { eventMap[event](); }

void Application::InitializeEvents() {
    eventMap = std::unordered_map<AppEvent, std::function<void()>>();
    eventMap[AppEvent::Exit] = [&]() {
        this->isRunning = false;
    };

    eventMap[AppEvent::Restart] = [&]() {
        // Delete all entities
        EntityManager::DestroyAllEntities();
        // Recreate game objects
        CreateGameObjects(this->mWindow);
    };
}

bool Application::InitializeSDL() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult == 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    this->mWindow = SDL_CreateWindow(
        "GameTutorial Application",
        1024,
        768,
        0
        //SDL_WINDOW_FULLSCREEN);
    );
    if (!this->mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    return true;
}

void Application::ProcessInput() {
    InputManager::ProcessInput();
}

void Application::Update() {
    TimeManager::Update();
    EntityManager::Update(TimeManager::GetDeltaTime());

    //std::cout << "Current framerate: " << TimeManager::GetCurrentFPS()
    //          << "; Delta Time: " << TimeManager::GetDeltaTime() << '\n';
}

void Application::GenerateOutput() {
    RenderManager::Draw();
}