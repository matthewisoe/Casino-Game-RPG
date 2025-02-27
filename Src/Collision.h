
#ifndef GAME_COLLISION_H
#define GAME_COLLISION_H
#include "SDL.h"
#include "Vector2D.h"

class ColliderComponent;

class Collision {
public:
    static bool AABB(const SDL_FRect& recA, const SDL_FRect& recB);
    static bool AABB(const ColliderComponent& colA, ColliderComponent& colB);
};


#endif //GAME_COLLISION_H
