#include "SlotMachineGame.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "LanguageManager.h"

Mix_Chunk* spinSound = nullptr;
Mix_Chunk* jackpotSound = nullptr;

const char* QUIT_BUTTON = "assets/PNG/Button/xButton.png";
SDL_Texture* quitButtonTexture = nullptr;
SDL_Rect quitButtonRect = {10, 10, 50, 50}; // Position and size of the quit button

const char* WIN_IMAGE = "assets/50win.png";
SDL_Texture* winTexture = nullptr; // texture for the win image
int selectedBet = 10;  //  player's current bet
int spinSpeed = 10; // Fast:25 Slow :100

bool isWin = false; // Tracks if the spin is a win
bool betPlaced = false; // Tracks if a bet has been placed
bool spinButtonPressed = false;
bool betButtonPressed = false;
bool isSpinning = false;
Uint32 spinStartTime = 0;  // Start time for the spin effect
const int SPIN_DURATION = 2000;  //
Uint32 winDisplayStartTime = 0; // Start time
const Uint32 WIN_DISPLAY_DURATION = 2000; //  to show the win image for 2 seconds

const char* FRAME_IMAGE = "assets/SMFrame_blank.png";
const char* SPIN_BUTTON = "assets/spin.png";
const char* SPIN_BUTTON_PRESSED = "assets/spin_pressed.png";
const char* PAYLINE_FRAME = "assets/paylineframe.png";
const char* BET_BUTTON = "assets/bet.png";
const char* BET_BUTTON_PRESSED = "assets/bet_pressed.png";

const char* symbolImages[] = {
    "assets/cherries.png",
    "assets/lemon.png",
    "assets/melon.png",
    "assets/Bar1.png",
    "assets/bell.png",
    "assets/Lucky7_rainbow.png"
};

// Constructor
SlotMachineGame::SlotMachineGame()
    : slotMachine(nullptr), symbols(SlotMachine::REEL_COUNT, 0),  // Ensure REEL_COUNT is referenced correctly
      isSpinning(false), spinTime(0) {  // Initialize spinTime to 0, isSpinning to false
}
// Destructor
SlotMachineGame::~SlotMachineGame() {
    delete slotMachine;
}

// Initialize the game
bool SlotMachineGame::init(const char* title, int xpos, int ypos, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("assets/font/NotoSansSC-VariableFont_wght.ttf", 24);  // Replace with the actual font path
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }

    slotMachine = new SlotMachine(renderer);
    slotMachine->loadAssets();

    // Load quit button texture
    quitButtonTexture = slotMachine->loadTexture(QUIT_BUTTON);
    if (!quitButtonTexture) {
        std::cerr << "Failed to load quit button texture: " << IMG_GetError() << std::endl;
        return false;
    }
    // Load win image texture
    winTexture = slotMachine->loadTexture(WIN_IMAGE);
    if (!winTexture) {
        std::cerr << "Failed to load win image texture: " << IMG_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load spin sound
    spinSound = Mix_LoadWAV("assets/sounds/slotssound.wav");
    if (!spinSound) {
        std::cerr << "Failed to load spin sound: " << Mix_GetError() << std::endl;
        return false;
    }
    // Load the jackpot sound
    jackpotSound = Mix_LoadWAV("assets/sounds/jackpotsound.mp3");
    if (!jackpotSound) {
        std::cerr << "Failed to load jackpot sound: " << Mix_GetError() << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}
void SlotMachineGame::renderText(const std::string& message, int x, int y, SDL_Color color) {
SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, message.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    int textWidth = textSurface->w;  // Store width
    int textHeight = textSurface->h; // Store height

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect textRect = {x, y, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}


// Handle input events
void SlotMachineGame::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Check if the quit button was clicked
            if (mouseX >= quitButtonRect.x && mouseX <= quitButtonRect.x + quitButtonRect.w &&
                mouseY >= quitButtonRect.y && mouseY <= quitButtonRect.y + quitButtonRect.h) {
                isRunning = false; // Quit the game
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                std::cout << "Quit button clicked. Returning to the map!" << std::endl;
                return;
                }

            // Detect click on Bet button only if a bet has not already been placed
            if (mouseX >= 280 && mouseX <= 400 && mouseY >= 465 && mouseY <= 565) {
                if (!betPlaced) {  // Bet can only be placed if no bet has been placed
                    betButtonPressed = true;
                    if (currentBalance >= selectedBet) {
                        currentBalance -= selectedBet; // Deduct bet amount
                        betPlaced = true;             // Set bet flag
                        std::cout << "Bet placed! Remaining balance: $" << currentBalance << std::endl;
                    } else {
                        std::cout << "Insufficient Balance to Bet!" << std::endl;
                    }
                }
            }

            // Detect click on Spin button
            if (mouseX >= 397 && mouseX <= 517 && mouseY >= 465 && mouseY <= 565) {
                if (betPlaced && !isSpinning) { // Spin only if a bet was placed
                    isSpinning = true;
                    spinTime = SDL_GetTicks(); // Start the timer
                    betPlaced = false;        // Reset the bet flag after starting a spin
                    Mix_PlayChannel(-1, spinSound, 0); // Play the spin sound
                    std::cout << "Spinning!" << std::endl;
                } else if (!betPlaced) {
                    std::cout << "You must place a bet before spinning!" << std::endl;
                }
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            betButtonPressed = false;
        }
    }
}

void SlotMachineGame::render() {
    // Clear the screen with a specific color
    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255); // Dark red
    SDL_RenderClear(renderer);

    // Render the slot machine frame and symbols
    slotMachine->render(symbols, isSpinning);

    // Render the quit button
    SDL_RenderCopy(renderer, quitButtonTexture, nullptr, &quitButtonRect);

    // Ensure there's enough space for the Bet and Balance text on the left side
    int textXPosition = 10; // Left-aligned
    int textYPositionBet = 520; // Adjust this to avoid overlap
    int textYPositionBalance = 550; // Adjust this to avoid overlap

    // Get localized strings
    std::string betLabel = LanguageManager::getInstance().getLocalizedResource("betText");
    std::string balanceLabel = LanguageManager::getInstance().getLocalizedResource("balanceText");

    // Render Bet and Balance labels
    SDL_Color whiteColor = {255, 255, 255, 255};

    renderText(betLabel + std::to_string(selectedBet), 10, 520, whiteColor);
    renderText(balanceLabel + std::to_string(currentBalance), 10, 550, whiteColor);
    if (isWin && winTexture) {
        SDL_Rect winImageRect = {200, 150, 400, 200}; // Adjust as needed
        SDL_RenderCopy(renderer, winTexture, nullptr, &winImageRect);
    }
    // Render Bet button (disabled if bet has been placed)
    SDL_Rect betButtonRect = {280, 465, 120, 100};
    if (betButtonPressed || betPlaced) {
        if (betButtonPressedTexture) {
            SDL_RenderCopy(renderer, betButtonPressedTexture, nullptr, &betButtonRect); // Show pressed texture if button is clicked or disabled
        }
    } else {
        if (betButtonTexture) {
            SDL_RenderCopy(renderer, betButtonTexture, nullptr, &betButtonRect); // Regular button texture
        }
    }

    // Render Spin button (show pressed texture when spinning)
    SDL_Rect spinButtonRect = {397, 465, 120, 100};
    if (isSpinning) {
        if (spinButtonPressedTexture) {
            SDL_RenderCopy(renderer, spinButtonPressedTexture, nullptr, &spinButtonRect);
        }
    } else {
        if (spinButtonTexture) {
            SDL_RenderCopy(renderer, spinButtonTexture, nullptr, &spinButtonRect);
        }
    }

    // Present the renderer to the screen
    SDL_RenderPresent(renderer);
}


// Update game state
void SlotMachineGame::update() {
    if (isSpinning) {
        int elapsedTime = SDL_GetTicks() - spinTime;

        // Update the reels every spinSpeed milliseconds
        if (elapsedTime % spinSpeed == 0) {
            for (int i = 0; i < REEL_COUNT; ++i) {
                symbols[i] = rand() % SYMBOL_COUNT;
            }
        }

        // Stop spinning after the duration
        if (elapsedTime >= SPIN_DURATION) {
            isSpinning = false; // Stop spinning

            // Stop the sound after spin duration
            Mix_HaltChannel(-1);
            // Introduce a higher chance for winning
            int winChance = rand() % 100; // Generate a number between 0 and 99

            if (winChance < 30) { // 30% chance to force a win
                int winningSymbol = rand() % SYMBOL_COUNT; // Pick a random symbol
                for (int i = 0; i < REEL_COUNT; ++i) {
                    symbols[i] = winningSymbol; // Force all reels to the same symbol
                }
            } else {
                // Finalize symbols randomly if no forced win
                for (int i = 0; i < REEL_COUNT; ++i) {
                    symbols[i] = rand() % SYMBOL_COUNT;
                }
            }

            // Check if all symbols are the same
            isWin = true; // Use the class-level variable
            for (int i = 1; i < REEL_COUNT; ++i) {
                if (symbols[i] != symbols[0]) {
                    isWin = false; // Update the class-level variable
                    break;
                }
            }


            if (isWin) {
                int winAmount = selectedBet * 5;  // Win amount is 50
                currentBalance += winAmount;
                std::cout << "You won! Your balance is now: $" << currentBalance << std::endl;

                // Start the timer for the win image
                winDisplayStartTime = SDL_GetTicks();
            }
            if (isWin) {
                if (SDL_GetTicks() - winDisplayStartTime < WIN_DISPLAY_DURATION) {
                    if (!Mix_Playing(-1)) { // Ensure the sound is only played once
                        Mix_PlayChannel(-1, jackpotSound, 0); // Play the jackpot sound
                    }
                } else {
                    isWin = false; // Stop showing the win image after the duration
                    Mix_HaltChannel(-1); // Stop playing the jackpot sound
                }
            }
        }
    }

    // Disable the win image after 2 seconds
    if (isWin && SDL_GetTicks() - winDisplayStartTime >= WIN_DISPLAY_DURATION) {
        isWin = false;
    }
}


void SlotMachineGame::clean() {
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(winTexture);
    TTF_CloseFont(font);
    delete slotMachine;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (spinSound) {
        Mix_FreeChunk(spinSound);
    }
    if (jackpotSound) {
        Mix_FreeChunk(jackpotSound);
        jackpotSound = nullptr;
    }
    Mix_CloseAudio();

}

// SlotMachine class
SlotMachineGame::SlotMachine::SlotMachine(SDL_Renderer* renderer) : renderer(renderer) {}

SlotMachineGame::SlotMachine::~SlotMachine() {
    for (auto texture : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(spinButtonTexture);
    SDL_DestroyTexture(spinButtonPressedTexture);
    SDL_DestroyTexture(betButtonTexture);
    SDL_DestroyTexture(betButtonPressedTexture);
    SDL_DestroyTexture(paylineTexture);
}

void SlotMachineGame::SlotMachine::loadAssets() {
    auto& langManager = LanguageManager::getInstance();
    frameTexture = loadTexture(FRAME_IMAGE);
    spinButtonTexture = loadTexture(langManager.getLocalizedResource("spin_button").c_str());
    spinButtonPressedTexture = loadTexture(langManager.getLocalizedResource("spin_button_pressed").c_str());
    paylineTexture = loadTexture(PAYLINE_FRAME);
    betButtonTexture = loadTexture(langManager.getLocalizedResource("bet_button").c_str());
    betButtonPressedTexture = loadTexture(langManager.getLocalizedResource("bet_button_pressed").c_str());

    for (int i = 0; i < SYMBOL_COUNT; ++i) {
        textures[i] = loadTexture(symbolImages[i]);
        if (!textures[i]) {
            std::cerr << "Failed to load texture for symbol: " << IMG_GetError() << std::endl;
        }
    }
}

std::vector<int> SlotMachineGame::SlotMachine::spin() {
    std::vector<int> result;
    for (int i = 0; i < REEL_COUNT; ++i) {
        result.push_back(rand() % SYMBOL_COUNT);
    }
    return result;
}

void SlotMachineGame::SlotMachine::render(const std::vector<int>& symbols, bool isSpinning) {
    // Render frame
    SDL_RenderCopy(renderer, frameTexture, nullptr, nullptr);

    // Set the spacing between each symbol
    int symbolSpacing = 50;  // Increased space between symbols

    // Set a new starting position for the symbols (e.g., move them left by 20 pixels)
    int initialXPosition = 70;  // This moves the symbols to the left

    // Render symbols with adjusted spacing and position
    for (int i = 0; i < REEL_COUNT; ++i) {
        SDL_Rect slotRect = {
            initialXPosition + i * (200 + symbolSpacing),  // Adjusted x-position and spacing
            180,  // Adjusted vertical position
            180,  // Slot width
            160   // Slot height
        };
        renderSymbol(symbols[i], slotRect);
    }

    // Render spin button
    SDL_Rect spinButtonRect = {397, 465, 120, 100};
    if (isSpinning) {
        SDL_RenderCopy(renderer, spinButtonPressedTexture, nullptr, &spinButtonRect);
    } else {
        SDL_RenderCopy(renderer, spinButtonTexture, nullptr, &spinButtonRect);
    }

    // Render bet button
    SDL_Rect betButtonRect = {280, 465, 120, 100}; // Position and size for Bet button
    if (betButtonPressed) {
        SDL_RenderCopy(renderer, betButtonPressedTexture, nullptr, &betButtonRect);
    } else {
        SDL_RenderCopy(renderer, betButtonTexture, nullptr, &betButtonRect);
    }
}

SDL_Texture* SlotMachineGame::SlotMachine::loadTexture(const char* filePath) {
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void SlotMachineGame::SlotMachine::renderSymbol(int symbol, SDL_Rect slotRect) {
    if (textures[symbol]) {
        // Query texture width and height
        int textureWidth, textureHeight;
        SDL_QueryTexture(textures[symbol], nullptr, nullptr, &textureWidth, &textureHeight);

        // Scale symbol to fit within the slot (keeping aspect ratio)
        float scaleX = static_cast<float>(slotRect.w) / static_cast<float>(textureWidth);
        float scaleY = static_cast<float>(slotRect.h) / static_cast<float>(textureHeight);
        float scale = std::min(scaleX, scaleY); // Choose the smaller scale to avoid stretching

        int scaledWidth = static_cast<int>(textureWidth * scale);
        int scaledHeight = static_cast<int>(textureHeight * scale);

        // Center the scaled symbol in the slot
        SDL_Rect symbolRect = {
            slotRect.x + (slotRect.w - scaledWidth) / 2,  // Center horizontally
            slotRect.y + (slotRect.h - scaledHeight) / 2, // Center vertically
            scaledWidth,                                  // Scaled symbol width
            scaledHeight                                  // Scaled symbol height
        };

        // Render the symbol
        SDL_RenderCopy(renderer, textures[symbol], nullptr, &symbolRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fallback white box
        SDL_RenderFillRect(renderer, &slotRect);
    }
}