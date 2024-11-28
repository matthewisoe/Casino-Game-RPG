#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
void showDisclaimerWindow(TTF_Font* font) {
    SDL_Window* disclaimerWindow = SDL_CreateWindow("Disclaimer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 300, SDL_WINDOW_SHOWN);
    if (!disclaimerWindow) {
        std::cerr << "Unable to create Disclaimer window: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Renderer* disclaimerRenderer = SDL_CreateRenderer(disclaimerWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!disclaimerRenderer) {
        SDL_DestroyWindow(disclaimerWindow);
        std::cerr << "Unable to create renderer for Disclaimer window: " << SDL_GetError() << std::endl;
        return;
    }
    // Load the icon image to be used as the window icon
    SDL_Surface* windowIcon = IMG_Load("C:/Users/GOH/CLionProjects/untitled/assets/PNG/button/warning.png"); // Specify path to icon image
    if (!windowIcon) {
        std::cerr << "Unable to load icon: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetWindowIcon(disclaimerWindow, windowIcon);  // Set the icon for the window
        SDL_FreeSurface(windowIcon);  // Free the surface after setting it as the icon
    }
    // Rest of the disclaimer window code unchanged...
    std::string disclaimerText = "Disclaimer:\nThis is a simulated casino game intended for entertainment purposes only.\nPlease gamble responsibly.";
    SDL_Color textColor = {255, 255, 255};  // White color for text on red background
    SDL_Surface* disclaimerSurface = TTF_RenderText_Blended_Wrapped(font, disclaimerText.c_str(), textColor, 450);
    SDL_Texture* disclaimerTexture = SDL_CreateTextureFromSurface(disclaimerRenderer, disclaimerSurface);
    SDL_Rect disclaimerRect = {(500 - disclaimerSurface->w) / 2, (300 - disclaimerSurface->h) / 2 - 25, disclaimerSurface->w, disclaimerSurface->h};
    SDL_FreeSurface(disclaimerSurface);
    SDL_Rect okButton = {200, 220, 100, 50};
    SDL_Surface* okTextSurface = TTF_RenderText_Solid(font, "OK", textColor);
    SDL_Texture* okTextTexture = SDL_CreateTextureFromSurface(disclaimerRenderer, okTextSurface);
    int okTextWidth, okTextHeight;
    SDL_QueryTexture(okTextTexture, nullptr, nullptr, &okTextWidth, &okTextHeight);
    SDL_Rect okTextRect = {okButton.x + (okButton.w - okTextWidth) / 2, okButton.y + (okButton.h - okTextHeight) / 2, okTextWidth, okTextHeight};
    SDL_FreeSurface(okTextSurface);
    bool disclaimerAccepted = false;
    SDL_Event event;
    while (!disclaimerAccepted) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyTexture(disclaimerTexture);
                SDL_DestroyTexture(okTextTexture);
                SDL_DestroyRenderer(disclaimerRenderer);
                SDL_DestroyWindow(disclaimerWindow);
                SDL_Quit();
                exit(0);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= okButton.x && mouseX <= okButton.x + okButton.w &&
                    mouseY >= okButton.y && mouseY <= okButton.y + okButton.h) {
                    disclaimerAccepted = true;
                }
            }
        }
        SDL_SetRenderDrawColor(disclaimerRenderer, 255, 0, 0, 255); // Red background
        SDL_RenderClear(disclaimerRenderer);
        SDL_RenderCopy(disclaimerRenderer, disclaimerTexture, nullptr, &disclaimerRect);
        SDL_SetRenderDrawColor(disclaimerRenderer, 0, 0, 0, 255); // Black button background
        SDL_RenderFillRect(disclaimerRenderer, &okButton);
        SDL_RenderCopy(disclaimerRenderer, okTextTexture, nullptr, &okTextRect);
        SDL_RenderPresent(disclaimerRenderer);
    }
    SDL_DestroyTexture(disclaimerTexture);
    SDL_DestroyTexture(okTextTexture);
    SDL_DestroyRenderer(disclaimerRenderer);
    SDL_DestroyWindow(disclaimerWindow);
}
void showAboutWindow(TTF_Font* font) {
    const int aboutWindowWidth = 600;
    const int aboutWindowHeight = 400;

    SDL_Window* aboutWindow = SDL_CreateWindow(
        "About", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        aboutWindowWidth, aboutWindowHeight, SDL_WINDOW_SHOWN
    );

    if (!aboutWindow) {
        std::cerr << "Unable to create About window: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Renderer* aboutRenderer = SDL_CreateRenderer(aboutWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!aboutRenderer) {
        SDL_DestroyWindow(aboutWindow);
        std::cerr << "Unable to create renderer for About window: " << SDL_GetError() << std::endl;
        return;
    }

    std::string aboutText =
        "Welcome to Casino Game!\n\n"
        "This is a fun and interactive simulated casino experience designed for entertainment purposes only. "
        "Immerse yourself in a vibrant virtual casino where you can explore various games, learn the rules, and enjoy the thrill of chance – all without any real-world risk.\n\n"
        "Key Features:\n"
        "- A beautiful and user-friendly interface designed to make your gameplay enjoyable.\n"
        "- Built-in background music and sound effects for an immersive experience.\n"
        "- Clear, beginner-friendly instructions for a seamless introduction to casino games.\n"
        "- Responsible gaming emphasis: This is a simulation and not a real gambling platform.\n\n"
        "Disclaimer:\n"
        "This game is intended for entertainment purposes only. It does not involve real money or any form of betting. "
        "Please play responsibly and keep it fun!";

    SDL_Color textColor = {0, 0, 0};  // Black color
    SDL_Surface* aboutSurface = TTF_RenderText_Blended_Wrapped(font, aboutText.c_str(), textColor, aboutWindowWidth - 50);
    SDL_Texture* aboutTexture = SDL_CreateTextureFromSurface(aboutRenderer, aboutSurface);
    int textHeight = aboutSurface->h;  // Full height of the rendered text
    SDL_FreeSurface(aboutSurface);

    int scrollOffset = 0;  // Vertical scroll offset
    const int scrollStep = 20;  // Amount to scroll per wheel or key press
    const int maxScrollOffset = textHeight - aboutWindowHeight + 50;  // Maximum scroll limit

    SDL_Rect textRect = {25, 25 - scrollOffset, aboutWindowWidth - 50, textHeight};

    bool quitAbout = false;
    SDL_Event event;

    while (!quitAbout) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                quitAbout = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    scrollOffset += scrollStep;
                    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    scrollOffset -= scrollStep;
                    if (scrollOffset < 0) scrollOffset = 0;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitAbout = true;
                }
            }
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {  // Scroll up
                    scrollOffset -= scrollStep;
                    if (scrollOffset < 0) scrollOffset = 0;
                } else if (event.wheel.y < 0) {  // Scroll down
                    scrollOffset += scrollStep;
                    if (scrollOffset > maxScrollOffset) scrollOffset = maxScrollOffset;
                }
            }
        }

        textRect.y = 25 - scrollOffset;  // Update text position based on scroll

        SDL_SetRenderDrawColor(aboutRenderer, 255, 255, 255, 255);  // White background
        SDL_RenderClear(aboutRenderer);

        SDL_RenderCopy(aboutRenderer, aboutTexture, nullptr, &textRect);

        SDL_RenderPresent(aboutRenderer);
    }

    SDL_DestroyTexture(aboutTexture);
    SDL_DestroyRenderer(aboutRenderer);
    SDL_DestroyWindow(aboutWindow);
}
// Function to draw a thicker border around a rectangle
void drawThickerBorder(SDL_Renderer* renderer, SDL_Rect rect, int thickness) {
    for (int i = 0; i < thickness; ++i) {
        SDL_Rect borderRect = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
        SDL_RenderDrawRect(renderer, &borderRect);
    }
}
void showGameSelectionScreen(SDL_Renderer* renderer, TTF_Font* font,
                             SDL_Texture* bgTexture, SDL_Texture* backButtonTexture,
                             SDL_Texture* muteTexture, SDL_Texture* unmuteTexture, bool& isMuted) {

    // Define paths for assets (with correct relative paths)
    const char* boyPaths[] = {
        "assets/PNG/Characters/character1/frame1.png",
        "assets/PNG/Characters/character2/frame2.png",
        "assets/PNG/Characters/character3/frame3.png"
    };

    const char* girlPaths[] = {
        "assets/PNG/Characters/character4/frame1.png",
        "assets/PNG/Characters/character5/frame1.png",
        "assets/PNG/Characters/character6/frame1.png"
    };

    SDL_Surface* boySurfaces[3], * girlSurfaces[3];
    SDL_Texture* boyTextures[3], * girlTextures[3];

    // Load boy character images
    for (int i = 0; i < 3; ++i) {
        boySurfaces[i] = IMG_Load(boyPaths[i]);
        if (!boySurfaces[i]) {
            std::cerr << "Failed to load boy character image: " << IMG_GetError() << std::endl;
            return;
        }
        boyTextures[i] = SDL_CreateTextureFromSurface(renderer, boySurfaces[i]);
        SDL_FreeSurface(boySurfaces[i]);
        SDL_SetTextureBlendMode(boyTextures[i], SDL_BLENDMODE_BLEND); // Set blend mode for transparency
    }

    // Load girl character images
    for (int i = 0; i < 3; ++i) {
        girlSurfaces[i] = IMG_Load(girlPaths[i]);
        if (!girlSurfaces[i]) {
            std::cerr << "Failed to load girl character image: " << IMG_GetError() << std::endl;
            return;
        }
        girlTextures[i] = SDL_CreateTextureFromSurface(renderer, girlSurfaces[i]);
        SDL_FreeSurface(girlSurfaces[i]);
        SDL_SetTextureBlendMode(girlTextures[i], SDL_BLENDMODE_BLEND); // Set blend mode for transparency
    }

    // Define rectangles for character positions and buttons
    SDL_Rect charRects[3] = {
        {100, 200, 150, 200},
        {325, 200, 150, 200},
        {550, 200, 150, 200}
    };

    SDL_Rect backButtonRect = {20, WINDOW_HEIGHT - 70, 50, 50};
    SDL_Rect muteButtonRect = {WINDOW_WIDTH - 60, WINDOW_HEIGHT - 60, 50, 50};
    SDL_Rect proceedButtonRect = {WINDOW_WIDTH / 2 - 75, WINDOW_HEIGHT - 100, 150, 50};  // Define proceed button rect

    // Load boy/girl selection buttons
    SDL_Surface* boyButtonSurface = IMG_Load("assets/PNG/Button/boy_button.png");
    SDL_Surface* girlButtonSurface = IMG_Load("assets/PNG/Button/girl_button.png");
    if (!boyButtonSurface || !girlButtonSurface) {
        std::cerr << "Failed to load button images: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* boyButtonTexture = SDL_CreateTextureFromSurface(renderer, boyButtonSurface);
    SDL_Texture* girlButtonTexture = SDL_CreateTextureFromSurface(renderer, girlButtonSurface);
    SDL_FreeSurface(boyButtonSurface);
    SDL_FreeSurface(girlButtonSurface);

    SDL_Rect boyButtonRect = {WINDOW_WIDTH / 2 - 200, 50, 100, 50};
    SDL_Rect girlButtonRect = {WINDOW_WIDTH / 2 + 100, 50, 100, 50};

    // Create the text "Choose your own character"
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Choose your own character", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = {(WINDOW_WIDTH - titleSurface->w) / 2, 135, titleSurface->w, titleSurface->h};
    SDL_FreeSurface(titleSurface);

    // Character names for boy and girl characters
    const char* boyNames[] = {"Adam", "Bernard", "Charlie"};
    const char* girlNames[] = {"Alice", "Beatrice", "Catherine"};

    SDL_Texture* boyNameTextures[3], * girlNameTextures[3];
    SDL_Rect nameRects[3];

    // Create text for character names
    for (int i = 0; i < 3; ++i) {
        SDL_Surface* boyNameSurface = TTF_RenderText_Solid(font, boyNames[i], textColor);
        boyNameTextures[i] = SDL_CreateTextureFromSurface(renderer, boyNameSurface);
        nameRects[i] = {charRects[i].x + (charRects[i].w - boyNameSurface->w) / 2,
                        charRects[i].y + charRects[i].h + 5,
                        boyNameSurface->w, boyNameSurface->h};
        SDL_FreeSurface(boyNameSurface);

        SDL_Surface* girlNameSurface = TTF_RenderText_Solid(font, girlNames[i], textColor);
        girlNameTextures[i] = SDL_CreateTextureFromSurface(renderer, girlNameSurface);
        SDL_FreeSurface(girlNameSurface);
    }

    bool quitGameSelection = false;
    SDL_Event event;
    int selectedCharacter = -1; // -1 means no character selected
    bool isBoySelected = true;   // Flag to check if Boy or Girl set is selected
    int selectedBoyIndex = -1, selectedGirlIndex = -1; // Tracks selected boy or girl character index

    while (!quitGameSelection) {
        SDL_RenderClear(renderer);

        // Set background color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); // Clears the window to white

        // Render the title text and buttons
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        SDL_RenderCopy(renderer, boyButtonTexture, NULL, &boyButtonRect);
        SDL_RenderCopy(renderer, girlButtonTexture, NULL, &girlButtonRect);

        // Render selected character images and names
        if (isBoySelected) {
            for (int i = 0; i < 3; ++i) {
                SDL_RenderCopy(renderer, boyTextures[i], NULL, &charRects[i]);

                // Draw a thicker red border around the selected character
                if (i == selectedBoyIndex) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
                    drawThickerBorder(renderer, charRects[i], 5); // 5 is the border thickness

                    // Render the name together with the red border
                    SDL_RenderCopy(renderer, boyNameTextures[i], NULL, &nameRects[i]);
                }
            }
        } else {
            for (int i = 0; i < 3; ++i) {
                SDL_RenderCopy(renderer, girlTextures[i], NULL, &charRects[i]);

                // Draw a thicker red border around the selected character
                if (i == selectedGirlIndex) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
                    drawThickerBorder(renderer, charRects[i], 5); // 5 is the border thickness

                    // Render the name together with the red border
                    SDL_RenderCopy(renderer, girlNameTextures[i], NULL, &nameRects[i]);
                }
            }
        }

        // Only render the proceed button if a character is selected
        if (selectedBoyIndex != -1 || selectedGirlIndex != -1) {
            // Draw a green button
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for the button
            SDL_RenderFillRect(renderer, &proceedButtonRect);  // Draw the button rectangle

            // Create "PROCEED" text with black color
            SDL_Surface* proceedTextSurface = TTF_RenderText_Solid(font, "PROCEED", {0, 0, 0}); // Black color
            if (proceedTextSurface != nullptr) {
                SDL_Texture* proceedTextTexture = SDL_CreateTextureFromSurface(renderer, proceedTextSurface);
                if (proceedTextTexture != nullptr) {
                    // Position the text in the center of the button
                    SDL_Rect textRect = {
                        proceedButtonRect.x + (proceedButtonRect.w - proceedTextSurface->w) / 2,  // Center the text horizontally
                        proceedButtonRect.y + (proceedButtonRect.h - proceedTextSurface->h) / 2,  // Center the text vertically
                        proceedTextSurface->w,  // Width of the text
                        proceedTextSurface->h   // Height of the text
                    };

                    // Render the "PROCEED" text
                    SDL_RenderCopy(renderer, proceedTextTexture, NULL, &textRect);

                    // Clean up the text texture after rendering
                    SDL_DestroyTexture(proceedTextTexture);
                }
                SDL_FreeSurface(proceedTextSurface); // Free the surface after creating the texture
            }
        }

        // Render mute/unmute button
        SDL_Texture* soundTexture = isMuted ? muteTexture : unmuteTexture;
        SDL_RenderCopy(renderer, soundTexture, NULL, &muteButtonRect);

        // Render back button
        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitGameSelection = true;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if the boy button is clicked
                if (x >= boyButtonRect.x && x <= boyButtonRect.x + boyButtonRect.w &&
                    y >= boyButtonRect.y && y <= boyButtonRect.y + boyButtonRect.h) {
                    isBoySelected = true; // Select boy character set
                }

                // Check if the girl button is clicked
                if (x >= girlButtonRect.x && x <= girlButtonRect.x + girlButtonRect.w &&
                    y >= girlButtonRect.y && y <= girlButtonRect.y + girlButtonRect.h) {
                    isBoySelected = false; // Select girl character set
                }

                // Check if mute/unmute button is clicked
                if (x >= muteButtonRect.x && x <= muteButtonRect.x + muteButtonRect.w &&
                    y >= muteButtonRect.y && y <= muteButtonRect.y + muteButtonRect.h) {
                    isMuted = !isMuted;
                }

                // Check if the back button is clicked
                if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                    y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                    quitGameSelection = true;
                }

                // Check if the proceed button is clicked (only when a character is selected)
                if (x >= proceedButtonRect.x && x <= proceedButtonRect.x + proceedButtonRect.w &&
                    y >= proceedButtonRect.y && y <= proceedButtonRect.y + proceedButtonRect.h) {
                    if (selectedBoyIndex != -1 || selectedGirlIndex != -1) {
                        // Proceed to the next screen or action here
                        // Example: Proceed with the character selection and print the selected character's name
                        if (selectedBoyIndex != -1) {
                            std::cout << "Proceeding with " << boyNames[selectedBoyIndex] << " character!" << std::endl;
                        } else if (selectedGirlIndex != -1) {
                            std::cout << "Proceeding with " << girlNames[selectedGirlIndex] << " character!" << std::endl;
                        }
                        quitGameSelection = true;  // Exit the game selection screen
                    }
                    }


                // Check if a character is clicked
                for (int i = 0; i < 3; ++i) {
                    if (x >= charRects[i].x && x <= charRects[i].x + charRects[i].w &&
                        y >= charRects[i].y && y <= charRects[i].y + charRects[i].h) {
                        if (isBoySelected) {
                            selectedBoyIndex = i;
                            selectedGirlIndex = -1; // Deselect girl characters
                        } else {
                            selectedGirlIndex = i;
                            selectedBoyIndex = -1; // Deselect boy characters
                        }
                    }
                }
            }
        }
    }

    // Cleanup textures
    for (int i = 0; i < 3; ++i) {
        SDL_DestroyTexture(boyTextures[i]);
        SDL_DestroyTexture(girlTextures[i]);
        SDL_DestroyTexture(boyNameTextures[i]);
        SDL_DestroyTexture(girlNameTextures[i]);
    }

    SDL_DestroyTexture(boyButtonTexture);
    SDL_DestroyTexture(girlButtonTexture);
    SDL_DestroyTexture(titleTexture);
}

void fadeUpText(SDL_Renderer* renderer, TTF_Font* font, const std::string& playerName) {
    // Prepare the greeting message
    std::string message = "Hi, " + playerName + "!";
    SDL_Color textColor = {255, 255, 255, 255};  // White color

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect = {(WINDOW_WIDTH - textWidth) / 2, WINDOW_HEIGHT / 2, textWidth, textHeight};

    // Animation parameters
    int startY = textRect.y;
    int targetY = textRect.y - 100;  // Move up by 100 pixels
    int alpha = 255;                // Start with full opacity
    int fadeSpeed = 5;              // Pixels per frame for vertical movement
    int fadeStep = 5;               // Decrease opacity per frame

    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

    while (alpha > 0) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyTexture(textTexture);
                SDL_Quit();
                exit(0);
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
        SDL_RenderClear(renderer);

        // Render the fading text
        textRect.y = startY - fadeSpeed * (255 - alpha) / fadeStep;  // Adjust position based on alpha
        if (textRect.y < targetY) textRect.y = targetY;
        SDL_SetTextureAlphaMod(textTexture, alpha);  // Set transparency
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Adjust alpha and delay for fade effect
        alpha -= fadeStep;
        SDL_Delay(50);  // ~60 FPS
    }

    SDL_DestroyTexture(textTexture);
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    Mix_Music* bgMusic = Mix_LoadMUS("assets/sounds/bgm.mp3");
    Mix_Chunk* typingSound = Mix_LoadWAV("assets/sounds/click.wav");
    if (!bgMusic || !typingSound) {
        std::cerr << "Failed to load audio files! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    Mix_VolumeChunk(typingSound, 128);
    Mix_PlayMusic(bgMusic, -1);
    TTF_Font* font = TTF_OpenFont("C:/Users/GOH/CLionProjects/untitled/assets/font/OpenSans-Italic-VariableFont_wdth,wght.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        Mix_FreeChunk(typingSound);
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    // Show disclaimer window first
    showDisclaimerWindow(font);
    SDL_Window* window = SDL_CreateWindow("Casino Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer || !window) {
        std::cerr << "Failed to create window or renderer! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        Mix_FreeChunk(typingSound);
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Surface* bgSurface = IMG_Load("assets/PNG/Background/bg3.png");
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    SDL_Surface* logoSurface = IMG_Load("assets/PNG/Logo/logo.png");
    SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);
    SDL_Surface* muteSurface = IMG_Load("assets/PNG/Button/mute.png");
    SDL_Surface* unmuteSurface = IMG_Load("assets/PNG/Button/unmute.png");
    SDL_Texture* muteTexture = SDL_CreateTextureFromSurface(renderer, muteSurface);
    SDL_Texture* unmuteTexture = SDL_CreateTextureFromSurface(renderer, unmuteSurface);
    SDL_FreeSurface(muteSurface);
    SDL_FreeSurface(unmuteSurface);
    SDL_StartTextInput();
    std::string playerName = "";
    SDL_Rect inputBox = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300, 50};
    SDL_Rect playButton = {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 60, 100, 50};
    SDL_Rect settingsButton = {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 120, 100, 50};
    SDL_Rect aboutButton = {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 180, 100, 50};
    SDL_Rect muteButtonRect = {WINDOW_WIDTH - 60, WINDOW_HEIGHT - 60, 50, 50};
    bool isMuted = false;
    SDL_Color textColor = {0, 0, 0};
    SDL_Color placeholderColor = {192, 192, 192};
    SDL_Color whiteColor = {255, 255, 255};
    SDL_Surface* playTextSurface = TTF_RenderText_Solid(font, "Play", whiteColor);
    SDL_Surface* settingsTextSurface = TTF_RenderText_Solid(font, "Settings", whiteColor);
    SDL_Surface* aboutTextSurface = TTF_RenderText_Solid(font, "About", whiteColor);
    SDL_Texture* playTextTexture = SDL_CreateTextureFromSurface(renderer, playTextSurface);
    SDL_Texture* settingsTextTexture = SDL_CreateTextureFromSurface(renderer, settingsTextSurface);
    SDL_Texture* aboutTextTexture = SDL_CreateTextureFromSurface(renderer, aboutTextSurface);
    SDL_FreeSurface(playTextSurface);
    SDL_FreeSurface(settingsTextSurface);
    SDL_FreeSurface(aboutTextSurface);
    bool quit = false;
    SDL_Event event;
    bool gameStarted = false;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_TEXTINPUT && !gameStarted) {
                playerName += event.text.text;
                Mix_PlayChannel(-1, typingSound, 0);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0 && !gameStarted) {
                playerName.pop_back();
                Mix_PlayChannel(-1, typingSound, 0);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
    mouseY >= playButton.y && mouseY <= playButton.y + playButton.h) {
                    gameStarted = true;
                    std::cout << "Game Started! Player Name: " << playerName << std::endl;

                    // Load the back button texture
                    SDL_Surface* backButtonSurface = IMG_Load("assets/PNG/Button/back.png");
                    SDL_Texture* backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
                    SDL_FreeSurface(backButtonSurface);

                    // Fade-up text effect
                    fadeUpText(renderer, font, playerName);

                    // Show game selection screen
                    showGameSelectionScreen(renderer, font, bgTexture, backButtonTexture, muteTexture, unmuteTexture, isMuted);
    }



                if (mouseX >= settingsButton.x && mouseX <= settingsButton.x + settingsButton.w &&
                    mouseY >= settingsButton.y && mouseY <= settingsButton.y + settingsButton.h) {
                    std::cout << "Settings Button Clicked!" << std::endl;
                }
                if (mouseX >= aboutButton.x && mouseX <= aboutButton.x + aboutButton.w &&
                    mouseY >= aboutButton.y && mouseY <= aboutButton.y + aboutButton.h) {
                    showAboutWindow(font);
                }
                if (mouseX >= muteButtonRect.x && mouseX <= muteButtonRect.x + muteButtonRect.w &&
                    mouseY >= muteButtonRect.y && mouseY <= muteButtonRect.y + muteButtonRect.h) {
                    isMuted = !isMuted;
                    if (isMuted) {
                        Mix_PauseMusic();
                    } else {
                        Mix_ResumeMusic();
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        SDL_Rect logoRect = {(WINDOW_WIDTH - 200) / 2, 50, 200, 200};
        SDL_RenderCopy(renderer, logoTexture, nullptr, &logoRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &inputBox);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &inputBox);
        SDL_Surface* textSurface;
        if (playerName.empty()) {
            textSurface = TTF_RenderText_Solid(font, "Enter your name", placeholderColor);
        } else {
            textSurface = TTF_RenderText_Solid(font, playerName.c_str(), textColor);
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect textRect = {inputBox.x + (inputBox.w - textWidth) / 2, inputBox.y + (inputBox.h - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playButton);
        SDL_RenderFillRect(renderer, &settingsButton);
        SDL_RenderFillRect(renderer, &aboutButton);
        int playTextWidth, playTextHeight;
        SDL_QueryTexture(playTextTexture, nullptr, nullptr, &playTextWidth, &playTextHeight);
        SDL_Rect playTextRect = {playButton.x + (playButton.w - playTextWidth) / 2, playButton.y + (playButton.h - playTextHeight) / 2, playTextWidth, playTextHeight};
        SDL_RenderCopy(renderer, playTextTexture, nullptr, &playTextRect);
        int settingsTextWidth, settingsTextHeight;
        SDL_QueryTexture(settingsTextTexture, nullptr, nullptr, &settingsTextWidth, &settingsTextHeight);
        SDL_Rect settingsTextRect = {settingsButton.x + (settingsButton.w - settingsTextWidth) / 2, settingsButton.y + (settingsButton.h - settingsTextHeight) / 2, settingsTextWidth, settingsTextHeight};
        SDL_RenderCopy(renderer, settingsTextTexture, nullptr, &settingsTextRect);
        int aboutTextWidth, aboutTextHeight;
        SDL_QueryTexture(aboutTextTexture, nullptr, nullptr, &aboutTextWidth, &aboutTextHeight);
        SDL_Rect aboutTextRect = {aboutButton.x + (aboutButton.w - aboutTextWidth) / 2, aboutButton.y + (aboutButton.h - aboutTextHeight) / 2, aboutTextWidth, aboutTextHeight};
        SDL_RenderCopy(renderer, aboutTextTexture, nullptr, &aboutTextRect);
        SDL_RenderCopy(renderer, isMuted ? muteTexture : unmuteTexture, nullptr, &muteButtonRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(playTextTexture);
    SDL_DestroyTexture(settingsTextTexture);
    SDL_DestroyTexture(aboutTextTexture);
    SDL_DestroyTexture(muteTexture);
    SDL_DestroyTexture(unmuteTexture);
    SDL_DestroyTexture(logoTexture);
    SDL_DestroyTexture(bgTexture);
    Mix_FreeChunk(typingSound);
    Mix_FreeMusic(bgMusic);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}