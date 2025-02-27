#ifndef GAME_COLLIDERCOMPONENT_H
#define GAME_COLLIDERCOMPONENT_H
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component{
public:
    SDL_FRect collider;
    std::string tag;

    TransformComponent* transform;

    // Constructor to set the tag
    ColliderComponent(const std::string t){
        tag = t;
    }

    void init() override{
        if(!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
        Game::colliders.push_back(this);
    }

    void update() override{
        collider.x = static_cast<float>(transform->cposition.x);
        collider.y = static_cast<float>(transform->cposition.y);
        collider.w = transform->cwidth * transform->scale;
        collider.h = transform->cheight * transform->scale;
    }
};
#endif //GAME_COLLIDERCOMPONENT_H
