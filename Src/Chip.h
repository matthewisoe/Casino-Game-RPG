#pragma once
#include <SDL.h>
#include <string>

class Chip {
public:
    Chip(int x, int y, int width, int height, int value, SDL_Texture* texture);
    void Render(SDL_Renderer* renderer);
    bool IsClicked(int mouseX, int mouseY);
    void SetPosition(int newX, int newY);
    int GetValue() const { return value; }
    SDL_Texture* GetTexture() const { return texture; }
    int value;
private:
    SDL_Rect chipRect;      // Rectangle for the chip's position and size
    SDL_Texture* texture;   // Texture of the chip

};
