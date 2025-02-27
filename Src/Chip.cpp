#include "Chip.h"

Chip::Chip(int x, int y, int width, int height, int value, SDL_Texture* texture)
        : chipRect({x, y, width, height}), value(value), texture(texture) {}

void Chip::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &chipRect);
}

bool Chip::IsClicked(int mouseX, int mouseY) {
    return mouseX >= chipRect.x && mouseX <= chipRect.x + chipRect.w &&
           mouseY >= chipRect.y && mouseY <= chipRect.y + chipRect.h;
}

void Chip::SetPosition(int newX, int newY) {
    chipRect.x = newX;
    chipRect.y = newY;
}
