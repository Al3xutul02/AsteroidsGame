#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "../../math/tools.h"
#include "../../math/vector2.h"
#include "abstractions/component.h"

struct Sprite : Component {
    SDL_Rect Destination;
    std::string FilePath;
    float AngleOffset;
    float Angle;
    SDL_FlipMode FlipMode;
    uint8_t DrawOrder;
    float Opacity;

    inline Sprite(uint32_t ownerId,
        const SDL_Rect& destination,
        const std::string& filePath,
        float angleOffset = 0.0f,
        float angle = 0.0f,
        SDL_FlipMode flipMode = SDL_FLIP_NONE,
        uint8_t drawOrder = 0,
        float opacity = 1.0f) : Component(ownerId),
        Destination(destination), FilePath(filePath),
        AngleOffset(angleOffset), Angle(angle), FlipMode(flipMode),
        DrawOrder(drawOrder), Opacity(opacity) {}

    static void Update(std::vector<Sprite>& sprites, float deltaTime);
};