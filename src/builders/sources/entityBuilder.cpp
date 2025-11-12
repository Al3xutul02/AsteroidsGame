#include "../entityBuilder.h"

uint32_t EntityBuilder::CreateShip() {
    uint32_t ship = EntityManager::CreateEntity();
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

    auto tags = std::vector<Collider::Tag>({Collider::Tag::Player});
    Collider collider = Collider(ship,
        tags,
        0, 24.0f,
        std::function<void(Collider* self, Collider* other)>([](Collider* self, Collider* other) {
            std::cout << "Collider event triggered on Entity " << self->OwnerId
                      << " by Entity " << other->OwnerId << '\n';
        })
    );

    EntityManager::AddComponent<Transform>(ship, transform);
    EntityManager::AddComponent<Sprite>(ship, sprite);
    EntityManager::AddComponent<Controller>(ship, controller);
    EntityManager::AddComponent<Collider>(ship, collider);

    // Test debug sprite
    uint32_t debugSprite = EntityManager::CreateEntity(ship);
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

    return ship;
}