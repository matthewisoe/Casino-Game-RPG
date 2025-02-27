#include "ChipOnTable.h"
#include <SDL_mixer.h>

// Constructor to initialize position and texture
ChipOnTable::ChipOnTable() {}

// Add a new chip to the table
void ChipOnTable::AddChip(SDL_Texture* chipTexture) {
    // Create a new chip with the fixed size and position on the x-axis
    Chip* newChip = new Chip(0, 0, 50, 50, 0, chipTexture);  // Fixed size (40x40)
    tempChips.push_back(newChip);
}

// Render all chips on the table
void ChipOnTable::RenderChips(SDL_Renderer* renderer) {
    for (size_t i = 0; i < tempChips.size(); ++i) {
        // Stack chips 10 pixels apart along the y-axis
        int offsetY = i * 7;  // Adjust this value to change the gap between stacked chips
        tempChips[i]->SetPosition(140, 490 - offsetY);  // y increases as more chips are added
        tempChips[i]->Render(renderer);
    }
}

void ChipOnTable::RenderWinningChips(SDL_Renderer* renderer) {
    for (size_t i = 0; i < tempChips.size(); ++i) {
        // Stack chips 10 pixels apart along the y-axis
        int offsetY = i * 7;  // Adjust this value to change the gap between stacked chips
        tempChips[i]->SetPosition(200, 490 - offsetY);  // y increases as more chips are added
        tempChips[i]->Render(renderer);
    }
    // Play the coin sound after the winning chips are rendered
    PlayCoinSound();
}
// Method to play the coin sound
void ChipOnTable::PlayCoinSound() {
    // Check if the sound has been loaded successfully
    static Mix_Chunk* coinSound = nullptr;
    if (!coinSound) {
        // Load the sound file
        coinSound = Mix_LoadWAV("assets/sounds/coinsound.mp3");
        if (coinSound == nullptr) {
            // Handle the error (e.g., log the error or show a message)
            SDL_Log("Failed to load coin sound: %s", Mix_GetError());
            return;
        }
    }

    // Play the sound (on channel 0)
    Mix_PlayChannel(-1, coinSound, 0);  // -1 means use the first available channel
}
// Remove all chips from the table
void ChipOnTable::RemoveChips() {
    for (auto& chip : tempChips) {
        delete chip;  // Ensure memory is freed
    }
    tempChips.clear();
}