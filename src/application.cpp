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

bool Application::Initialize(float targetFPS) {
    this->InitializeEvents();
    bool success = this->InitializeSDL();

    EntityManager::Initialize(this);
    TimeManager::Initialize(this, targetFPS);
    InputManager::Initialize(this);
    RenderManager::Initialize(this, this->mWindow);

    // Test entity
    uint32_t ship = EntityBuilder::CreateShip();

    // Test collider
    uint32_t colliderEntity = EntityManager::CreateEntity();
    Transform cTransform = Transform(colliderEntity, false,
        Math::Vector2(500, 500)
    );

    SDL_Rect colliderDest = SDL_Rect {
        (int)cTransform.GlobalLocation.Position.x - 96,
        (int)cTransform.GlobalLocation.Position.y - 96,
        (int)(96), (int)(96)
    };
    Sprite cSprite = Sprite(colliderEntity,
        colliderDest,
        "./resources/DebugCircle.png",
        0.0f, 0.0f, SDL_FLIP_NONE,
        1, 0.5f
    );

    std::vector<Collider::Tag> tags = std::vector<Collider::Tag>({Collider::Tag::Enemy});
    Collider cCollider = Collider(colliderEntity,
        tags,
        0, 48.0f,
        std::function<void(Collider* self, Collider* other)>([](Collider* self, Collider* other) {
            if (std::find(other->Tags.begin(), other->Tags.end(), Collider::Tag::Player) != other->Tags.end()) {
                std::cout << "Collider event triggered on Entity " << self->OwnerId
                      << " by Entity " << other->OwnerId << '\n';
            }
        })
    );

    EntityManager::AddComponent<Transform>(colliderEntity, cTransform);
    EntityManager::AddComponent<Sprite>(colliderEntity, cSprite);
    EntityManager::AddComponent<Collider>(colliderEntity, cCollider);

    // Asteroid Spawner
    uint32_t asteroidsSpawner = EntityManager::CreateEntity();

    Spawner spawner = Spawner(asteroidsSpawner, this->mWindow,
        Math::Vector2(100, 200), Math::Vector2(-30, 30), Math::Vector2(10, 30),
        100.0f, 0.5f
    );

    EntityManager::AddComponent<Spawner>(asteroidsSpawner, spawner);

    RenderManager::LoadTextures({
        "./resources/Ship.png",
        "./resources/DebugCircle.png",
        "./resources/Asteroids.png"
    });

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
    eventMap[AppEvent::CloseApplication] = [&]() {
        this->isRunning = false;
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