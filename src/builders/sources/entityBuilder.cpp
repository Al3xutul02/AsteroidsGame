#include "../entityBuilder.h"

uint32_t EntityBuilder::CreateShip(int ownerId) {
    uint32_t ship;
    if (ownerId < 0) {
        ship = EntityManager::CreateEntity();
    } else {
        ship = EntityManager::CreateEntity((uint32_t) ownerId);
    }
    
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
        "./resources/Ship.png",
        90.0f, 0.0f, SDL_FLIP_NONE,
        2, 1.0f
    );

    Controller controller = Controller(ship,
        {
            { Control::Up,      Key::SDL_SCANCODE_W },
            { Control::Left,    Key::SDL_SCANCODE_A },
            { Control::Down,    Key::SDL_SCANCODE_S },
            { Control::Right,   Key::SDL_SCANCODE_D },
            { Control::Up,      Key::SDL_SCANCODE_UP },
            { Control::Down,    Key::SDL_SCANCODE_DOWN },
            { Control::Left,    Key::SDL_SCANCODE_LEFT },
            { Control::Right,   Key::SDL_SCANCODE_RIGHT },
            { Control::Fire,    Key::SDL_SCANCODE_SPACE}
        },
        500.0f, 1000.0f, 0.035f, 400.0f, 0.5f
    );

    auto tags = std::vector<Collider::Tag>({Collider::Tag::Player});
    Collider collider = Collider(ship,
        tags,
        0, 24.0f,
        std::function<void(Collider* self, Collider* other)>([](Collider* self, Collider* other) {
            if (std::find(other->Tags.begin(), other->Tags.end(), Collider::Tag::Enemy) != other->Tags.end()) {
                std::cout << "Player Ship hit by Asteroid! Ship ID " << self->OwnerId << '\n';
                TimeManager::DestroyTimer(
                    EntityManager::GetComponent<Controller>(self->OwnerId)->BulletTimerId
                );
                EntityManager::DestroyEntity(self->OwnerId);
            }
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
        "./resources/DebugCircle.png",
        0.0f, 0.0f, SDL_FLIP_NONE,
        2, 0.5f
    );

    EntityManager::AddComponent<Transform>(debugSprite, dTransform);
    EntityManager::AddComponent<Sprite>(debugSprite, dSprite);

    return ship;
}

uint32_t EntityBuilder::CreateAsteroid(Math::Vector2 initialPosition, Math::Vector2 initialVelocity,
    float initialRotationVelocity, float destroyTime, int ownerId) {
    uint32_t asteroid;
    if (ownerId < 0) {
        asteroid = EntityManager::CreateEntity();
    } else {
        asteroid = EntityManager::CreateEntity((uint32_t) ownerId);
    }

    Transform transform = Transform(asteroid, false, initialPosition, initialVelocity, Math::Vector2::Zero,
    0.0f, initialRotationVelocity, 0.0f);

    SDL_Rect destination = SDL_Rect {
        (int)transform.GlobalLocation.Position.x - 48,
        (int)transform.GlobalLocation.Position.y - 48,
        48 * 2, 48 * 2
    };
    Sprite sprite = Sprite(asteroid,
        destination,
        "./resources/Asteroids.png",
        0.0f, 0.0f, SDL_FLIP_NONE, 1
    );

    auto tags = std::vector<Collider::Tag>({Collider::Tag::Enemy});
    Collider collider = Collider(asteroid,
        tags,
        0, 30.0f,
        std::function<void(Collider* self, Collider* other)>([](Collider* self, Collider* other) {
            if (std::find(other->Tags.begin(), other->Tags.end(), Collider::Tag::Player) != other->Tags.end()) {
                std::cout << "Asteroid destroyed! Asteroid ID " << self->OwnerId << '\n';
                TimeManager::DestroyTimer(
                    EntityManager::GetComponent<Fleeting>(self->OwnerId)->TimerId
                );
                EntityManager::DestroyEntity(self->OwnerId);
            }
        })
    );

    Fleeting fleeting = Fleeting(asteroid, destroyTime);

    EntityManager::AddComponent<Transform>(asteroid, transform);
    EntityManager::AddComponent<Sprite>(asteroid, sprite);
    EntityManager::AddComponent<Collider>(asteroid, collider);
    EntityManager::AddComponent<Fleeting>(asteroid, fleeting);

    // Test debug sprite
    uint32_t debugSprite = EntityManager::CreateEntity(asteroid);
    Transform dTransform = Transform(debugSprite, true,
        Math::Vector2(0, 0)
    );

    SDL_Rect debugDest = SDL_Rect {
        (int)dTransform.GlobalLocation.Position.x - 60,
        (int)dTransform.GlobalLocation.Position.y - 60,
        (int)(60), (int)(60)
    };
    Sprite dSprite = Sprite(debugSprite,
        debugDest,
        "./resources/DebugCircle.png",
        0.0f, 0.0f, SDL_FLIP_NONE,
        1, 0.5f
    );

    EntityManager::AddComponent<Transform>(debugSprite, dTransform);
    EntityManager::AddComponent<Sprite>(debugSprite, dSprite);

    return asteroid;
}

uint32_t EntityBuilder::CreateBullet(Math::Vector2 initialPosition, Math::Vector2 initialVelocity,
    float initialRotation, float destroyTime, int ownerId) {
    uint32_t bullet;
    if (ownerId < 0) {
        bullet = EntityManager::CreateEntity();
    } else {
        bullet = EntityManager::CreateEntity((uint32_t) ownerId);
    }

    Transform transform = Transform(bullet, false, initialPosition, initialVelocity, Math::Vector2::Zero,
        initialRotation, 0.0f, 0.0f);

    SDL_Rect destination = SDL_Rect {
        (int)transform.GlobalLocation.Position.x - 32,
        (int)transform.GlobalLocation.Position.y - 32,
        32 * 2, 32 * 2
    };
    Sprite sprite = Sprite(bullet,
        destination,
        "./resources/Bullet.png",
        90.0f, 0.0f, SDL_FLIP_NONE, 1
    );

    auto tags = std::vector<Collider::Tag>({Collider::Tag::Player});
    Collider collider = Collider(bullet,
        tags,
        0, 8.0f,
        std::function<void(Collider* self, Collider* other)>([](Collider* self, Collider* other) {
            if (std::find(other->Tags.begin(), other->Tags.end(), Collider::Tag::Enemy) != other->Tags.end()) {
                std::cout << "Bullet hit Asteroid! Bullet ID " << self->OwnerId << '\n';
                TimeManager::DestroyTimer(
                    EntityManager::GetComponent<Fleeting>(self->OwnerId)->TimerId
                );
                EntityManager::DestroyEntity(self->OwnerId);
            }
        })
    );

    Fleeting fleeting = Fleeting(bullet, destroyTime);

    EntityManager::AddComponent<Transform>(bullet, transform);
    EntityManager::AddComponent<Sprite>(bullet, sprite);
    EntityManager::AddComponent<Collider>(bullet, collider);
    EntityManager::AddComponent<Fleeting>(bullet, fleeting);

    // Test debug sprite
    uint32_t debugSprite = EntityManager::CreateEntity(bullet);
    Transform dTransform = Transform(debugSprite, true,
        Math::Vector2(0, 0)
    );

    SDL_Rect debugDest = SDL_Rect {
        (int)dTransform.GlobalLocation.Position.x - 16,
        (int)dTransform.GlobalLocation.Position.y - 16,
        (int)(32), (int)(32)
    };
    Sprite dSprite = Sprite(debugSprite,
        debugDest,
        "./resources/DebugCircle.png",
        0.0f, 0.0f, SDL_FLIP_NONE,
        1, 0.5f
    );

    EntityManager::AddComponent<Transform>(debugSprite, dTransform);
    EntityManager::AddComponent<Sprite>(debugSprite, dSprite);

    return bullet;
}