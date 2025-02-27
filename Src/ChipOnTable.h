#ifndef CHIP_ON_TABLE_H
#define CHIP_ON_TABLE_H

#include <vector>
#include "Chip.h"
#include <SDL.h>
#include <SDL_mixer.h>

// ChipOnTable class declaration
class ChipOnTable {
public:
    // Constructor
    ChipOnTable();

    // to add a chip to the table
    void AddChip(SDL_Texture* chipTexture);

    // render all chips on the table
    void RenderChips(SDL_Renderer* renderer);

    void RenderWinningChips(SDL_Renderer* renderer);

    // remove all chips from the table
    void RemoveChips();

private:
    std::vector<Chip*> tempChips;  // Vector to store chips currently placed on the table
    int x, y;
    SDL_Texture* texture;  // Texture for the chips
    void PlayCoinSound();  // Declare PlayCoinSound here

};

#endif // CHIP_ON_TABLE_H