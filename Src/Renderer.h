#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include "Card.h"
#include "Chip.h"

class Renderer {
private:
    SDL_Texture* tableTexture;
    void InitTableTexture();

public:
    SDL_Renderer* renderer;
    SDL_Texture* LoadTexture(const std::string& file);
    Renderer(SDL_Renderer* r);
    void RenderTable();
    void RenderCard(const Card& card, int x, int y, bool playFlipSound);
    void RenderHand(const std::vector<Card>& hand, int x, int y);
    void RenderText(const std::string& text, int x, int y, SDL_Color color, int fontSize);
    void RenderDealerHand(const std::vector<Card>& hand, int x, int y, bool secondCardFaceUp);
    void RenderCardBack(int x, int y);
    void RenderChips(const std::vector<Chip>& chips);
};
#endif // RENDERER_H
