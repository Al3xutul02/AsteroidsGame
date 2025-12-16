#include "../controller.h"
#include <iostream>
#include <algorithm>
#include "../../../managers/entityManager.h"
#include "../../../builders/entityBuilder.h"

void Controller::Update(std::vector<Controller>& controllers, float deltaTime) {
    for (Controller& controller : controllers) {
        Transform* transform = EntityManager::GetComponent<Transform>(controller.OwnerId);
        if (transform == nullptr || !transform->Enabled) {
            std::cout << "Could not fetch Transform component for entity " << controller.OwnerId << '\n';
            continue;
        }

        if (!controller.Enabled) { continue; }

        Math::Vector2 direction = Math::Vector2::Zero;
        for (auto& [control, key] : controller.KeyMap) {
            if (!InputManager::IsKeyPressed(key)) { continue; }
            switch(control) {
                case Control::Up:
                    direction.y -= 1.0f;
                    break;
                case Control::Down:
                    direction.y += 1.0f;
                    break;
                case Control::Left:
                    direction.x -= 1.0f;
                    break;
                case Control::Right:
                    direction.x += 1.0f;
                    break;
                default:
                    break;
            }
        }

        direction.x = std::clamp(direction.x, -1.0f, 1.0f);
        direction.y = std::clamp(direction.y, -1.0f, 1.0f);
        if (direction != Math::Vector2::Zero &&
            std::abs(direction.x) == std::abs(direction.y)) {
            direction = Math::Normalize(direction);
        }
        direction *= controller.Acceleration;
        transform->Location.Acceleration = direction;

        int xSign = (int)(((float)std::signbit(transform->Location.Velocity.x) - 0.5f) * 2);
        int ySign = (int)(((float)std::signbit(transform->Location.Velocity.y) - 0.5f) * 2);
        Math::Vector2 dragForce(
            (std::pow(transform->Location.Velocity.x, 2) * controller.DragCoefficient) / 2 * xSign,
            (std::pow(transform->Location.Velocity.y, 2) * controller.DragCoefficient) / 2 * ySign
        );
        transform->Location.Acceleration += dragForce;

        transform->Location.Velocity.x = std::clamp(transform->Location.Velocity.x,
            -controller.MaxMovementSpeed + transform->Location.Acceleration.x * deltaTime,
            controller.MaxMovementSpeed - transform->Location.Acceleration.x * deltaTime);
        transform->Location.Velocity.y = std::clamp(transform->Location.Velocity.y,
            -controller.MaxMovementSpeed + transform->Location.Acceleration.y * deltaTime,
            controller.MaxMovementSpeed - transform->Location.Acceleration.y * deltaTime);

        float mouseAngle = Math::GetAngle(
            Math::Normalize(InputManager::GetMousePosition() - transform->Location.Position)
        );
        transform->Rotation.Angle = mouseAngle;

        std::optional<TimerStatus> statusFetch = TimeManager::GetTimerStatus(controller.BulletTimerId);
        if (!statusFetch.has_value()) {
            std::cout << "Error: Controller component of entity " << controller.OwnerId
                      << " does not have a timer...";
            continue;
        }

        TimerStatus status = statusFetch.value();
        if (InputManager::IsKeyPressed(controller.KeyMap[Control::Fire]) &&
            (status == TimerStatus::Stopped || status == TimerStatus::Finished))
        {
            Math::Vector2 bulletDirection = Math::Normalize(
                InputManager::GetMousePosition() - transform->Location.Position
            );
            Math::Vector2 bulletVelocity = bulletDirection * controller.BulletSpeed;

            uint32_t bulletId = EntityBuilder::CreateBullet(
                transform->Location.Position + bulletDirection * 30.0f,
                bulletVelocity,
                transform->Rotation.Angle,
                2.0f,
                controller.OwnerId
            );

            TimeManager::StartTimer(controller.BulletTimerId);
        }
    }
}