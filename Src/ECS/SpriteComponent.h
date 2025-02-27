//
// Created by Matt Irv on 25/11/24.
//

#ifndef GAME_SPRITECOMPONENT_H
#define GAME_SPRITECOMPONENT_H

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int speed = 100;

public:
    int animIndex = 0;

    std::map<const char*, Animation> animations;
    SpriteComponent() = default;
    SpriteComponent(const char* path){
        setTex(path);
    }
    SpriteComponent(const char* path, bool isAnimated){
        animated = isAnimated;

        // Define animations for walking in different directions
        animations.emplace("WalkDown", Animation(0, 3, 200));
        animations.emplace("WalkLeft", Animation(1, 3, 200));
        animations.emplace("WalkRight", Animation(2, 3, 200));
        animations.emplace("WalkUp", Animation(3, 3, 200));
        animations.emplace("IdleDown", Animation(4, 3, 200));
        animations.emplace("IdleLeft", Animation(5, 3, 200));
        animations.emplace("IdleRight", Animation(6, 3, 200));
        animations.emplace("IdleUp", Animation(7, 3, 200));

        Play("Idle");

        setTex(path);
    }
    ~SpriteComponent(){
        SDL_DestroyTexture(texture);
    }
    void setTex(const char* path){
        texture = TextureManager::LoadTexture(path);
    }
    void init() override{
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        if(animated){
            srcRect.w = transform->width;
            srcRect.h = transform->height;
        }else{
            srcRect.w = transform->width;
            srcRect.h = transform->height;
        }
    }

    void update() override {
        if (animated) {
            // Calculate the frame index based on animation speed
            srcRect.x = static_cast<int>(SDL_GetTicks() / speed % frames) * srcRect.w;

        }
        srcRect.y = animIndex * transform->height; // Determine row based on animation index

        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void Play(const char* animName) {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }


    void draw() override{
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
#endif //GAME_SPRITECOMPONENT_H
