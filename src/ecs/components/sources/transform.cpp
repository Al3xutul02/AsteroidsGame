#include "../transform.h"
#include <iostream>
#include <map>
#include <cmath>
#include <numbers>
#include "../../../managers/entityManager.h"

Transform::Transform(uint64_t ownerId, bool inheritsParentTransform,
const Math::Vector2& locationPosition,
const Math::Vector2& locationVelocity,
const Math::Vector2& locationAcceleration,
float rotationAngle,
float rotationVelocity,
float rotationAcceleration) : Component(ownerId), InheritsParentTransform(inheritsParentTransform),
Location(locationPosition, locationVelocity, locationAcceleration),
Rotation(rotationAngle, rotationVelocity, rotationAcceleration),
GlobalLocation(), GlobalRotation() {
    std::optional<uint64_t> parent = EntityManager::GetParentEntity(ownerId);
    if (!parent.has_value()) {
        this->GlobalLocation = this->Location;
        this->GlobalRotation = this->Rotation;
        return;
    }
    Transform* parentTransform = EntityManager::GetComponent<Transform>(parent.value());
    if (parentTransform == nullptr) {
        this->GlobalLocation = this->Location;
        this->GlobalRotation = this->Rotation;
        return;
    }

    this->GlobalLocation.Position = parentTransform->GlobalLocation.Position + locationPosition;
    this->GlobalLocation.Velocity = parentTransform->GlobalLocation.Velocity + locationVelocity;
    this->GlobalLocation.Acceleration = parentTransform->GlobalLocation.Acceleration + locationAcceleration;

    this->GlobalRotation.Angle = parentTransform->GlobalRotation.Angle + rotationAngle;
    this->GlobalRotation.Velocity = parentTransform->GlobalRotation.Velocity + rotationVelocity;
    this->GlobalRotation.Acceleration = parentTransform->GlobalRotation.Acceleration + rotationAcceleration;
}

static void CalculateHierarchyDepths(
    std::vector<Transform>& transforms,
    std::map<uint8_t, std::vector<Transform*>>& depthMap);

static void ApplyLocalKinematics(Transform* transform, float deltaTime);

static void ApplyRotation(
    Math::Vector2& local, Math::Vector2& global, Math::Vector2& parentGlobal,
    const float cosAngle, const float sinAngle);

void Transform::Update(std::vector<Transform>& transforms, float deltaTime) {
    std::map<uint8_t, std::vector<Transform*>> depthMap;
    CalculateHierarchyDepths(transforms, depthMap);
    
    for (auto& [depth, transforms] : depthMap) {
        for (Transform* const transform : transforms) {
            if (!transform->Enabled) { continue; }
            ApplyLocalKinematics(transform, deltaTime);
            
            std::optional<uint64_t> parentId = EntityManager::GetParentEntity(transform->OwnerId);
            if (parentId.has_value() && transform->InheritsParentTransform) {
                Transform* parentTransform = EntityManager::GetComponent<Transform>(parentId.value());
                if (parentTransform != nullptr) {
                    static constexpr float degToRad = std::numbers::pi_v<float> / 180.0f;
                    const float parentAngleRad = parentTransform->Rotation.Angle * degToRad;
                    const float cosAngle = std::cosf(parentAngleRad);
                    const float sinAngle = std::sinf(parentAngleRad);
                    
                    ApplyRotation(
                        transform->Location.Acceleration, transform->GlobalLocation.Acceleration,
                        parentTransform->GlobalLocation.Acceleration,
                        cosAngle, sinAngle
                    );

                    ApplyRotation(
                        transform->Location.Velocity, transform->GlobalLocation.Velocity,
                        parentTransform->GlobalLocation.Velocity,
                        cosAngle, sinAngle
                    );

                    ApplyRotation(
                        transform->Location.Position, transform->GlobalLocation.Position,
                        parentTransform->GlobalLocation.Position,
                        cosAngle, sinAngle
                    );

                    transform->GlobalRotation.Velocity =
                        parentTransform->GlobalRotation.Velocity + transform->Rotation.Velocity;
                    transform->GlobalRotation.Angle =
                        parentTransform->GlobalRotation.Angle + transform->Rotation.Angle;

                    continue;
                }
            }

            transform->GlobalLocation = transform->Location;
            transform->GlobalRotation = transform->Rotation;
        }
    }
}

static void CalculateHierarchyDepths(
    std::vector<Transform>& transforms,
    std::map<uint8_t, std::vector<Transform*>>& depthMap) 
{
    static const uint8_t depthLimit = std::numeric_limits<uint8_t>::max();

    for (Transform& transform : transforms) {
        uint8_t depth = 0;
        std::optional<uint64_t> parent = EntityManager::GetParentEntity(transform.OwnerId);
        
        while(parent.has_value() && depth < depthLimit) {
            depth++;
            parent = EntityManager::GetParentEntity(parent.value());
        }

        if (depth == depthLimit && parent.has_value()) {
            std::cerr << "Warning: Child depth limit reached (" << (int)depthLimit << ") for entity " 
                      << transform.OwnerId << ", cycle might exist. Skipping update for this entity and its children.\n";
            continue; 
        }

        depthMap[depth].push_back(&transform);
    }
}

static void ApplyLocalKinematics(Transform* transform, float deltaTime) {
    transform->Location.Velocity += transform->Location.Acceleration * deltaTime;
    transform->Location.Position += transform->Location.Velocity * deltaTime;

    transform->Rotation.Velocity += transform->Rotation.Acceleration * deltaTime;
    transform->Rotation.Angle += transform->Rotation.Velocity * deltaTime;
}

static void ApplyRotation(
    Math::Vector2& local, Math::Vector2& global, Math::Vector2& parentGlobal,
    const float cosAngle, const float sinAngle)
{
    const Math::Vector2 rotatedLocal(
        local.x * cosAngle - local.y * sinAngle,
        local.x * sinAngle + local.y * cosAngle
    );

    global = parentGlobal + rotatedLocal;
}