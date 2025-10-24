#include "../collider.h"
#include <iostream>
#include <unordered_map>
#include "../../../managers/entityManager.h"

struct CollisionSnapshot {
    Collider* Origin;
    uint32_t OwnerId;
    Math::Vector2 GlobalPosition;
    uint8_t Layer;
    float Radius;
    std::function<void(Collider* self, Collider* other)> OnCollision;
};

static const uint8_t CellSize = 64;

static void CreateSnapshots(
    std::vector<Collider>& colliders,
    std::unordered_map<uint64_t, std::vector<CollisionSnapshot*>>& spatialMap,
    std::vector<CollisionSnapshot>& snapshots);

static bool CheckCollision(const CollisionSnapshot& a, const CollisionSnapshot& b);

static uint64_t GetSpatialHashKey(int x, int y);

void Collider::Update(std::vector<Collider>& colliders, float deltaTime) {
    std::unordered_map<uint64_t, std::vector<CollisionSnapshot*>> spatialMap;
    std::vector<CollisionSnapshot> snapshots;
    snapshots.reserve(colliders.size());

    CreateSnapshots(colliders, spatialMap, snapshots);
    for (const CollisionSnapshot& snapshotA : snapshots) {
        const float R = snapshotA.Radius;
        const float x = snapshotA.GlobalPosition.x;
        const float y = snapshotA.GlobalPosition.y;

        const int gridXMin = static_cast<int>(std::floor((x - R) / CellSize));
        const int gridXMax = static_cast<int>(std::floor((x + R) / CellSize));
        const int gridYMin = static_cast<int>(std::floor((y - R) / CellSize));
        const int gridYMax = static_cast<int>(std::floor((y + R) / CellSize));

        for (int gx = gridXMin; gx <= gridXMax; ++gx) {
            for (int gy = gridYMin; gy <= gridYMax; ++gy) {
                const int64_t key = GetSpatialHashKey(gx, gy);
                auto it = spatialMap.find(key);
                if (it == spatialMap.end()) continue;

                for (CollisionSnapshot* snapshotB : it->second) {
                    if (snapshotA.OwnerId == snapshotB->OwnerId) continue; 
                    
                    if (CheckCollision(snapshotA, *snapshotB)) {
                        std::cout << "Collision between Entity " << snapshotA.OwnerId 
                                    << " and Entity " << snapshotB->OwnerId << "\n";
                        snapshotA.OnCollision(snapshotA.Origin, snapshotB->Origin);
                    }
                }
            }
        }
    }
}

static void CreateSnapshots(
    std::vector<Collider>& colliders,
    std::unordered_map<uint64_t, std::vector<CollisionSnapshot*>>& spatialMap,
    std::vector<CollisionSnapshot>& snapshots)
{
    for (Collider& collider : colliders) {
        Transform* transform = EntityManager::GetComponent<Transform>(collider.OwnerId);
        if (transform == nullptr || !transform->Enabled) {
            std::cout << "Could not fetch Transform component for entity " << collider.OwnerId << '\n';
            continue;
        }

        if (!collider.Enabled) { continue; }

        snapshots.push_back(CollisionSnapshot{
            &collider,
            collider.OwnerId,
            transform->GlobalLocation.Position,
            collider.Layer,
            collider.Radius,
            collider.OnCollision
        });
        CollisionSnapshot* snapshotPtr = &snapshots.back();

        const float R = snapshotPtr->Radius;
        const float x = snapshotPtr->GlobalPosition.x;
        const float y = snapshotPtr->GlobalPosition.y;

        const int gridXMin = static_cast<int>(std::floor((x - R) / CellSize));
        const int gridXMax = static_cast<int>(std::floor((x + R) / CellSize));
        const int gridYMin = static_cast<int>(std::floor((y - R) / CellSize));
        const int gridYMax = static_cast<int>(std::floor((y + R) / CellSize));

        for (int gx = gridXMin; gx <= gridXMax; gx++) {
            for (int gy = gridYMin; gy <= gridYMax; gy++) {
                const int64_t key = GetSpatialHashKey(gx, gy);
                spatialMap[key].push_back(snapshotPtr);
            }
        }
    }
}

static bool CheckCollision(const CollisionSnapshot& a, const CollisionSnapshot& b) {
    if (a.Layer != b.Layer) return false;

    const Math::Vector2 distance = a.GlobalPosition - b.GlobalPosition;
    const float distanceSquared = Math::LengthSqr(distance);

    const float radiiSum = a.Radius + b.Radius;
    const float radiiSumSq = radiiSum * radiiSum;

    return distanceSquared <= radiiSumSq;
}

static uint64_t GetSpatialHashKey(int x, int y) {
    return (static_cast<uint64_t>(x) << 32) | (static_cast<uint64_t>(y));
}