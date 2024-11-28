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
    // Load character images
    SDL_Surface* char1Surface = IMG_Load("assets/PNG/Characters/character1/frame1.png");
    SDL_Surface* char2Surface = IMG_Load("assets/PNG/Characters/character2/frame2.png");
    SDL_Surface* char3Surface = IMG_Load("assets/PNG/Characters/character3/frame3.png");

    if (!char1Surface || !char2Surface || !char3Surface) {
        std::cerr << "Failed to load character images: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
    SDL_Texture* char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
    SDL_Texture* char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);

    SDL_FreeSurface(char1Surface);
    SDL_FreeSurface(char2Surface);
    SDL_FreeSurface(char3Surface);

    // Enable blending for transparency
    SDL_SetTextureBlendMode(char1Texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(char2Texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(char3Texture, SDL_BLENDMODE_BLEND);

    // Rectangles for character positions
    SDL_Rect char1Rect = {100, 200, 150, 200};
    SDL_Rect char2Rect = {325, 200, 150, 200};
    SDL_Rect char3Rect = {550, 200, 150, 200};

    SDL_Rect backButtonRect = {20, WINDOW_HEIGHT - 70, 50, 50};
    SDL_Rect muteButtonRect = {WINDOW_WIDTH - 60, WINDOW_HEIGHT - 60, 50, 50};
    SDL_Rect proceedButtonRect = {WINDOW_WIDTH / 2 - 75, WINDOW_HEIGHT - 100, 150, 50}; // Position for Proceed button

    // Create the text "Choose your own character"
    SDL_Color textColor = {0, 0, 0}; // Black color
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Choose your own character", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    int titleWidth = titleSurface->w;
    int titleHeight = titleSurface->h;
    SDL_Rect titleRect = {(WINDOW_WIDTH - titleWidth) / 2, 100, titleWidth, titleHeight}; // Centered at the top
    SDL_FreeSurface(titleSurface);

    // Character names
    const char* char1Name = "Adam";
    const char* char2Name = "Bernard";
    const char* char3Name = "Charlie";

    bool quitGameSelection = false;
    SDL_Event event;
    int selectedCharacter = -1; // -1 means no character selected

    // Create the text for character names
    SDL_Surface* nameSurface1 = TTF_RenderText_Solid(font, char1Name, textColor);
    SDL_Texture* nameTexture1 = SDL_CreateTextureFromSurface(renderer, nameSurface1);
    int nameWidth1 = nameSurface1->w;
    int nameHeight1 = nameSurface1->h;
    SDL_Rect nameRect1 = {char1Rect.x + (char1Rect.w - nameWidth1) / 2, char1Rect.y + char1Rect.h + 5, nameWidth1, nameHeight1};
    SDL_FreeSurface(nameSurface1);

    SDL_Surface* nameSurface2 = TTF_RenderText_Solid(font, char2Name, textColor);
    SDL_Texture* nameTexture2 = SDL_CreateTextureFromSurface(renderer, nameSurface2);
    int nameWidth2 = nameSurface2->w;
    int nameHeight2 = nameSurface2->h;
    SDL_Rect nameRect2 = {char2Rect.x + (char2Rect.w - nameWidth2) / 2, char2Rect.y + char2Rect.h + 5, nameWidth2, nameHeight2};
    SDL_FreeSurface(nameSurface2);

    SDL_Surface* nameSurface3 = TTF_RenderText_Solid(font, char3Name, textColor);
    SDL_Texture* nameTexture3 = SDL_CreateTextureFromSurface(renderer, nameSurface3);
    int nameWidth3 = nameSurface3->w;
    int nameHeight3 = nameSurface3->h;
    SDL_Rect nameRect3 = {char3Rect.x + (char3Rect.w - nameWidth3) / 2, char3Rect.y + char3Rect.h + 5, nameWidth3, nameHeight3};
    SDL_FreeSurface(nameSurface3);

    while (!quitGameSelection) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitGameSelection = true;
                SDL_Quit();
                exit(0);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Check if back button is clicked
                if (mouseX >= backButtonRect.x && mouseX <= backButtonRect.x + backButtonRect.w &&
                    mouseY >= backButtonRect.y && mouseY <= backButtonRect.y + backButtonRect.h) {
                    quitGameSelection = true;  // Back to the previous screen
                }

                // Check if mute button is clicked
                if (mouseX >= muteButtonRect.x && mouseX <= muteButtonRect.x + muteButtonRect.w &&
                    mouseY >= muteButtonRect.y && mouseY <= muteButtonRect.y + muteButtonRect.h) {
                    isMuted = !isMuted;
                    if (isMuted) Mix_PauseMusic();
                    else Mix_ResumeMusic();
                }

                // Check if proceed button is clicked
                if (mouseX >= proceedButtonRect.x && mouseX <= proceedButtonRect.x + proceedButtonRect.w &&
                    mouseY >= proceedButtonRect.y && mouseY <= proceedButtonRect.y + proceedButtonRect.h &&
                    selectedCharacter != -1) {
                    // Proceed to the next step (For now, just quit the selection)
                    quitGameSelection = true;
                    std::cout << "Character " << (selectedCharacter == 1 ? "Adam" : (selectedCharacter == 2 ? "Bernard" : "Charlie")) << " selected!" << std::endl;
                }

                // Check if a character is clicked
                if (mouseX >= char1Rect.x && mouseX <= char1Rect.x + char1Rect.w &&
                    mouseY >= char1Rect.y && mouseY <= char1Rect.y + char1Rect.h) {
                    selectedCharacter = 1;  // Character 1 selected
                }
                else if (mouseX >= char2Rect.x && mouseX <= char2Rect.x + char2Rect.w &&
                         mouseY >= char2Rect.y && mouseY <= char2Rect.y + char2Rect.h) {
                    selectedCharacter = 2;  // Character 2 selected
                }
                else if (mouseX >= char3Rect.x && mouseX <= char3Rect.x + char3Rect.w &&
                         mouseY >= char3Rect.y && mouseY <= char3Rect.y + char3Rect.h) {
                    selectedCharacter = 3;  // Character 3 selected
                }
            }
        }

        SDL_RenderClear(renderer);

        // Render a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render the title
        SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);

        // Render characters with transparency
        SDL_RenderCopy(renderer, char1Texture, nullptr, &char1Rect);
        SDL_RenderCopy(renderer, char2Texture, nullptr, &char2Rect);
        SDL_RenderCopy(renderer, char3Texture, nullptr, &char3Rect);

        // Highlight the selected character (with a thicker red border)
        if (selectedCharacter == 1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red border
            drawThickerBorder(renderer, char1Rect, 4);  // Thicker border (4px)
            SDL_RenderCopy(renderer, nameTexture1, nullptr, &nameRect1);  // Show name
        }
        if (selectedCharacter == 2) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red border
            drawThickerBorder(renderer, char2Rect, 4);  // Thicker border (4px)
            SDL_RenderCopy(renderer, nameTexture2, nullptr, &nameRect2);  // Show name
        }
        if (selectedCharacter == 3) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red border
            drawThickerBorder(renderer, char3Rect, 4);  // Thicker border (4px)
            SDL_RenderCopy(renderer, nameTexture3, nullptr, &nameRect3);  // Show name
        }

        // Render the proceed button if a character is selected
        if (selectedCharacter != -1) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for proceed button
            SDL_RenderFillRect(renderer, &proceedButtonRect);
            // Render text on Proceed button
            SDL_Surface* proceedSurface = TTF_RenderText_Solid(font, "Proceed", textColor);
            SDL_Texture* proceedTexture = SDL_CreateTextureFromSurface(renderer, proceedSurface);
            SDL_FreeSurface(proceedSurface);
            SDL_RenderCopy(renderer, proceedTexture, nullptr, &proceedButtonRect);
            SDL_DestroyTexture(proceedTexture);
        }

        // Render back and mute/unmute buttons
        SDL_RenderCopy(renderer, backButtonTexture, nullptr, &backButtonRect);
        SDL_RenderCopy(renderer, isMuted ? muteTexture : unmuteTexture, nullptr, &muteButtonRect);

        SDL_RenderPresent(renderer);
    }

    // Clean up textures
    SDL_DestroyTexture(char1Texture);
    SDL_DestroyTexture(char2Texture);
    SDL_DestroyTexture(char3Texture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(nameTexture1);
    SDL_DestroyTexture(nameTexture2);
    SDL_DestroyTexture(nameTexture3);
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