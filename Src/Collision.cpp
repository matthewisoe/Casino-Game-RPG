#include "Collision.h"
#include "ECS/ColliderComponent.h"
#include "Vector2D.h"

bool Collision::AABB(const SDL_FRect& recA, const SDL_FRect& recB) {
    return (recA.x + recA.w >= recB.x && recA.x <= recB.x + recB.w &&
            recA.y + recA.h >= recB.y && recA.y <= recB.y + recB.h);
}

bool Collision::AABB(const ColliderComponent& colA, ColliderComponent& colB) {
    return AABB(colA.collider, colB.collider);
}