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
    SDL_Window* aboutWindow = SDL_CreateWindow("About", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
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
    bool quitAbout = false;
    SDL_Event event;
    std::string gameIntro = "Welcome to the Casino Game!";
    std::string gameRules = "Rules:\n1. Rule 1: Description\n2. Rule 2: Description\n3. Rule 3: Description\n...";
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface* introSurface = TTF_RenderText_Blended_Wrapped(font, gameIntro.c_str(), textColor, 500);
    SDL_Texture* introTexture = SDL_CreateTextureFromSurface(aboutRenderer, introSurface);
    SDL_Rect introRect = {50, 50, introSurface->w, introSurface->h};
    SDL_FreeSurface(introSurface);
    SDL_Surface* rulesSurface = TTF_RenderText_Blended_Wrapped(font, gameRules.c_str(), textColor, 500);
    SDL_Texture* rulesTexture = SDL_CreateTextureFromSurface(aboutRenderer, rulesSurface);
    SDL_Rect rulesRect = {50, 150, rulesSurface->w, rulesSurface->h};
    SDL_FreeSurface(rulesSurface);
    while (!quitAbout) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                quitAbout = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quitAbout = true;
            }
        }
        SDL_SetRenderDrawColor(aboutRenderer, 255, 255, 255, 255);
        SDL_RenderClear(aboutRenderer);
        SDL_RenderCopy(aboutRenderer, introTexture, nullptr, &introRect);
        SDL_RenderCopy(aboutRenderer, rulesTexture, nullptr, &rulesRect);
        SDL_RenderPresent(aboutRenderer);
    }
    SDL_DestroyTexture(introTexture);
    SDL_DestroyTexture(rulesTexture);
    SDL_DestroyRenderer(aboutRenderer);
    SDL_DestroyWindow(aboutWindow);
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
                if (!gameStarted && playerName.length() > 0 && mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
                    mouseY >= playButton.y && mouseY <= playButton.y + playButton.h) {
                    gameStarted = true;
                    std::cout << "Game Started! Player Name: " << playerName << std::endl;
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