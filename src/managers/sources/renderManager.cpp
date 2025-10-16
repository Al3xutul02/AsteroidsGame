#include "../renderManager.h"
#include <iostream>
#include <map>
#include "../../application.h"

Application* RenderManager::mApp = nullptr;
SDL_Renderer* RenderManager::mRenderer = nullptr;
std::vector<Sprite>* RenderManager::mSprites = nullptr;
std::unordered_map<std::string, SDL_Texture*> RenderManager::mTextureMap = std::unordered_map<std::string, SDL_Texture*>();

void RenderManager::Initialize(Application* app, SDL_Window* window) {
    mApp = app;
    mRenderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
}

void RenderManager::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    
    mApp = nullptr;
    mRenderer = nullptr;
}

void RenderManager::SetSprites(std::vector<Sprite>& sprites) {
    mSprites = &sprites;
}

void RenderManager::LoadTextures(const std::vector<std::string>& files) {
    mTextureMap.clear();
    for(const std::string& file : files) {
        SDL_Texture* texture = IMG_LoadTexture(mRenderer, &(file[0]));
        bool success = SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);
        if (!success) {
            std::cout << "Failed to set texture scale mode: " << SDL_GetError() << '\n';
            continue;
        }

        mTextureMap[file] = texture;
    }
}

void RenderManager::Draw() {
    SDL_RenderClear(mRenderer);

    if (mSprites != nullptr) {
        std::map<uint8_t, std::vector<Sprite*>> orderedSprites;
        for (Sprite& sprite : *mSprites) { orderedSprites[sprite.DrawOrder].push_back(&sprite); }

        for (auto& [order, sprites] : orderedSprites) {
            for (Sprite* const sprite : sprites) {
                if (!sprite->Enabled) { continue; }

                SDL_FRect fDestination;
                SDL_RectToFRect(&sprite->Destination, &fDestination);
                
                SDL_SetTextureAlphaModFloat(mTextureMap[sprite->FilePath], sprite->Opacity);
                bool success = SDL_RenderTextureRotated(
                    mRenderer,
                    mTextureMap[sprite->FilePath],
                    NULL,
                    &fDestination,
                    sprite->AngleOffset + sprite->Angle,
                    NULL,
                    sprite->FlipMode
                );
                if (!success) {
                    std::cout << "Error while rendering texture: " << SDL_GetError() << '\n';
                    continue;
                }
            }
        }
    }
    else {
        std::cout << "No sprite list reference inside the render manager\n";
    }

    SDL_RenderPresent(mRenderer);
}