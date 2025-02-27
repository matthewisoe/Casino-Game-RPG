#include "Renderer.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_mixer.h>
#include "LanguageManager.h"

extern Mix_Chunk* cardFlipSound;

Renderer::Renderer(SDL_Renderer* r) : renderer(r) {
    InitTableTexture();
}

void Renderer::InitTableTexture() {
    LanguageManager& langManager = LanguageManager::getInstance();
    std::string currentLanguage = langManager.getLanguage();

    // Fetch the localized table image based on the current language
    std::string tableImagePath = langManager.getLocalizedResource("blackjackTable");

    // Default to English version if no localized resource is available
    if (tableImagePath.empty()) {
        tableImagePath = "assets/table_image/blackjacktable.png";
    }
    // Load the texture into tableTexture
    tableTexture = LoadTexture(tableImagePath); // Now we load the texture
    if (!tableTexture) {
        std::cerr << "Error loading table texture: " << SDL_GetError() << std::endl;
    }
}

SDL_Texture* Renderer::LoadTexture(const std::string& file) {
    SDL_Surface* tempSurface = IMG_Load(file.c_str());
    if (!tempSurface) {
        std::cerr << "Error loading image: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void Renderer::RenderDealerHand(const std::vector<Card>& hand, int x, int y, bool secondCardFaceUp) {
    if (hand.size() > 0) {
        // Render the first card (always visible)
        RenderCard(hand[0], x, y,false);

        // If the dealer's second card is face-up, render it
        if (secondCardFaceUp) {
            RenderCard(hand[1], x + 30, y,false);  // Adjust the x position as needed
        }
            // If the second card is face-down, render a placeholder (back of card)
        else {
            RenderCardBack(x + 30, y);  // Draw the back of the card
        }
    }
}

void Renderer::RenderCardBack(int x, int y) {
    std::string imageForBack = "assets/cards/back_black_basic.png";
    SDL_Texture* backCardTexture = LoadTexture(imageForBack);
    if (!backCardTexture) return;

    SDL_Rect rect = {x, y, 80, 120};
    SDL_RenderCopy(renderer, backCardTexture, nullptr, &rect);

    SDL_DestroyTexture(backCardTexture);
}

void Renderer::RenderCard(const Card& card, int x, int y, bool playFlipSound) {
    std::string imagePath = "assets/cards/" + card.rank + "_" + card.suit + "_white.png";
    SDL_Texture* cardTexture = LoadTexture(imagePath);
    if (!cardTexture) return;

    SDL_Rect rect = {x, y, 80, 120};  // Adjust dimensions as needed
    SDL_RenderCopy(renderer, cardTexture, nullptr, &rect);
    // Play flip sound if requested
    if (playFlipSound) {
        Mix_PlayChannel(-1, cardFlipSound, 0);
    }
    SDL_DestroyTexture(cardTexture);  // Clean up after rendering the card
}

void Renderer::RenderHand(const std::vector<Card>& hand, int x, int y) {
    int offset = 30; // Adjust offset for spacing
    for (const auto& card : hand) {
        RenderCard(card, x, y,false);
        x += offset;
    }
}


void Renderer::RenderTable() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // Define table size based on texture's dimensions
    int tableWidth = 800;
    int tableHeight = 600;

    // Calculate the position to center the table
    int x = (windowWidth - tableWidth) / 2;
    int y = (windowHeight - tableHeight) / 2;

    SDL_Rect tableRect = { x, y, tableWidth, tableHeight };

    // Render the table texture
    SDL_RenderCopy(renderer, tableTexture, nullptr, &tableRect);  // Rendering the texture
}

void Renderer::RenderText(const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    // Use SDL_ttf to render text
    TTF_Font* font = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 24);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }


    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Error creating text surface: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);  // Render the text

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}
