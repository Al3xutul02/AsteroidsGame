#include "../sprite.h"
#include <iostream>
#include <algorithm>
#include "../../../managers/entityManager.h"
#include "../../../managers/renderManager.h"

void Sprite::Update(std::vector<Sprite>& sprites, float deltaTime) {
    for (Sprite& sprite : sprites) {
        Transform* transform = EntityManager::GetComponent<Transform>(sprite.OwnerId);
        if (transform == nullptr || !transform->Enabled) {
            std::cout << "Could not fetch Transform component for entity " << sprite.OwnerId << '\n';
            continue;
        }

        if (!sprite.Enabled) { continue; }
        
        sprite.Destination.x = (int)(transform->GlobalLocation.Position.x - sprite.Destination.w / 2.0f);
        sprite.Destination.y = (int)(transform->GlobalLocation.Position.y - sprite.Destination.h / 2.0f);
        sprite.Angle = transform->GlobalRotation.Angle;
        sprite.Opacity = std::clamp(sprite.Opacity, 0.0f, 1.0f);
    }

    RenderManager::SetSprites(sprites);
}