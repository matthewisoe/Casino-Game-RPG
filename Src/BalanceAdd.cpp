#include "BalanceAdd.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "LanguageManager.h"
#include <string>

Mix_Chunk* coinSound = nullptr;

// Constructor & Destructor
BalanceAddWindow::BalanceAddWindow()
        : balanceTextTexture(nullptr),
          button50Texture(nullptr), button100Texture(nullptr),
          button200Texture(nullptr), button500Texture(nullptr),
          quitButtonTexture(nullptr),
          renderer(nullptr), window(nullptr), font(nullptr), running(false), backgroundTexture(nullptr) {}

BalanceAddWindow::~BalanceAddWindow() {
    // Destroy textures
    SDL_DestroyTexture(balanceTextTexture);
    SDL_DestroyTexture(button50Texture);
    SDL_DestroyTexture(button100Texture);
    SDL_DestroyTexture(button200Texture);
    SDL_DestroyTexture(button500Texture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(backgroundTexture);

    // Destroy renderer and window
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }

    // Close font and quit SDL subsystems
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_Quit();
}

bool BalanceAddWindow::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initialize SDL and TTF
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    // Create window
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }


    coinSound = Mix_LoadWAV("assets/sounds/coinsound.mp3");
    if (!coinSound) {
            SDL_Log("Failed to load coin sound: %s", Mix_GetError());
            return 0;
    }

    loadBackgroundImage();

    // Load button textures (Replace with actual image paths)
    button50Texture = loadTexture("assets/button_image/button50.png");
    button100Texture = loadTexture("assets/button_image/button100.png");
    button200Texture = loadTexture("assets/button_image/button200.png");
    button500Texture = loadTexture("assets/button_image/button500.png");
    quitButtonTexture = loadTexture("assets/button_image/xButton.png");

    // Initialize button positions
    button50Rect = {180, 340, 100, 50};
    button100Rect = {300, 370, 100, 50};
    button200Rect = {420, 370, 100, 50};
    button500Rect = {540, 340, 100, 50};
    quitButtonRect = {10, 10, 50, 50};  // Top-left corner for the Quit button

    running = true;
    return true;
}

// Helper function to load an image as a texture
SDL_Texture* BalanceAddWindow::loadTexture(const char* file) {
    SDL_Surface* loadedSurface = IMG_Load(file);  // Use IMG_Load for different image formats
    if (loadedSurface == nullptr) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);  // Convert surface to texture
    SDL_FreeSurface(loadedSurface);  // Free the surface as it's no longer needed
    return texture;
}

// Helper function to render images (buttons)
void BalanceAddWindow::renderButton(SDL_Texture* buttonTexture, SDL_Rect& buttonRect) {
    SDL_RenderCopy(renderer, buttonTexture, nullptr, &buttonRect);
}

void BalanceAddWindow::renderBackground() {
    if (backgroundTexture != nullptr) {
        // Render the background image (covering the entire window)
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    } else {
        std::cerr << "Failed to load background image!" << std::endl;
    }
}
void BalanceAddWindow::loadBackgroundImage() {
    std::string language = LanguageManager::getInstance().getCurrentLanguage();

    if (language == "zh") {  // For Chinese
        backgroundTexture = loadTexture("assets/background/barBackgroundCH.png");
        if (backgroundTexture == nullptr) {
            std::cerr << "Failed to load Chinese background!" << std::endl;  // Debugging line
        }
    } else {  // Default background
        backgroundTexture = loadTexture("assets/background/barBackground.png");
        if (backgroundTexture == nullptr) {
            std::cerr << "Failed to load default background!" << std::endl;  // Debugging line
        }
    }
}

void BalanceAddWindow::renderText(const std::string& message, int x, int y, SDL_Color color) {

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, message.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderUTF8_Blended Error: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }
    int textWidth = textSurface->w;  // Store width
    int textHeight = textSurface->h; // Store height


    SDL_Rect textRect = {x, y, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}

void BalanceAddWindow::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {  // Loop through all events in the queue
        if (event.type == SDL_QUIT) {
            running = false;  // Close the window if the user wants to quit
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            // Create SDL_Point for mouse position
            SDL_Point mousePos = {x, y};

            // Check if the mouse click is within any of the buttons
            if (SDL_PointInRect(&mousePos, &quitButtonRect)) {
                running = false;  // Exit the program when the Quit button is clicked
            } else if (SDL_PointInRect(&mousePos, &button50Rect)) {
                Mix_PlayChannel(-1, coinSound, 0); // Play coin sound
                addMoney(50);  // Add $50 to balance
            } else if (SDL_PointInRect(&mousePos, &button100Rect)) {
                Mix_PlayChannel(-1, coinSound, 0); // Play coin sound
                addMoney(100);  // Add $100 to balance
            } else if (SDL_PointInRect(&mousePos, &button200Rect)) {
                Mix_PlayChannel(-1, coinSound, 0); // Play coin sound
                addMoney(200);  // Add $200 to balance
            } else if (SDL_PointInRect(&mousePos, &button500Rect)) {
                Mix_PlayChannel(-1, coinSound, 0); // Play coin sound
                addMoney(500);  // Add $500 to balance
            }
        }
    }
}

void BalanceAddWindow::update() {
}

void BalanceAddWindow::render() {

    if (renderer == nullptr) {
        std::cerr << "Renderer is not initialized!" << std::endl;
        return;
    }

    // Clear the renderer first to avoid flickering
    SDL_RenderClear(renderer);  // Clear the screen with the current drawing color

    SDL_SetRenderDrawColor(renderer, 161, 120, 67, 255);  // Set background color
    SDL_Rect topRect = {0,0, 800, 300};
    SDL_RenderFillRect(renderer, &topRect);

    SDL_SetRenderDrawColor(renderer, 11, 0, 135, 255);
    SDL_Rect bottomRect = {0, 300, 800, 300};
    SDL_RenderFillRect(renderer, &bottomRect);

    SDL_SetRenderDrawColor(renderer, 135, 0, 11, 255);
    SDL_Rect leftRect = {0, 300, 200, 300};
    SDL_RenderFillRect(renderer, &leftRect);

    SDL_SetRenderDrawColor(renderer, 135, 0, 11, 255);
    SDL_Rect rightRect = {650, 300, 200, 300};
    SDL_RenderFillRect(renderer, &rightRect);

    renderBackground();

    // Retrieve localized text
    std::string balanceLabel = LanguageManager::getInstance().getLocalizedResource("currentbalanceText");
    if (balanceLabel.empty()) {
        balanceLabel = "Current Balance: $"; // Fallback to default
    }
    std::string balanceText = balanceLabel + std::to_string(balance);
    renderBackground();

    renderText(balanceText, 10, 560, {255, 255, 255});

    // Render the Quit button
    renderButton(quitButtonTexture, quitButtonRect);

    // Render other buttons
    renderButton(button50Texture, button50Rect);
    renderButton(button100Texture, button100Rect);
    renderButton(button200Texture, button200Rect);
    renderButton(button500Texture, button500Rect);

    SDL_RenderPresent(renderer);  // Present the rendered content
}


void BalanceAddWindow::close() {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

bool BalanceAddWindow::isRunning() const {
    return running;
}

void BalanceAddWindow::addMoney(int amount) {
    if (amount > 0) {
        balance += amount;  // Add the money to the balance
    }
}

void BalanceAddWindow::setBalance(int amount) {
    balance = amount;  // Set the initial balance to a specific amount
}

int BalanceAddWindow::getBalance() {
    return balance;
}
