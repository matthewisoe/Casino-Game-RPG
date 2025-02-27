#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_main.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <unordered_map> // For std::unordered_map
#include <vector>        // For std::vector
#include "Game.h"
#include "SlotMachineGame.h"
#include "Player.h"
#include "Renderer.h"
#include "Button.h"
#include <boost/asio.hpp>
#include <thread>
#include "GameClient.h"
#include "LanguageManager.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void showSettingsWindow(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* backButtonTexture);

// Create the main window
SDL_Window* window = SDL_CreateWindow("Casino Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

Mix_Chunk* clickSound2 = nullptr; // Declare a sound effect for chips

std::string currentLanguage = "en"; // Global variable to track current language
std::string playerName = "";
Player init_player;
// Localized About content
std::unordered_map<std::string, std::string> aboutContent = {
    {"en",
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
        "Please play responsibly and keep it fun!"},
    {"zh",
        "欢迎来到Casino Game！\n\n"
        "这是一个有趣且互动的模拟赌场体验，仅供娱乐用途。"
        "沉浸在充满活力的虚拟赌场中，探索各种游戏、学习规则，并享受机会的刺激——"
        "所有这些都无需任何真实的风险。\n\n"
        "主要特点:\n"
        "- 一个美丽且用户友好的界面，旨在使您的游戏体验愉快。\n"
        "- 内置背景音乐和音效，提供身临其境的体验。\n"
        "- 清晰、适合初学者的说明，帮助您无缝进入赌场游戏。\n"
        "- 注重负责任的游戏：这仅是一个模拟平台，并非真正的赌博平台。\n\n"
        "免责声明:\n"
        "此游戏仅供娱乐用途。它不涉及真钱或任何形式的投注。"
        "请负责任地玩耍，保持乐趣！"}
};

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
// Function to fetch localized About content
std::string getLocalizedAbout() {
    return aboutContent[currentLanguage];
}
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


// Function to get localized text based on the current language
std::string getText(const std::string& key) {
    static std::map<std::string, std::map<std::string, std::string>> translations = {
            {"en", {
                           {"Language", "Language"},
                           {"Help", "Help"},
                           {"Credits", "Credits"},
                           {"Terms & Privacy", "Terms & Privacy"},
                           {"Back", "Back"},
                           {"Play", "Play"},
                           {"Settings", "Settings"},
                           {"About", "About"},
                           {"Enter_Name", "Enter your name"},
                           {"OK", "OK"} // For disclaimer window
                   }},
            {"zh", {
                           {"Language", "语言"},
                           {"Help", "帮助"},
                           {"Credits", "制作团队"},
                           {"Terms & Privacy", "条款与隐私"},
                           {"Back", "返回"},
                           {"Play", "开始游戏"},
                           {"Settings", "设置"},
                           {"About", "关于"},
                           {"Enter_Name", "输入你的名字"},
                           {"OK", "确定"} // For disclaimer window
                   }}
    };
    // Return the localized text if available, else return the key itself
    if (translations.find(currentLanguage) != translations.end() &&
        translations[currentLanguage].find(key) != translations[currentLanguage].end()) {
        return translations[currentLanguage][key];
    }
    return key; // Fallback to key if translation not found
}

// Localized Credits content
std::unordered_map<std::string, std::string> creditsContent = {
        {"en",
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
                "Email: 123040017@link.cuhk.edu.cn\n"},
        {"zh",
                "鸣谢\n\n"
                "1. 开发团队\n"
                "Matthew Irving Soenarjo\n"
                "Darren Clay Liem\n"
                "Ramses Piramus\n"
                "Goh Ean Jie\n"
                "Chi Cong Yu\n\n"
                "2. 库和框架\n"
                "SDL2: 用于图形和输入处理的简单多媒体层 (https://www.libsdl.org)\n"
                "SDL_ttf: 用于字体渲染 (https://www.libsdl.org/projects/SDL_ttf/)\n"
                "SDL_mixer: 用于音频支持 (https://www.libsdl.org/projects/SDL_mixer/)\n"
                "SDL_image: 用于加载图像 (https://www.libsdl.org/projects/SDL_image/)\n\n"
                "3. 字体\n"
                "Noto Sans SC: 谷歌提供的简体中文字体 (https://www.google.com/get/noto)\n\n"
                "4. 资源\n"
                "背景图像: [来源或归属，例如 freepik.com 等]\n"
                "音效: [来源或归属，例如 freesound.org 等]\n"
                "图标: [来源或归属，例如 flaticon.com 等]\n\n"
                "5. 特别鸣谢\n"
                "感谢所有在测试和反馈期间提供帮助的贡献者。\n"
                "感谢开源社区提供的宝贵工具和资源。\n\n"
                "6. 联系我们\n"
                "如果您有任何问题、建议或反馈，请联系我们：\n"
                "电子邮件: 123040017@link.cuhk.edu.cn\n"}
};

// Function to fetch localized Credits content
std::string getLocalizedCredits() {
    return creditsContent[currentLanguage];
}

std::unordered_map<std::string, std::string> helpContent = {
        {"en",
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
                "Email: 123040017@link.cuhk.edu.cn\n"},
        {"zh",
                "帮助\n\n"
                "1. 概述\n"
                "欢迎使用 Casino Game！这是一款简单、有趣且互动的应用程序，旨在提供娱乐体验。请使用此帮助指南了解如何导航和玩游戏。\n\n"
                "2. 主菜单\n\n"
                "开始游戏：启动游戏并探索各种赌场游戏。\n"
                "设置：调整语言和声音等首选项。\n"
                "关于：了解开发人员和游戏背后的技术。\n"
                "退出：退出应用程序。\n\n"
                "3. 游戏控制\n\n"
                "鼠标：\n"
                "左键单击与按钮、牌或其他游戏元素交互。\n\n"
                "键盘：\n"
                "使用箭头键或鼠标滚轮滚动文本或选项。\n"
                "按 Esc 返回上一级菜单或退出当前屏幕。\n\n"
                "4. 设置菜单\n\n"
                "更改语言：在可用语言（英文、中文）之间切换。\n"
                "调整音量：静音或取消静音背景音乐和音效。\n\n"
                "5. 游戏提示\n\n"
                "在开始之前仔细阅读每个游戏的规则。\n"
                "熟能生巧——使用练习轮提高技能。\n"
                "玩得开心，合理娱乐！\n\n"
                "6. 常见问题\n\n"
                "屏幕冻结：\n"
                "重新启动游戏或检查 SDL 相关库的更新。\n\n"
                "没有声音：\n"
                "确保设备音频已启用，并在游戏中调整音量设置。\n\n"
                "语言未更改：\n"
                "验证设置并根据需要重新启动应用程序。\n\n"
                "7. 联系我们\n\n"
                "如果您有任何问题、建议或反馈，请随时联系我们：\n"
                "电子邮件：123040017@link.cuhk.edu.cn\n"}
};

// Function to fetch localized Help content
std::string getLocalizedHelp() {
    return helpContent[currentLanguage];
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

    // Fetch localized Credits content
    const std::string creditsText = getLocalizedCredits();

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

    // Fetch localized Help content
    const std::string helpText = getLocalizedHelp();

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

void initializeLocalizedResources() {
    LanguageManager& langManager = LanguageManager::getInstance();

    // English resources
    langManager.addLocalizedResource("en", "languageButton", "assets/button_image/languageButton.png");
    langManager.addLocalizedResource("en", "helpButton", "assets/button_image/helpButton.png");
    langManager.addLocalizedResource("en", "creditsButton", "assets/button_image/creditButton.png");
    langManager.addLocalizedResource("en", "termsButton", "assets/button_image/termsButton.png");
    langManager.addLocalizedResource("en", "backButton", "assets/button_image/backButton.png");
    langManager.addLocalizedResource("en", "settingsText", "Settings");
    langManager.addLocalizedResource("en", "balanceText", "Balance: $");
    langManager.addLocalizedResource("en", "currentbalanceText", "Current Balance: $");
    langManager.addLocalizedResource("en", "betText", "Bet: $");
    langManager.addLocalizedResource("en", "hitButton", "assets/button_image/hitButton.png");
    langManager.addLocalizedResource("en", "standButton", "assets/button_image/standButton.png");
    langManager.addLocalizedResource("en", "betButton", "assets/button_image/betButton.png");
    langManager.addLocalizedResource("en", "quitButton", "assets/button_image/quitButton.png");
    langManager.addLocalizedResource("en", "blackjackTable", "assets/table_image/blackjacktable.png");
    langManager.addLocalizedResource("en", "tieMessage", "It's a Tie! Dealer has Blackjack too!");
    langManager.addLocalizedResource("en", "winblackjackMessage", "You Win with Blackjack!");
    langManager.addLocalizedResource("en", "losebustMessage", "You Lose! (Bust)");
    langManager.addLocalizedResource("en", "dealerbustMessage", "Dealer Busts! You Win!");
    langManager.addLocalizedResource("en", "winhigherscoreMessage", "You Win! You had a higher score");
    langManager.addLocalizedResource("en", "losedealerhigherMessage", "You Lose! Dealer had higher score");
    langManager.addLocalizedResource("en", "tiebetpushesMessage", "It's a Tie! The bet pushes");
    langManager.addLocalizedResource("en", "bet_button", "assets/bet.png");
    langManager.addLocalizedResource("en", "spin_button", "assets/spin.png");
    langManager.addLocalizedResource("en", "bet_button_pressed", "assets/bet_pressed.png");
    langManager.addLocalizedResource("en", "spin_button_pressed", "assets/spin_pressed.png");
    langManager.addLocalizedResource("en", "startButton", "assets/button_image/startButton.png");

    // Chinese resources
    langManager.addLocalizedResource("zh", "languageButton", "assets/button_image/languageButtonCH.png");
    langManager.addLocalizedResource("zh", "helpButton", "assets/button_image/helpButtonCH.png");
    langManager.addLocalizedResource("zh", "creditsButton", "assets/button_image/creditButtonCH.png");
    langManager.addLocalizedResource("zh", "termsButton", "assets/button_image/termsButtonCH.png");
    langManager.addLocalizedResource("zh", "backButton", "assets/button_image/backButtonCH.png");
    langManager.addLocalizedResource("zh", "settingsText", "设置");
    langManager.addLocalizedResource("zh", "balanceText", "余额: $");
    langManager.addLocalizedResource("zh", "currentbalanceText", "当前余额: $");
    langManager.addLocalizedResource("zh", "betText", "下注: $");
    langManager.addLocalizedResource("zh", "hitButton", "assets/button_image/hitButtonCH.png");
    langManager.addLocalizedResource("zh", "standButton", "assets/button_image/standButtonCH.png");
    langManager.addLocalizedResource("zh", "betButton", "assets/button_image/betButtonCH.png");
    langManager.addLocalizedResource("zh", "quitButton", "assets/button_image/quitButtonCH.png");
    langManager.addLocalizedResource("zh", "blackjackTable", "assets/table_image/blackjacktableCH.png");
    langManager.addLocalizedResource("zh", "tieMessage", "平局了！庄家也有黑杰克！");
    langManager.addLocalizedResource("zh", "winblackjackMessage", "你赢了！你拿到了黑杰克！");
    langManager.addLocalizedResource("zh", "losebustMessage", "你输了！（爆牌）");
    langManager.addLocalizedResource("zh", "dealerbustMessage", "庄家爆牌！你赢了！！");
    langManager.addLocalizedResource("zh", "winhigherscoreMessage", "你赢了！你的得分更高！");
    langManager.addLocalizedResource("zh", "losedealerhigherMessage", "你输了！庄家的得分更高！");
    langManager.addLocalizedResource("zh", "tiebetpushesMessage", "平局！赌注原数退回！");
    langManager.addLocalizedResource("zh", "bet_button", "assets/betCH.png");
    langManager.addLocalizedResource("zh", "spin_button", "assets/spinCH.png");
    langManager.addLocalizedResource("zh", "bet_button_pressed", "assets/bet_pressedCH.png");
    langManager.addLocalizedResource("zh", "spin_button_pressed", "assets/spin_pressedCH.png");
    langManager.addLocalizedResource("zh", "startButton", "assets/button_image/startButtonCH.png");

}

void showLanguageSelection(SDL_Renderer* renderer, TTF_Font* font) {

    SDL_Color blackColor = {0, 0, 0};
    Renderer render(renderer);

    SDL_Texture* englishButtonTexture = render.LoadTexture("assets/button_image/englishButton.png");
    SDL_Texture* zhongwenButtonTexture = render.LoadTexture("assets/button_image/zhongwenButton.png");

    Button englishButton(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 150, 320, 140, englishButtonTexture);
    Button zhongwenButton(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 20, 320, 140, zhongwenButtonTexture);

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

                if (englishButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    LanguageManager::getInstance().setLanguage("en");
                    currentLanguage = "en";
                    quitLanguageSelection = true; // Exit language selection
                }

                if (zhongwenButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    LanguageManager::getInstance().setLanguage("zh");
                    currentLanguage = "zh";
                    quitLanguageSelection = true; // Exit language selection
                }
            }
        }

        // Render language selection screen
        SDL_RenderClear(renderer);
        englishButton.Render(renderer);
        zhongwenButton.Render(renderer);
        SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
        SDL_RenderClear(renderer);
    }

    SDL_DestroyTexture(englishButtonTexture);
    SDL_DestroyTexture(zhongwenButtonTexture);
}

void registerLocalizedResources() {
    LanguageManager& langManager = LanguageManager::getInstance();

    // English resources
    langManager.addLocalizedResource("en", "pokerBubble", "assets/blackjackbubble.png");
    langManager.addLocalizedResource("en", "slotBubble", "assets/slotsbubble.png");
    langManager.addLocalizedResource("en", "exitBubble", "assets/confirmation_popup.png");
    langManager.addLocalizedResource("en", "balanceBubble", "assets/balancebubble.png");
    langManager.addLocalizedResource("en", "currentbalancetext", "Current Balance: $");

    // Chinese resources
    langManager.addLocalizedResource("zh", "pokerBubble", "assets/blackjackbubbleCH.png");
    langManager.addLocalizedResource("zh", "slotBubble", "assets/slotsbubbleCH.png");
    langManager.addLocalizedResource("zh", "exitBubble", "assets/confirmation_popupCH.png");
    langManager.addLocalizedResource("zh", "balanceBubble", "assets/balancebubbleCH.png");
    langManager.addLocalizedResource("zh", "currentbalancetext", "当前余额: $");
}

// Helper function to create a texture from localized text
SDL_Texture* createTextTexture(const std::string& key, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    std::string text = getText(key);
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to render text for key \"" << key << "\": " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from surface for key \"" << key << "\": " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(textSurface);
    return textTexture;
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
    SDL_Surface* windowIcon = IMG_Load("assets/PNG/button/warning.png"); // Specify path to icon image
    if (!windowIcon) {
        std::cerr << "Unable to load icon: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetWindowIcon(disclaimerWindow, windowIcon);  // Set the icon for the window
        SDL_FreeSurface(windowIcon);  // Free the surface after setting it as the icon
    }

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

// Show About window with dynamic content based on currentLanguage
void showAboutWindow(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color blackColor = {0, 0, 0};
    bool quitAboutWindow = false;
    SDL_Event event;

    // Fetch localized About content
    const std::string aboutText = getLocalizedAbout();

    // Create a surface for the localized About text
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, aboutText.c_str(), blackColor, WINDOW_WIDTH - 40);
    if (!textSurface) {
        std::cerr << "Failed to create About text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    int scrollOffset = 0; // Initial scroll position
    const int scrollSpeed = 40; // Pixels to scroll per action

    while (!quitAboutWindow) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitAboutWindow = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitAboutWindow = true; // Close About window on Esc
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

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        // Render About text
        SDL_Rect textViewport = {0, scrollOffset, textWidth, WINDOW_HEIGHT - 80};
        SDL_Rect renderRect = {20, 20, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 80};
        SDL_RenderCopy(renderer, textTexture, &textViewport, &renderRect);

        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(textTexture);
}

void showSettingsWindow(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* backButtonTexture) {
    Renderer render(renderer);

    // Load language-specific button textures using LanguageManager
    SDL_Texture* languageButtonTexture = render.LoadTexture(
        LanguageManager::getInstance().getLocalizedResource("languageButton").c_str());
    SDL_Texture* helpButtonTexture = render.LoadTexture(
        LanguageManager::getInstance().getLocalizedResource("helpButton").c_str());
    SDL_Texture* creditsButtonTexture = render.LoadTexture(
        LanguageManager::getInstance().getLocalizedResource("creditsButton").c_str());
    SDL_Texture* termsButtonTexture = render.LoadTexture(
        LanguageManager::getInstance().getLocalizedResource("termsButton").c_str());
    backButtonTexture = render.LoadTexture(
        LanguageManager::getInstance().getLocalizedResource("backButton").c_str());

    Button languageButton(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 140, 200, 70, languageButtonTexture);
    Button helpButton(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 60, 205, 75, helpButtonTexture);
    Button creditsButton(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 20, 200, 70, creditsButtonTexture);
    Button termsButton(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 100, 200, 70, termsButtonTexture);
    Button backButton(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 100, 100, 50, backButtonTexture);

    SDL_Color blackColor = {255, 255, 255};

    TTF_Font* headerFont = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 45);
    TTF_SetFontStyle(headerFont, TTF_STYLE_BOLD);
    if (!headerFont) {
        std::cerr << "Failed to load header font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Header texture using localized text
    std::string headerText = LanguageManager::getInstance().getLocalizedResource("settingsText");
    SDL_Surface* headerSurface = TTF_RenderUTF8_Solid(headerFont, headerText.c_str(), blackColor);
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
                if (backButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    quitSettings = true; // Return to previous screen
                }

                if (languageButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    showLanguageSelection(renderer, font);

                    // Refresh button textures after returning
                    SDL_DestroyTexture(languageButtonTexture);
                    SDL_DestroyTexture(helpButtonTexture);
                    SDL_DestroyTexture(creditsButtonTexture);
                    SDL_DestroyTexture(termsButtonTexture);
                    SDL_DestroyTexture(backButtonTexture);

                    languageButtonTexture = render.LoadTexture(
                        LanguageManager::getInstance().getLocalizedResource("languageButton").c_str());
                    helpButtonTexture = render.LoadTexture(
                        LanguageManager::getInstance().getLocalizedResource("helpButton").c_str());
                    creditsButtonTexture = render.LoadTexture(
                        LanguageManager::getInstance().getLocalizedResource("creditsButton").c_str());
                    termsButtonTexture = render.LoadTexture(
                        LanguageManager::getInstance().getLocalizedResource("termsButton").c_str());
                    backButtonTexture = render.LoadTexture(
                        LanguageManager::getInstance().getLocalizedResource("backButton").c_str());

                    // Update button textures
                    languageButton.UpdateTexture(languageButtonTexture);
                    helpButton.UpdateTexture(helpButtonTexture);
                    creditsButton.UpdateTexture(creditsButtonTexture);
                    termsButton.UpdateTexture(termsButtonTexture);
                    backButton.UpdateTexture(backButtonTexture);

                    // Update header text to reflect new language
                    SDL_DestroyTexture(headerTexture);
                    headerText = getText("Settings");
                    headerSurface = TTF_RenderUTF8_Solid(headerFont, headerText.c_str(), blackColor);
                    headerTexture = SDL_CreateTextureFromSurface(renderer, headerSurface);
                    SDL_FreeSurface(headerSurface);
                }

                if (termsButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    showTermsAndPrivacyWindow(renderer, font);
                }
                if (creditsButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    showCreditsWindow(renderer, font);
                }
                if (helpButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    showHelpWindow(renderer, font);
                }
            }
        }

        // Render settings screen
        SDL_RenderClear(renderer);
        languageButton.Render(renderer);
        helpButton.Render(renderer);
        creditsButton.Render(renderer);
        termsButton.Render(renderer);
        backButton.Render(renderer);
        SDL_RenderCopy(renderer, headerTexture, nullptr, &headerRect);
        SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
        SDL_RenderClear(renderer);
    }

    SDL_DestroyTexture(headerTexture);
    TTF_CloseFont(headerFont);

    // Clean up button textures
    SDL_DestroyTexture(languageButtonTexture);
    SDL_DestroyTexture(helpButtonTexture);
    SDL_DestroyTexture(creditsButtonTexture);
    SDL_DestroyTexture(termsButtonTexture);
    SDL_DestroyTexture(backButtonTexture);
}

// Function to draw a thicker border around a rectangle
void drawThickerBorder(SDL_Renderer* renderer, SDL_Rect rect, int thickness) {
    for (int i = 0; i < thickness; ++i) {
        SDL_Rect borderRect = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
        SDL_RenderDrawRect(renderer, &borderRect);
    }
}
void showGameWindow(SDL_Renderer* renderer, GameClient& client) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Game* game = new Game(init_player, client);

    const int FPS = 60;                     // Target frames per second
    const int frameDelay = 1000 / FPS;      // Milliseconds per frame

    Uint32 frameStart;                      // Time the frame starts
    int frameTime;                          // Time to render the frame

    // Initialize the game
    game->init("CasinoMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
    // Main game loop
    while (game->running()) {
        frameStart = SDL_GetTicks();

        // Handle events, update game logic, and render
        game->handleEvents();
        game->update();
        game->render();

        // Calculate frame time and apply delay if necessary
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up resources
    game->clean();

    // Quit SDL subsystem
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void showGameSelectionScreen(SDL_Renderer* renderer, TTF_Font* font,
                             SDL_Texture* bgTexture, SDL_Texture* backButtonTexture,
                             SDL_Texture* muteTexture, SDL_Texture* unmuteTexture, bool& isMuted,GameClient& client) {
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, "亚当", {0, 0, 0});
    if (!textSurface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from text surface: " << SDL_GetError() << std::endl;
        return;
    }
    // Define static arrays for character names
    std::string boyNamesChinese[] = {"亚当", "伯纳德", "查理"};
    std::string boyNamesEnglish[] = {"Adam", "Bernard", "Charlie"};
    std::string girlNamesChinese[] = {"爱丽丝", "贝亚特丽斯", "凯瑟琳"};
    std::string girlNamesEnglish[] = {"Alice", "Beatrice", "Catherine"};
    // Determine character names based on current language
    const std::string* boyNames = (currentLanguage == "zh") ? boyNamesChinese : boyNamesEnglish;
    const std::string* girlNames = (currentLanguage == "zh") ? girlNamesChinese : girlNamesEnglish;

    // Determine button paths based on the current language
    const char* boyButtonPath = (currentLanguage == "zh")
                                ? "assets/PNG/Button/boy_button1CH.png"
                                : "assets/PNG/Button/boy_button1.png";
    const char* girlButtonPath = (currentLanguage == "zh")
                                 ? "assets/PNG/Button/girl_button1CH.png"
                                 : "assets/PNG/Button/girl_button1.png";

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
    SDL_Surface* boyButtonSurface = IMG_Load(boyButtonPath);
    SDL_Surface* girlButtonSurface = IMG_Load(girlButtonPath);
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
    SDL_Color textColor = {0, 0, 0}; // Black color

    // Text for "Choose your own character" in multiple languages
    const char* titleText = (currentLanguage == "zh")
                            ? "选择你的角色"
                            : "Choose your own character";

    // Render the title text
    SDL_Surface* titleSurface = TTF_RenderUTF8_Solid(font, titleText, textColor);
    if (!titleSurface) {
        std::cerr << "Failed to render title text: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = {(WINDOW_WIDTH - titleSurface->w) / 2, 120, titleSurface->w, titleSurface->h}; // Top-center position
    SDL_FreeSurface(titleSurface);


    SDL_Texture* boyNameTextures[3], * girlNameTextures[3];
    SDL_Rect nameRects[3];

    // Create text for character names
    for (int i = 0; i < 3; ++i) {
        SDL_Surface* boyNameSurface = TTF_RenderUTF8_Solid(font, boyNames[i].c_str(), textColor);
        if (!boyNameSurface) {
            std::cerr << "Failed to render boy name: " << TTF_GetError() << std::endl;
            continue;
        }
        boyNameTextures[i] = SDL_CreateTextureFromSurface(renderer, boyNameSurface);
        nameRects[i] = {charRects[i].x + (charRects[i].w - boyNameSurface->w) / 2,
                        charRects[i].y + charRects[i].h + 5, // Position below the character sprite
                        boyNameSurface->w, boyNameSurface->h};
        SDL_FreeSurface(boyNameSurface);

        SDL_Surface* girlNameSurface = TTF_RenderUTF8_Solid(font, girlNames[i].c_str(), textColor);
        if (!girlNameSurface) {
            std::cerr << "Failed to render girl name: " << TTF_GetError() << std::endl;
            continue;
        }
        girlNameTextures[i] = SDL_CreateTextureFromSurface(renderer, girlNameSurface);
        SDL_FreeSurface(girlNameSurface);
    }
    // Text for "PROCEED" button in multiple languages
    const char* proceedText = (currentLanguage == "zh") ? "继续" : "PROCEED";

    bool quitGameSelection = false;
    SDL_Event event;
    int selectedCharacter = -1; // -1 means no character selected
    bool isBoySelected = true;   // Flag to check if Boy or Girl set is selected
    int selectedBoyIndex = -1, selectedGirlIndex = -1; // Tracks selected boy or girl character index

    Mix_Chunk* clickSound2 = Mix_LoadWAV("assets/sounds/clickSound2.mp3");
    if (clickSound2 == nullptr) {
        std::cerr << "Failed to load click sound: " << Mix_GetError() << std::endl;
        return;
    }

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

        if (selectedBoyIndex != -1 || selectedGirlIndex != -1) {
            // Draw the green "PROCEED" button
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
            SDL_RenderFillRect(renderer, &proceedButtonRect);  // Draw the button rectangle

            // Localized text for "PROCEED"
            const char* proceedText = (currentLanguage == "zh") ? "继续" : "PROCEED";

            // Render the "PROCEED" text
            SDL_Surface* proceedSurface = TTF_RenderUTF8_Solid(font, proceedText, textColor);
            if (!proceedSurface) {
                std::cerr << "Failed to render proceed text: " << TTF_GetError() << std::endl;
            } else {
                SDL_Texture* proceedTexture = SDL_CreateTextureFromSurface(renderer, proceedSurface);
                if (!proceedTexture) {
                    std::cerr << "Failed to create proceed text texture: " << SDL_GetError() << std::endl;
                } else {
                    // Center the text in the button
                    SDL_Rect proceedTextRect = {
                        proceedButtonRect.x + (proceedButtonRect.w - proceedSurface->w) / 2,
                        proceedButtonRect.y + (proceedButtonRect.h - proceedSurface->h) / 2,
                        proceedSurface->w,
                        proceedSurface->h
                    };

                    SDL_RenderCopy(renderer, proceedTexture, nullptr, &proceedTextRect);
                    SDL_DestroyTexture(proceedTexture); // Clean up texture after use
                }
                SDL_FreeSurface(proceedSurface); // Free the surface
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
                    Mix_PlayChannel(-1, clickSound2, 0); // Play click sound once
                    isBoySelected = true; // Select boy character set
                }

                // Check if the girl button is clicked
                if (x >= girlButtonRect.x && x <= girlButtonRect.x + girlButtonRect.w &&
                    y >= girlButtonRect.y && y <= girlButtonRect.y + girlButtonRect.h) {
                    Mix_PlayChannel(-1, clickSound2, 0); // Play click sound once
                    isBoySelected = false; // Select girl character set
                }

                // Check if mute/unmute button is clicked
                if (x >= muteButtonRect.x && x <= muteButtonRect.x + muteButtonRect.w &&
                    y >= muteButtonRect.y && y <= muteButtonRect.y + muteButtonRect.h) {
                    isMuted = !isMuted; // Toggle the mute state
                    if (isMuted) {
                        Mix_PauseMusic(); // Pause music
                    } else {
                        Mix_ResumeMusic(); // Resume music
                    }
                }

                // Check if the back button is clicked
                if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                    y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                    Mix_Chunk* clickSound2 = nullptr; // Declare a sound effect for chips
                    quitGameSelection = true;
                }

                // Check if the proceed button is clicked (only when a character is selected)
                if (x >= proceedButtonRect.x && x <= proceedButtonRect.x + proceedButtonRect.w &&
                    y >= proceedButtonRect.y && y <= proceedButtonRect.y + proceedButtonRect.h) {
                    Mix_PlayChannel(-1, clickSound2, 0); // Play click sound once
                    if (selectedBoyIndex != -1 || selectedGirlIndex != -1) {
                        // Proceed to the next screen or action here
                        if (selectedBoyIndex != -1) {
                            std::cout << "Proceeding with " << boyNames[selectedBoyIndex] << " character!" << std::endl;
                        } else if (selectedGirlIndex != -1) {
                            std::cout << "Proceeding with " << girlNames[selectedGirlIndex] << " character!" << std::endl;
                        }

                        // Call showGameWindow to transition to the game screen
                        int skinId = (selectedBoyIndex != -1) ? selectedBoyIndex : selectedGirlIndex + 3;
                        init_player.setSkinId(skinId);
                        showGameWindow(renderer,client); // Pass the player's name and skin ID
                        quitGameSelection = true;
                    }
                }

                // Check if a character is clicked
                for (int i = 0; i < 3; ++i) {
                    if (x >= charRects[i].x && x <= charRects[i].x + charRects[i].w &&
                        y >= charRects[i].y && y <= charRects[i].y + charRects[i].h) {
                        Mix_PlayChannel(-1, clickSound2, 0); // Play click sound once
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

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, message.c_str(), textColor);
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

void runNetworking(boost::asio::io_context& io_context, GameClient& client) {
    try {
        // coonnect the client to the server
        client.connect("172.16.217.236", "12902");

        // Get the player ID from the server
        int player_id = client.send_login();

        // Create a shared_ptr to the tcp_connection for the client
        auto connection = tcp_connection::create(io_context);

        // Instantiate the global init_player object
        init_player = Player(player_id, connection);

        // run the networking loop
        client.run();
    } catch (const std::exception& e) {
        std::cerr << "Networking error: " << e.what() << std::endl;
    }
}

// Main function
int main(int argc, char* args[]) {
    try {
        // Create io_context and GameClient instance
        boost::asio::io_context io_context;
        GameClient client(io_context);

        // Start networking in a separate thread
        std::thread networkThread(runNetworking, std::ref(io_context), std::ref(client));

        initializeLocalizedResources();
        registerLocalizedResources();
    // Initialize SDL with video and audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_image for PNG support
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_ttf for font rendering
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_mixer for audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load background music and typing sound effect
    Mix_Music* bgMusic = Mix_LoadMUS("assets/sounds/bgm.mp3");
    Mix_Chunk* typingSound = Mix_LoadWAV("assets/sounds/click.wav");
    if (!bgMusic || !typingSound) {
        std::cerr << "Failed to load audio files! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(bgMusic);
        Mix_FreeChunk(typingSound);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Mix_VolumeChunk(typingSound, 150); // Set typing sound volume
    Mix_PlayMusic(bgMusic, -1);         // Play background music infinitely

    // Load the main font (supports both English and Chinese)
    TTF_Font* font = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        Mix_FreeMusic(bgMusic);
        Mix_FreeChunk(typingSound);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1; // Return an integer value
    }

    // Show disclaimer window first
    showDisclaimerWindow(font);

    if (!window) {
        std::cerr << "Failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        Mix_FreeChunk(typingSound);
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create the renderer for the main window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        Mix_FreeChunk(typingSound);
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load and create textures for background and logo
    SDL_Surface* bgSurface = IMG_Load("assets/PNG/Background/bg3.png");
    if (!bgSurface) {
        std::cerr << "Failed to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!bgTexture) {
        std::cerr << "Failed to create background texture! SDL_Error: " << SDL_GetError() << std::endl;
    }

    SDL_Surface* logoSurface = IMG_Load("assets/PNG/Logo/logo.png");
    if (!logoSurface) {
        std::cerr << "Failed to load logo image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);
    if (!logoTexture) {
        std::cerr << "Failed to create logo texture! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Load mute and unmute button textures
    SDL_Surface* muteSurface = IMG_Load("assets/PNG/Button/mute1.png");
    SDL_Surface* unmuteSurface = IMG_Load("assets/PNG/Button/unmute1.png");
    if (!muteSurface || !unmuteSurface) {
        std::cerr << "Failed to load mute/unmute images! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    SDL_Texture* muteTexture = SDL_CreateTextureFromSurface(renderer, muteSurface);
    SDL_Texture* unmuteTexture = SDL_CreateTextureFromSurface(renderer, unmuteSurface);
    SDL_FreeSurface(muteSurface);
    SDL_FreeSurface(unmuteSurface);
    if (!muteTexture || !unmuteTexture) {
        std::cerr << "Failed to create mute/unmute textures! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Start text input for player name
    SDL_StartTextInput();

    // Load the chip sound effect
    clickSound2 = Mix_LoadWAV("assets/sounds/clickSound2.mp3");
    if (!clickSound2) {
        SDL_Log("Failed to load chip sound: %s", Mix_GetError());
        return 0;
    }
    Renderer render(renderer);

    SDL_Texture* playButtonTexture = render.LoadTexture("assets/button_image/playButton.png");
    SDL_Texture* settingsButtonTexture = render.LoadTexture("assets/button_image/settingsButton.png");
    SDL_Texture* aboutButtonTexture = render.LoadTexture("assets/button_image/aboutButton.png");

    Button playButton(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 + 40, 140, 60, playButtonTexture);
    Button settingsButton(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 + 100, 140, 60, settingsButtonTexture);
    Button aboutButton(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 + 160, 140, 60, aboutButtonTexture);

    // Initialize player name and UI element rectangles
    SDL_Rect inputBox = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300, 50};
    SDL_Rect muteButtonRect = {WINDOW_WIDTH - 60, WINDOW_HEIGHT - 60, 50, 50};

    bool isMuted = false;
    SDL_Color textColor = {0, 0, 0};               // Black color for input text
    SDL_Color placeholderColor = {192, 192, 192}; // Gray color for placeholder
    SDL_Color whiteColor = {255, 255, 255};       // White color for button texts

    // Create button textures with localized texts
    SDL_Texture* playTextTexture = createTextTexture("Play", font, whiteColor, renderer);
    SDL_Texture* settingsTextTexture = createTextTexture("Settings", font, whiteColor, renderer);
    SDL_Texture* aboutTextTexture = createTextTexture("About", font, whiteColor, renderer);

    if (!playTextTexture || !settingsTextTexture || !aboutTextTexture) {
        std::cerr << "Failed to create one or more button textures!" << std::endl;
        // Handle error appropriately, possibly by exiting
    }

    bool quit = false;
    SDL_Event event;
    bool gameStarted = false;

    // Variable to track previous language for detecting changes
    std::string previousLanguage = currentLanguage;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            // Handle quit event
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            // Handle text input for player name
            if (event.type == SDL_TEXTINPUT && !gameStarted) {
                playerName += event.text.text;
                Mix_PlayChannel(-1, typingSound, 0);
            }

            // Handle backspace for player name
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0 && !gameStarted) {
                playerName.pop_back();
                Mix_PlayChannel(-1, typingSound, 0);
            }

            // Handle mouse button clicks
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Handle Play Button Click
                if (playButton.IsClicked(mouseX, mouseY)) {
                    Mix_Chunk* clickSound2 = nullptr; // Declare a sound effect for chips
                    if (!playerName.empty()) { // Ensure player has entered a name
                        gameStarted = true;

                        // Load back button texture
                        SDL_Surface* backButtonSurface = IMG_Load("assets/PNG/Button/back1.png");
                        if (!backButtonSurface) {
                            std::cerr << "Failed to load back button image: " << IMG_GetError() << std::endl;
                            continue;
                        }
                        SDL_Texture* backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
                        SDL_FreeSurface(backButtonSurface);
                        if (!backButtonTexture) {
                            std::cerr << "Failed to create back button texture: " << SDL_GetError() << std::endl;
                            continue;
                        }

                        // Fade up text with player's name
                        fadeUpText(renderer, font, playerName);

                        init_player.setName(playerName);
                        // Show game selection screen
                        showGameSelectionScreen(renderer, font, bgTexture, backButtonTexture, muteTexture, unmuteTexture, isMuted, client);


                        // Destroy back button texture after use
                        SDL_DestroyTexture(backButtonTexture);
                    } else {
                        // Optionally, provide feedback that the name is required
                        std::cerr << "Please enter your name before playing." << std::endl;
                    }
                }

                // Handle Settings Button Click
                if (settingsButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);

                    // Store previous language before opening settings
                    previousLanguage = currentLanguage;

                    // Load back button texture
                    SDL_Surface* backButtonSurface = IMG_Load("assets/PNG/Button/back1.png");
                    if (!backButtonSurface) {
                        std::cerr << "Failed to load back button image: " << IMG_GetError() << std::endl;
                        continue;
                    }
                    SDL_Texture* backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
                    SDL_FreeSurface(backButtonSurface);
                    if (!backButtonTexture) {
                        std::cerr << "Failed to create back button texture: " << SDL_GetError() << std::endl;
                        continue;
                    }

                    // Show Settings window
                    showSettingsWindow(renderer, font, backButtonTexture);

                    // Destroy back button texture after use
                    SDL_DestroyTexture(backButtonTexture);

                    // After settings window is closed, check if language has changed
                    if (currentLanguage != previousLanguage) {
                        // Destroy old button textures
                        if (playTextTexture) SDL_DestroyTexture(playTextTexture);
                        if (settingsTextTexture) SDL_DestroyTexture(settingsTextTexture);
                        if (aboutTextTexture) SDL_DestroyTexture(aboutTextTexture);
                        // Reload button textures with the updated language
                        if (currentLanguage == "zh") {
                            playButtonTexture = render.LoadTexture("assets/button_image/startButtonCH.png");
                            settingsButtonTexture = render.LoadTexture("assets/button_image/settingsButtonCH.png");
                            aboutButtonTexture = render.LoadTexture("assets/button_image/aboutButtonCH.png");
                        } else {
                            playButtonTexture = render.LoadTexture("assets/button_image/playButton.png");
                            settingsButtonTexture = render.LoadTexture("assets/button_image/settingsButton.png");
                            aboutButtonTexture = render.LoadTexture("assets/button_image/aboutButton.png");
                        }

                        // Update button textures
                        playButton.UpdateTexture(playButtonTexture);
                        settingsButton.UpdateTexture(settingsButtonTexture);
                        aboutButton.UpdateTexture(aboutButtonTexture);

                        // Optionally, handle errors if any texture fails to load
                        if (!playButtonTexture || !settingsButtonTexture || !aboutButtonTexture) {
                            std::cerr << "Failed to update button textures for the new language!" << std::endl;
                        }
                    }
                }

                // Handle About Button Click
                if (aboutButton.IsClicked(mouseX, mouseY)) {
                    Mix_PlayChannel(-1, clickSound2, 0);
                    showAboutWindow(renderer, font);
                }

                // Handle Mute/Unmute Button Click
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

        // Rendering section

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render background if loaded successfully
        if (bgTexture) {
            SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        }

        // Render logo if loaded successfully
        if (logoTexture) {
            SDL_Rect logoRect = {(WINDOW_WIDTH - 200) / 2, 50, 200, 200};
            SDL_RenderCopy(renderer, logoTexture, nullptr, &logoRect);
        }

        // Render input box
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for input box background
        SDL_RenderFillRect(renderer, &inputBox);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for input box border
        SDL_RenderDrawRect(renderer, &inputBox);

        // Render player name or placeholder
        SDL_Surface* textSurface;
        if (playerName.empty()) {
            textSurface = TTF_RenderUTF8_Solid(font, getText("Enter_Name").c_str(), placeholderColor);
        } else {
            textSurface = TTF_RenderUTF8_Solid(font, playerName.c_str(), textColor);
        }
        if (!textSurface) {
            std::cerr << "Failed to render input text: " << TTF_GetError() << std::endl;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            std::cerr << "Failed to create input text texture: " << SDL_GetError() << std::endl;
        }

        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect textRect = {inputBox.x + 10, inputBox.y + (inputBox.h - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Render Play Button
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for buttons

        if (playTextTexture) {
            playButton.Render(renderer);
        }

        // Render Settings Button
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for buttons

        if (settingsTextTexture) {
            settingsButton.Render(renderer);
        }

        // Render About Button
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for buttons

        if (aboutTextTexture) {
            aboutButton.Render(renderer);
        }

        // Render Mute/Unmute Button
        if (muteTexture && unmuteTexture) {
            SDL_Texture* soundTexture = isMuted ? muteTexture : unmuteTexture;
            SDL_RenderCopy(renderer, soundTexture, nullptr, &muteButtonRect);
        }

        // Present the updated renderer
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approximately 60 FPS
    }

    // Destroy button textures
    if (playTextTexture) SDL_DestroyTexture(playTextTexture);
    if (settingsTextTexture) SDL_DestroyTexture(settingsTextTexture);
    if (aboutTextTexture) SDL_DestroyTexture(aboutTextTexture);
    if (muteTexture) SDL_DestroyTexture(muteTexture);
    if (unmuteTexture) SDL_DestroyTexture(unmuteTexture);
    if (logoTexture) SDL_DestroyTexture(logoTexture);
    if (bgTexture) SDL_DestroyTexture(bgTexture);

    // Free audio resources
    Mix_FreeMusic(bgMusic);
    Mix_FreeChunk(typingSound);

    Mix_CloseAudio();

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
}
