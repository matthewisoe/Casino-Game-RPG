#ifndef SLOTMACHINEGAME_H
#define SLOTMACHINEGAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

extern Mix_Chunk* spinSound; // Declare the spinSound variable
extern Mix_Chunk* jackpotSound;

class SlotMachineGame {
public:
    SlotMachineGame();
    ~SlotMachineGame();
    TTF_Font* font;

    bool init(const char* title, int xpos, int ypos, int width, int height);
    bool betPlaced = false; // Tracks whether a bet has been placed
    void handleEvents();
    void update();
    void render();
    void clean();
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    SDL_Texture* CreateTextTexture(const std::string& text, SDL_Color color);
    SDL_Texture* loadTexture(const char* filePath);  // Declare loadTexture here

    bool running() const { return isRunning; }
    SDL_Texture* balanceTexture;
    SDL_Texture* betTexture;
    SDL_Color whiteColor = {255, 255, 255, 255};  // White color
    void setBalance(int balance){
        currentBalance = balance;
    }
    const int getBalance(){
        return currentBalance;
    }
private:
    SDL_Texture* betButtonPressedTexture = nullptr;
    SDL_Texture* betButtonTexture = nullptr;
    SDL_Texture* spinButtonPressedTexture = nullptr;
    SDL_Texture* spinButtonTexture = nullptr;

    int currentBalance;  // The player's current balance

    enum Symbol {
        CHERRY,
        LEMON,
        MELON,
        BAR,
        BELL,
        SEVEN,
        SYMBOL_COUNT
    };

    class SlotMachine {
    public:
        static const int REEL_COUNT = 3;

        SlotMachine(SDL_Renderer* renderer);
        ~SlotMachine();

        // Load assets for the slot machine
        void loadAssets();

        // Spin the reels
        std::vector<int> spin();

        // Render the slot machine UI
        void render(const std::vector<int>& symbols, bool buttonPressed);
        SDL_Texture* loadTexture(const char* filePath); // Move to public section

    private:
        SDL_Renderer* renderer;
        SDL_Texture* frameTexture;
        SDL_Texture* spinButtonTexture;
        SDL_Texture* betButtonTexture;
        SDL_Texture* betButtonPressedTexture;
        SDL_Texture* spinButtonPressedTexture;
        SDL_Texture* paylineTexture;
        SDL_Texture* textures[SYMBOL_COUNT] = {nullptr};

        // Render a single symbol
        void renderSymbol(int symbol, SDL_Rect rect);
    };

    // SDL objects for the game
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Game state variables
    bool isRunning = false;
    bool buttonPressed = false;
    bool isBetPlaced = false; // Track if a bet has been placed

    // Slot machine instance
    SlotMachine* slotMachine = nullptr;

    // Symbols currently displayed on the reels
    std::vector<int> symbols;
    bool isSpinning;
    Uint32 spinTime;       // Time when the spin started
    const int spinDuration = 2000; // Spin duration in milliseconds
    const int spinSpeed = 100;     // Speed of the spin (in ms between updates)

    // These are required for the reel count (number of reels in the slot machine)
    static const int REEL_COUNT = 3; // Assuming 3 reels
};

#endif // SLOTMACHINEGAME_H

