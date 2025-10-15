#include "application.h"
#include "managers/entityManager.h"
#include "managers/inputManager.h"
#include "managers/renderManager.h"
#include "managers/timeManager.h"
#include "ecs/components/transform.h"
#include "ecs/components/sprite.h"
#include "ecs/components/controller.h"

bool Application::Initialize(float targetFPS) {
    this->InitializeEvents();
    bool success = this->InitializeSDL();

    EntityManager::Initialize(this);
    TimeManager::Initialize(this, targetFPS);
    InputManager::Initialize(this);
    RenderManager::Initialize(this, this->mWindow);

    // Test entity
    uint64_t ship = EntityManager::CreateEntity();
    Transform transform = Transform(ship, false,
        Math::Vector2(250, 250)
    );

    SDL_Rect destination = SDL_Rect {
        (int)transform.GlobalLocation.Position.x - 48,
        (int)transform.GlobalLocation.Position.y - 48,
        48 * 2, 48 * 2
    };
    Sprite sprite = Sprite(ship,
        destination,
        "./Ship.png",
        90.0f
    );

    Controller controller = Controller(ship,
        {
            { Key::SDL_SCANCODE_W, Control::Up },
            { Key::SDL_SCANCODE_A, Control::Left },
            { Key::SDL_SCANCODE_S, Control::Down },
            { Key::SDL_SCANCODE_D, Control::Right },
            { Key::SDL_SCANCODE_UP, Control::Up },
            { Key::SDL_SCANCODE_DOWN, Control::Down },
            { Key::SDL_SCANCODE_LEFT, Control::Left },
            { Key::SDL_SCANCODE_RIGHT, Control::Right }
        },
        500.0f, 1000.0f, 0.035f
    );

    EntityManager::AddComponent<Transform>(ship, transform);
    EntityManager::AddComponent<Sprite>(ship, sprite);
    EntityManager::AddComponent<Controller>(ship, controller);

    // Test debug sprite
    uint64_t debugSprite = EntityManager::CreateEntity(ship);
    Transform dTransform = Transform(debugSprite, true,
        Math::Vector2(0, 0)
    );

    SDL_Rect debugDest = SDL_Rect {
        (int)dTransform.GlobalLocation.Position.x - 48,
        (int)dTransform.GlobalLocation.Position.y - 48,
        (int)(48), (int)(48)
    };
    Sprite dSprite = Sprite(debugSprite,
        debugDest,
        "./DebugCircle.png",
        0.0f, 0.0f, SDL_FLIP_NONE,
        1, 0.5f
    );

    EntityManager::AddComponent<Transform>(debugSprite, dTransform);
    EntityManager::AddComponent<Sprite>(debugSprite, dSprite);

    RenderManager::LoadTextures({
        "./Ship.png",
        "./DebugCircle.png"
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

    std::cout << "Current framerate: " << TimeManager::GetCurrentFPS()
              << "; Delta Time: " << TimeManager::GetDeltaTime() << '\n';
}

void Application::GenerateOutput() {
    RenderManager::Draw();
}