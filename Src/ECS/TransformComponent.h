#ifndef GAME_POSITIONCOMPONENT_H
#define GAME_POSITIONCOMPONENT_H

#include "Components.h"
#include "../Vector2D.h"
#include <iostream> //

class TransformComponent : public Component {
private:
    int xpos;
    int ypos;

public:
    Vector2D position;        // Current position
    Vector2D velocity;        // Current velocity
    Vector2D cposition;       // Centered position

    Vector2D lastPosition;    // Position before the update

    int height = 240;         // Default height of the entity
    int width = 108;          // Default width of the entity
    int cheight = 6;          // Centered height
    int cwidth = 16;          // Centered width
    int scale = 1;            // Scaling factor
    int speed = 2;            // Speed multiplier
    bool blocked = false;     // Movement block flag

    TransformComponent() {
        position.Zero();
        lastPosition.Zero();
    }

    TransformComponent(float x, float y) {
        position.x = x;
        position.y = y;
        cposition.x = x;
        cposition.y = y;
        lastPosition.Zero();
    }

    TransformComponent(int h, int w) {
        this->height = h;
        this->width = w;
        lastPosition.Zero();
    }

    TransformComponent(float x, float y, int h, int w, int sc) {
        position.x = x;
        position.y = y;
        cposition.x = x;
        cposition.y = y;
        this->height = h;
        this->width = w;
        cheight = h;
        cwidth = w;
        scale = sc;
        lastPosition.Zero();
    }

    TransformComponent(float x, float y, float cx, float cy, int h, int w, int ch, int cw, int sc) {
        position.x = x;
        position.y = y;
        cposition.x = cx;
        cposition.y = cy;
        this->height = h;
        this->width = w;
        cheight = ch;
        cwidth = cw;
        scale = sc;
        lastPosition.Zero();
    }

    void init() override {
        velocity.Zero();
        lastPosition = position;
    }

    void update() override {
        // Save the current position before updating
        lastPosition = position;

        if (!blocked) {  // Update position if not blocked
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
            cposition.x += velocity.x * speed;
            cposition.y += velocity.y * speed;
        }
    }

    // Optional method to retrieve the last position
    Vector2D getLastPosition() const {
        return lastPosition;
    }
};

#endif // GAME_POSITIONCOMPONENT_H
