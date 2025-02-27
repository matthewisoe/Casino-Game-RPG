#ifndef BALANCEADD_H
#define BALANCEADD_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class BalanceAddWindow {
public:
    BalanceAddWindow();
    ~BalanceAddWindow();

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void close();
    bool isRunning() const;
    void addMoney(int amount);  // Function to add money
    void setBalance(int amount);  // Set the current balance
    void renderText(const std::string& text, int x, int y, SDL_Color color);  // Helper function for rendering text
    void renderButton(SDL_Texture *buttonTexture, SDL_Rect &buttonRect);
    SDL_Texture *loadTexture(const char *file);
    int getBalance();
    void renderBackground();

private:
    void PlayCoinSound();
    void loadBackgroundImage();
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    int balance;
    SDL_Texture* backgroundTexture;
    SDL_Texture* balanceTextTexture;
    SDL_Texture* button50Texture;
    SDL_Texture* button100Texture;
    SDL_Texture* button200Texture;
    SDL_Texture* button500Texture;
    SDL_Texture* quitButtonTexture;
    TTF_Font* font;

    SDL_Rect button50Rect, button100Rect, button200Rect, button500Rect, quitButtonRect;

};

#endif
