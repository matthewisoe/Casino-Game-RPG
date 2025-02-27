#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Renderer;

class Button {
public:
    Button(int x, int y, int w, int h, SDL_Texture* texture);
    bool IsClicked(int mouseX, int mouseY);
    void Render(SDL_Renderer* renderer);
    void SetAction(void (*action)());
    void setTexture(SDL_Texture* newTexture) {
        texture = newTexture;  // Set the private texture member
    }
    void UpdateTexture(SDL_Texture* newTexture);

private:
    SDL_Rect buttonRect;
    SDL_Texture* texture;
    void (*action)();

    SDL_Texture* LoadTexture(const std::string& file);
    void RenderText(SDL_Renderer* renderer);
};

#endif