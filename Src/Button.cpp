
#include "Button.h"
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>

Button::Button(int x, int y, int w, int h, SDL_Texture* texture)
        : buttonRect{x, y, w, h}, texture(texture) {}

bool Button::IsClicked(int mouseX, int mouseY) {
    return (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h);
}

SDL_Texture* Button::LoadTexture(const std::string& file) {
    SDL_Surface* tempSurface = IMG_Load(file.c_str());
    if (!tempSurface) {
        std::cerr << "Error loading image: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(SDL_GetWindowFromID(1)), tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void Button::SetAction(void (*action)()) {
    this->action = action;
}
void Button::UpdateTexture(SDL_Texture* newTexture) {
    texture = newTexture;
}

void Button::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &buttonRect);
}