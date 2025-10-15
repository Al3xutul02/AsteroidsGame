#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../ecs/components/sprite.h"

class Application;

class RenderManager {
public:
    static void Initialize(Application* app, SDL_Window* window);
    static void Shutdown();
    
    static void SetSprites(std::vector<Sprite>& sprites);
    static void LoadTextures(const std::vector<std::string>& files);
    static void Draw();

private:
    static Application* mApp;
    static SDL_Renderer* mRenderer;

    static std::vector<Sprite>* mSprites;
    static std::unordered_map<std::string, SDL_Texture*> mTextureMap;
};