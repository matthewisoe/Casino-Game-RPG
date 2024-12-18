#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <unordered_map> // For std::unordered_map
#include <vector>        // For std::vector

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

std::string currentLanguage = "en"; // Global variable to track current language
// Localized Terms & Privacy content
std::unordered_map<std::string, std::string> termsAndPrivacyContent = {
    {"en",
     "Terms & Privacy\n\n"
     "1. Introduction\n"
     "Welcome to Casino Game! By using this application, you agree to comply with and "
     "be bound by the following Terms of Service and Privacy Policy. Please read them carefully before using our services.\n\n"
     "2. Terms of Service\n"
     "2.1 Use of Application\n"
     "This application is intended for entertainment purposes only.\n\n"
     "2.2 User Responsibilities\n"
     "You are responsible for maintaining the security of your device and account information.\n\n"
     "2.3 Intellectual Property\n"
     "All content, features, and functionalities of the application are the intellectual property of Casino Game.\n\n"
     "2.4 Termination\n"
     "We reserve the right to suspend or terminate your access at any time.\n\n"
     "3. Privacy Policy\n"
     "3.1 Information We Collect\n"
     "We may collect personal information such as your name and usage data to improve the app functionality.\n\n"
     "3.2 Sharing of Information\n"
     "We do not sell your personal information to third parties.\n\n"
     "4. Data Security\n"
     "We implement measures to protect your data but cannot guarantee complete security.\n\n"
     "5. Contact Us\n"
     "For questions, contact 123040017@link.cuhk..edu.cn\n\n"},
    {"zh",
     "条款与隐私\n\n"
     "1. 介绍\n"
     "欢迎使用Casino Game！使用此应用程序即表示您同意遵守以下服务条款和隐私政策。请在使用我们的服务之前仔细阅读。\n\n"
     "2. 服务条款\n"
     "2.1 应用程序的使用\n"
     "此应用程序仅供娱乐用途。\n\n"
     "2.2 用户责任\n"
     "您有责任维护设备和帐户信息的安全性。\n\n"
     "2.3 知识产权\n"
     "应用程序的所有内容、功能和功能均为Casino Game的知识产权。\n\n"
     "2.4 终止\n"
     "我们保留随时暂停或终止您访问的权利。\n\n"
     "3. 隐私政策\n"
     "3.1 我们收集的信息\n"
     "我们可能会收集个人信息，例如您的姓名和使用数据，以改进应用功能。\n\n"
     "3.2 信息共享\n"
     "我们不会将您的个人信息出售给第三方。\n\n"
     "4. 数据安全\n"
     "我们实施了措施来保护您的数据，但无法保证完全的安全性。\n\n"
     "5. 联系我们\n"
     "如有问题，请联系 123040017@link.cuhk..edu.cn\n\n"}
};

// Function to fetch localized Terms & Privacy content
std::string getLocalizedTermsAndPrivacy() {
    return termsAndPrivacyContent[currentLanguage];
}
std::string getLocalizedText(const std::string& key) {
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> languageMap = {
        {"en", {{"settings", "Settings"}, {"language", "Language"}, {"help", "Help"}, {"credits", "Credits"}, {"terms", "Terms & Privacy"}, {"back", "Back"}}},
        {"zh", {{"settings", "设置"}, {"language", "语言"}, {"help", "帮助"}, {"credits", "鸣谢"}, {"terms", "条款与隐私"}, {"back", "返回"}}},
    };

    return languageMap[currentLanguage][key];
}


// Function to get text based on language
std::string getText(const std::string& key) {
    static std::map<std::string, std::map<std::string, std::string>> translations = {
        {"en", {{"Language", "Language"}, {"Help", "Help"}, {"Credits", "Credits"}, {"Terms & Privacy", "Terms & Privacy"}, {"Back", "Back"}}},
        {"zh", {{"Language", "语言"}, {"Help", "帮助"}, {"Credits", "制作团队"}, {"Terms & Privacy", "条款与隐私"}, {"Back", "返回"}}}
    };
    return translations[currentLanguage][key];
}
void showTermsAndPrivacyWindow(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color blackColor = {0, 0, 0};
    bool quitTermsWindow = false;
    SDL_Event event;

    // Fetch localized Terms & Privacy content
    const std::string termsText = getLocalizedTermsAndPrivacy();

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, termsText.c_str(), blackColor, WINDOW_WIDTH - 40);
    if (!textSurface) {
        std::cerr << "Failed to create text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    int scrollOffset = 0;
    const int scrollSpeed = 40;

    while (!quitTermsWindow) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitTermsWindow = true;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitTermsWindow = true;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    scrollOffset = std::max(scrollOffset - scrollSpeed, 0);
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    scrollOffset = std::min(scrollOffset + scrollSpeed, textHeight - WINDOW_HEIGHT + 80);
                }
            }

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scrollOffset = std::max(scrollOffset - scrollSpeed, 0);
                } else if (event.wheel.y < 0) {
                    scrollOffset = std::min(scrollOffset + scrollSpeed, textHeight - WINDOW_HEIGHT + 80);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect textViewport = {0, scrollOffset, textWidth, WINDOW_HEIGHT - 80};
        SDL_Rect renderRect = {20, 20, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 80};
        SDL_RenderCopy(renderer, textTexture, &textViewport, &renderRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textTexture);
}
void showCreditsWindow(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color blackColor = {0, 0, 0};
    bool quitCreditsWindow = false;
    SDL_Event event;

    // Credits text
    const std::string creditsText =
        "Credits\n\n"
        "1. Development Team\n"
        "Matthew Irving Soenarjo\n"
        "Darren Clay Liem\n"
        "Ramses Piramus\n"
        "Goh Ean Jie\n"
        "Chi Cong Yu\n\n"
        "2. Libraries and Frameworks\n"
        "SDL2: Simple DirectMedia Layer for graphics and input handling (https://www.libsdl.org)\n"
        "SDL_ttf: For font rendering (https://www.libsdl.org/projects/SDL_ttf/)\n"
        "SDL_mixer: For audio support (https://www.libsdl.org/projects/SDL_mixer/)\n"
        "SDL_image: For loading images (https://www.libsdl.org/projects/SDL_image/)\n\n"
        "3. Fonts\n"
        "Noto Sans SC: Simplified Chinese font by Google (https://www.google.com/get/noto)\n\n"
        "4. Assets\n"
        "Background Images: [Source or Attribution, e.g., freepik.com, etc.]\n"
        "Sound Effects: [Source or Attribution, e.g., freesound.org, etc.]\n"
        "Icons: [Source or Attribution, e.g., flaticon.com, etc.]\n\n"
        "5. Special Thanks\n"
        "To all contributors who helped during testing and feedback.\n"
        "To the open-source community for their invaluable tools and resources.\n\n"
        "6. Contact Us\n"
        "For inquiries or issues, please contact:\n"
        "Email: 123040017@link.cuhk.edu.cn\n";

    // Create texture for credits text
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, creditsText.c_str(), blackColor, WINDOW_WIDTH - 40);
    if (!textSurface) {
        std::cerr << "Failed to create text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    int scrollOffset = 0;
    const int scrollSpeed = 40; // Pixels per scroll action

    while (!quitCreditsWindow) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitCreditsWindow = true;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitCreditsWindow = true; // Exit on Escape key
                } else if (event.key.keysym.sym == SDLK_UP) {
                    scrollOffset = std::max(scrollOffset - scrollSpeed, 0); // Scroll up
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    scrollOffset = std::min(scrollOffset + scrollSpeed, textHeight - WINDOW_HEIGHT + 80); // Scroll down
                }
            }

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scrollOffset = std::max(scrollOffset - scrollSpeed, 0); // Scroll up
                } else if (event.wheel.y < 0) {
                    scrollOffset = std::min(scrollOffset + scrollSpeed, textHeight - WINDOW_HEIGHT + 80); // Scroll down
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render credits text
        SDL_Rect textViewport = {0, scrollOffset, textWidth, WINDOW_HEIGHT - 80};
        SDL_Rect renderRect = {20, 20, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 80};
        SDL_RenderCopy(renderer, textTexture, &textViewport, &renderRect);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(textTexture);
}
void showHelpWindow(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color blackColor = {0, 0, 0};
    bool quitHelpWindow = false;
    SDL_Event event;

    // Help text
    const std::string helpText =
        "Help\n\n"
        "1. Overview\n"
        "Welcome to the Casino Game! This is a simple, fun, and interactive application designed for entertainment purposes. Use this Help guide to understand how to navigate and play the game.\n\n"
        "2. Main Menu\n\n"
        "Play: Start the game and explore different casino games.\n"
        "Settings: Adjust preferences such as language and sound.\n"
        "About: Learn more about the developers and the technologies behind the game.\n"
        "Exit: Quit the application.\n\n"
        "3. Game Controls\n\n"
        "Mouse:\n"
        "Left-click to interact with buttons, cards, or other in-game elements.\n\n"
        "Keyboard:\n"
        "Use the arrow keys or mouse wheel to scroll through text or options.\n"
        "Press Esc to return to the previous menu or exit the current screen.\n\n"
        "4. Settings Menu\n\n"
        "Change Language: Switch between available languages (English, Chinese).\n"
        "Adjust Volume: Mute or unmute background music and sound effects.\n\n"
        "5. Tips for Playing\n\n"
        "Read the rules of each game carefully before starting.\n"
        "Practice makes perfect—use practice rounds to improve your skills.\n"
        "Have fun and play responsibly!\n\n"
        "6. Common Issues\n\n"
        "Screen Freezing:\n"
        "Restart the game or check for updates to SDL-related libraries.\n\n"
        "No Sound:\n"
        "Ensure your device audio is enabled and adjust volume settings in the game.\n\n"
        "Language Not Changing:\n"
        "Verify the settings and restart the application if necessary.\n\n"
        "7. Contact Us\n\n"
        "If you have any issues, suggestions, or feedback, feel free to contact us:\n"
        "Email: 123040017@link.cuhk.edu.cn\n";

    // Create texture for help text
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, helpText.c_str(), blackColor, WINDOW_WIDTH - 40);
    if (!textSurface) {
        std::cerr << "Failed to create text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    int scrollOffset = 0;
    const int scrollSpeed = 40; // Pixels per scroll action

    while (!quitHelpWindow) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitHelpWindow = true;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitHelpWindow = true; // Exit on Escape key
                } else if (event.key.keysym.sym == SDLK_UP) {
                    scrollOffset = std::max(scrollOffset - scrollSpeed, 0); // Scroll up
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    scrollOffset = std::min(scrollOffset + scrollSpeed, textHeight - WINDOW_HEIGHT + 80); // Scroll down
                }
            }

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scrollOffset = std::max(scrollOffset - scrollSpeed, 0); // Scroll up
                } else if (event.wheel.y < 0) {
                    scrollOffset = std::min(scrollOffset + scrollSpeed, textHeight - WINDOW_HEIGHT + 80); // Scroll down
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render help text
        SDL_Rect textViewport = {0, scrollOffset, textWidth, WINDOW_HEIGHT - 80};
        SDL_Rect renderRect = {20, 20, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 80};
        SDL_RenderCopy(renderer, textTexture, &textViewport, &renderRect);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(textTexture);
}

void showLanguageSelection(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color blackColor = {0, 0, 0};

    SDL_Rect englishButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300, 50};
    SDL_Rect chineseButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 20, 300, 50};

    // Render English Text
    SDL_Surface* englishSurface = TTF_RenderUTF8_Solid(font, "English", blackColor);
    SDL_Texture* englishTexture = SDL_CreateTextureFromSurface(renderer, englishSurface);
    SDL_FreeSurface(englishSurface);

    // Load Font with Chinese Character Support
    TTF_Font* chineseFont = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 24);
    if (!chineseFont) {
        std::cerr << "Failed to load Chinese font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Render Chinese Text
    SDL_Surface* chineseSurface = TTF_RenderUTF8_Solid(chineseFont, "中文", blackColor);
    SDL_Texture* chineseTexture = SDL_CreateTextureFromSurface(renderer, chineseSurface);
    SDL_FreeSurface(chineseSurface);
    TTF_CloseFont(chineseFont);

    if (!englishTexture || !chineseTexture) {
        std::cerr << "Failed to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    bool quitLanguageSelection = false;
    SDL_Event event;

    while (!quitLanguageSelection) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quitLanguageSelection = true;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= englishButton.x && mouseX <= englishButton.x + englishButton.w &&
                    mouseY >= englishButton.y && mouseY <= englishButton.y + englishButton.h) {
                    currentLanguage = "en"; // Set language to English
                    quitLanguageSelection = true;
                }

                if (mouseX >= chineseButton.x && mouseX <= chineseButton.x + chineseButton.w &&
                    mouseY >= chineseButton.y && mouseY <= chineseButton.y + chineseButton.h) {
                    currentLanguage = "zh"; // Set language to Chinese
                    quitLanguageSelection = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &englishButton);
        SDL_RenderDrawRect(renderer, &chineseButton);

        int textWidth, textHeight;

        // Draw English Text
        SDL_QueryTexture(englishTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect englishTextRect = {englishButton.x + (englishButton.w - textWidth) / 2, englishButton.y + (englishButton.h - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, englishTexture, nullptr, &englishTextRect);

        // Draw Chinese Text
        SDL_QueryTexture(chineseTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect chineseTextRect = {chineseButton.x + (chineseButton.w - textWidth) / 2, chineseButton.y + (chineseButton.h - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, chineseTexture, nullptr, &chineseTextRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(englishTexture);
    SDL_DestroyTexture(chineseTexture);
}

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
void showSettingsWindow(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* backButtonTexture) {
    SDL_Rect languageButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100, 300, 50};
    SDL_Rect helpButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 300, 50};
    SDL_Rect creditsButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 40, 300, 50};
    SDL_Rect termsButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 110, 300, 50};
    SDL_Rect backButton = {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 100, 100, 50};

    SDL_Color blackColor = {0, 0, 0};

    // Load header font (for larger text)
    TTF_Font* headerFont = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 48);
    if (!headerFont) {
        std::cerr << "Failed to load header font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Function to refresh button text textures
    auto refreshButtonTextures = [&](std::vector<SDL_Texture*>& textures) {
        textures.clear(); // Clear existing textures
        std::vector<std::pair<SDL_Rect, std::string>> buttons = {
            {languageButton, getLocalizedText("language")},
            {helpButton, getLocalizedText("help")},
            {creditsButton, getLocalizedText("credits")},
            {termsButton, getLocalizedText("terms")},
            {backButton, getLocalizedText("back")},
        };

        for (const auto& button : buttons) {
            SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, button.second.c_str(), blackColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            textures.push_back(textTexture);
        }
    };

    // Initialize textures
    std::vector<SDL_Texture*> buttonTextures;
    refreshButtonTextures(buttonTextures);

    // Header texture
    SDL_Surface* headerSurface = TTF_RenderUTF8_Solid(headerFont, getLocalizedText("settings").c_str(), blackColor);
    SDL_Texture* headerTexture = SDL_CreateTextureFromSurface(renderer, headerSurface);
    SDL_FreeSurface(headerSurface);

    int headerWidth, headerHeight;
    SDL_QueryTexture(headerTexture, nullptr, nullptr, &headerWidth, &headerHeight);
    SDL_Rect headerRect = {WINDOW_WIDTH / 2 - headerWidth / 2, 50, headerWidth, headerHeight};

    bool quitSettings = false;
    SDL_Event event;

    while (!quitSettings) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quitSettings = true;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Handle button clicks
                if (mouseX >= backButton.x && mouseX <= backButton.x + backButton.w &&
                    mouseY >= backButton.y && mouseY <= backButton.y + backButton.h) {
                    quitSettings = true; // Return to previous screen
                }

                if (mouseX >= languageButton.x && mouseX <= languageButton.x + languageButton.w &&
                    mouseY >= languageButton.y && mouseY <= languageButton.y + languageButton.h) {
                    showLanguageSelection(renderer, font);

                    // Refresh UI elements after language selection
                    SDL_DestroyTexture(headerTexture);
                    headerSurface = TTF_RenderUTF8_Solid(headerFont, getLocalizedText("settings").c_str(), blackColor);
                    headerTexture = SDL_CreateTextureFromSurface(renderer, headerSurface);
                    SDL_FreeSurface(headerSurface);

                    refreshButtonTextures(buttonTextures);
                }

                if (mouseX >= termsButton.x && mouseX <= termsButton.x + termsButton.w &&
                    mouseY >= termsButton.y && mouseY <= termsButton.y + termsButton.h) {
                    showTermsAndPrivacyWindow(renderer, font);
                }
                if (mouseX >= creditsButton.x && mouseX <= creditsButton.x + creditsButton.w &&
    mouseY >= creditsButton.y && mouseY <= creditsButton.y + creditsButton.h) {
                    showCreditsWindow(renderer, font);
    }
                if (mouseX >= helpButton.x && mouseX <= helpButton.x + helpButton.w &&
                    mouseY >= helpButton.y && mouseY <= helpButton.y + helpButton.h) {
                    showHelpWindow(renderer, font);
                    }

            }
        }

        // Render background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render header
        SDL_RenderCopy(renderer, headerTexture, nullptr, &headerRect);

        // Render buttons and their text
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        std::vector<SDL_Rect> buttonRects = {languageButton, helpButton, creditsButton, termsButton, backButton};
        for (size_t i = 0; i < buttonRects.size(); ++i) {
            SDL_RenderDrawRect(renderer, &buttonRects[i]);

            int textWidth, textHeight;
            SDL_QueryTexture(buttonTextures[i], nullptr, nullptr, &textWidth, &textHeight);
            SDL_Rect textRect = {buttonRects[i].x + (buttonRects[i].w - textWidth) / 2,
                                 buttonRects[i].y + (buttonRects[i].h - textHeight) / 2,
                                 textWidth, textHeight};
            SDL_RenderCopy(renderer, buttonTextures[i], nullptr, &textRect);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up textures
    SDL_DestroyTexture(headerTexture);
    for (SDL_Texture* texture : buttonTextures) {
        SDL_DestroyTexture(texture);
    }

    TTF_CloseFont(headerFont);
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

    TTF_Font* font = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        Mix_FreeChunk(typingSound);
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1; // Return an integer value
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

                    SDL_Surface* backButtonSurface = IMG_Load("assets/PNG/Button/back.png");
                    SDL_Texture* backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
                    SDL_FreeSurface(backButtonSurface);

                    fadeUpText(renderer, font, playerName);

                    showGameSelectionScreen(renderer, font, bgTexture, backButtonTexture, muteTexture, unmuteTexture, isMuted);

                    SDL_DestroyTexture(backButtonTexture);
                }

                if (mouseX >= settingsButton.x && mouseX <= settingsButton.x + settingsButton.w &&
                    mouseY >= settingsButton.y && mouseY <= settingsButton.y + settingsButton.h) {
                    SDL_Surface* backButtonSurface = IMG_Load("assets/PNG/Button/back.png");
                    SDL_Texture* backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
                    SDL_FreeSurface(backButtonSurface);

                    showSettingsWindow(renderer, font, backButtonTexture);

                    SDL_DestroyTexture(backButtonTexture);
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

    // Cleanup and quit
    TTF_CloseFont(font);
    Mix_FreeChunk(typingSound);
    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
