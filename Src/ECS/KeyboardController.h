#ifndef GAME_KEYBOARDCONTROLLER_H
#define GAME_KEYBOARDCONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include <SDL_mixer.h>

class KeyboardController : public Component {
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    Mix_Chunk *footstepSound;  // Declare footstep sound variable
    Game& game;  // Reference to the Game instance

    // Constructor to initialize the game reference
    KeyboardController(Game& game) : game(game) {}

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();

        // Initialize the footstep sound
        footstepSound = Mix_LoadWAV("assets/sounds/footstep.mp3");
        if (footstepSound == nullptr) {
            std::cerr << "Failed to load footstep sound: " << Mix_GetError() << std::endl;
        }
    }

    void update() override {
        if (Game::event.type == SDL_KEYDOWN) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    transform->velocity.y = -1;
                    sprite->Play("WalkUp");
                    if (footstepSound) Mix_PlayChannel(-1, footstepSound, 0);  // Play sound on movement
                    game.sendPlayerMovement("WalkUp");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    sprite->Play("WalkLeft");
                    if (footstepSound) Mix_PlayChannel(-1, footstepSound, 0);
                    game.sendPlayerMovement("WalkLeft");
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    sprite->Play("WalkRight");
                    if (footstepSound) Mix_PlayChannel(-1, footstepSound, 0);
                    game.sendPlayerMovement("WalkRight");
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    sprite->Play("WalkDown");
                    if (footstepSound) Mix_PlayChannel(-1, footstepSound, 0);
                    game.sendPlayerMovement("WalkDown");
                    break;
                default:
                    break;
            }
        }

        if (Game::event.type == SDL_KEYUP) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    transform->velocity.y = 0;
                    sprite->Play("IdleUp");
                    game.sendPlayerMovement("IdleUp");
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    sprite->Play("IdleLeft");
                    game.sendPlayerMovement("IdleLeft");
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    sprite->Play("IdleRight");
                    game.sendPlayerMovement("IdleRight");
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    sprite->Play("IdleDown");
                    game.sendPlayerMovement("IdleDown");
                    break;
                default:
                    break;
            }
        }
    }
    // Optionally, add a cleanup function to free the sound when done
    void cleanup() {
        if (footstepSound) {
            Mix_FreeChunk(footstepSound);
            footstepSound = nullptr;
        }
    }
};

#endif //GAME_KEYBOARDCONTROLLER_H
