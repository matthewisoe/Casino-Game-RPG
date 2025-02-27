#include "Game.h"
#include <iostream>
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "SlotMachineGame.h"
#include "blackjackGame.h"
#include "Button.h"
#include "Renderer.h"
#include "ChipOnTable.h"
#include "Player.h"
#include "BalanceAdd.h"
#include <SDL_mixer.h>
#include "LanguageManager.h"
#include <SDL_ttf.h>
#include <string>
#include <locale>

Mix_Chunk* chipSound = nullptr;
Mix_Chunk* cardFlipSound = nullptr;
Mix_Chunk* clickSound = nullptr;
Mix_Chunk* bubbleSound = nullptr;

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Texture* exitBubbleTexture;
SDL_Texture* pokerBubbleTexture;
SDL_Texture* slotBubbleTexture;
SDL_Texture* balanceBubbleTexture;
SDL_Rect bubbleDest;
SDL_Rect exitBubbleDest;
SDL_Texture* guideTexture = nullptr;

bool showPokerBubble = false;       // Flag to control bubble visibility
bool showSlotBubble = false;
bool showExitBubble = false;
bool showBalanceBubble = false;

std::vector<ColliderComponent*> Game::colliders;

enum groupLabels : std::size_t {
    groupMap,
    groupWall,
    groupPlayers,
    groupPokerTables,
    groupSlotMachines,
    groupChairs
};

auto& pokertables(manager.getGroup(groupPokerTables));
auto& slotmachines(manager.getGroup(groupSlotMachines));
auto& dealer(manager.addEntity());
auto& pillar1(manager.addEntity());
auto& pillar2(manager.addEntity());
auto& wallUp(manager.addEntity());
auto& wallLeft(manager.addEntity());
auto& wallRight(manager.addEntity());
auto& wallDownLeft(manager.addEntity());
auto& wallDownRight(manager.addEntity());
auto& pokerTable1(manager.addEntity());
auto& pokerTable2(manager.addEntity());
auto& slotMachine1(manager.addEntity());
auto& slotMachine3(manager.addEntity());
auto& chair1(manager.addEntity());
auto& chair2(manager.addEntity());
auto& chair3(manager.addEntity());
auto& chair4(manager.addEntity());
auto& chair5(manager.addEntity());
auto& chair6(manager.addEntity());
auto& chair7(manager.addEntity());
auto& chair8(manager.addEntity());
auto& chair9(manager.addEntity());
auto& chair10(manager.addEntity());
auto& chair11(manager.addEntity());
auto& chair12(manager.addEntity());
auto& player(manager.addEntity());
auto& exitdoor(manager.addEntity());
auto& christmastree(manager.addEntity());
auto& barTable(manager.addEntity());

Game::Game(Player player, GameClient& client) : localPlayer(player), gameClient(client) {}
Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
    try{
    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystems initialised!..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window){
            std::cout << "Window created!" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            std::cerr << "Renderer is not initialized!" << std::endl;
            return;
        }


        // Add this to your initialization code (e.g., in `Game::openPokerGameWindow` or a similar method)
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("Mix_OpenAudio Error: %s", Mix_GetError());
            return;
        }

        font = TTF_OpenFont("assets/font/static/NotoSansSC-Regular.ttf", 24);
        if (font == nullptr) {  // Check if the font failed to load
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }


        // Load the chip sound effect
        chipSound = Mix_LoadWAV("assets/sounds/chipSound.wav");
        if (!chipSound) {
            SDL_Log("Failed to load chip sound: %s", Mix_GetError());
            return;
        }
        // Load the card flip sound effect
        cardFlipSound = Mix_LoadWAV("assets/sounds/cardflip.wav");
        if (!cardFlipSound) {
            SDL_Log("Failed to load card flip sound: %s", Mix_GetError());
            return;
        }
        clickSound = Mix_LoadWAV("assets/sounds/clicksound.mp3");
        if (!clickSound) {
            SDL_Log("Failed to load click sound: %s", Mix_GetError());
            return;
        }
        bubbleSound = Mix_LoadWAV("assets/sounds/popoutSound.mp3");
        if (!clickSound) {
            SDL_Log("Failed to load click sound: %s", Mix_GetError());
            return;
        }
        isRunning = true;
        // Check the current language and load the appropriate guide texture
        std::string currentLang = LanguageManager::getInstance().getLanguage();
        if (currentLang == "zh") {
            guideTexture = TextureManager::LoadTexture("assets/guideCH.png");
        } else {
            guideTexture = TextureManager::LoadTexture("assets/guide.png");
        }
        if (!guideTexture) {
            SDL_Log("Failed to load guide texture: %s", SDL_GetError());
            return;
        }
        isRunning = true;
    }
    map = new Map();

    //ecs implementation

    christmastree.addComponent<TransformComponent>(50.0f, 500.0f, 98, 56, 1);
    christmastree.addComponent<ColliderComponent>("Christmastree");
    christmastree.addComponent<SpriteComponent>("assets/christmastree.png");

    //Exit door
    exitdoor.addComponent<TransformComponent>(402.0f, 612.0f, 32,96,1);
    exitdoor.addComponent<ColliderComponent>("Exitdoor");
    //dealer
    dealer.addComponent<TransformComponent>((float)(width / 2 - 36 / 2), (float)(height / 2 - 100 / 2) - 50);
    dealer.addComponent<SpriteComponent>("assets/dealer.png");
    //player
    player.addComponent<TransformComponent>((float)(width / 2 - 36 / 2),
                                            (float)(height - 60)-60,
                                            (float)(width / 2 - 36 / 2),
                                            (float)(height - 60) -10,
                                            32, 20, 10, 16, 2);

    switch (localPlayer.getSkinId()) {
        case 0:
            player.addComponent<SpriteComponent>("assets/boychar1_anim.png", true);
            break;
        case 1:
            player.addComponent<SpriteComponent>("assets/boychar2_anim.png", true);
            break;
        case 2:
            player.addComponent<SpriteComponent>("assets/boychar3_anim.png", true);
            break;
        case 3:
            player.addComponent<SpriteComponent>("assets/girlchar1_anim.png", true);
            break;
        case 4:
            player.addComponent<SpriteComponent>("assets/girlchar2_anim.png", true);
            break;
        case 5:
            player.addComponent<SpriteComponent>("assets/girlchar3_anim.png", true);
            break;
        default:
            break;
    }

    // Set the default animation
    //player
    player.getComponent<SpriteComponent>().Play("IdleUp");
    player.addComponent<KeyboardController>(*this);
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    //pillar1
    pillar1.addComponent<TransformComponent>(288.0f, 2.0f, 144, 32, 1);
    pillar1.addComponent<SpriteComponent>("assets/pillar.png");
    pillar1.addComponent<ColliderComponent>("pillar1");
    pillar1.addGroup(groupWall);
    //pillar2
    pillar2.addComponent<TransformComponent>(480.0f, 2.0f, 144, 32, 1);
    pillar2.addComponent<SpriteComponent>("assets/pillar.png");
    pillar2.addComponent<ColliderComponent>("pillar2");
    pillar2.addGroup(groupWall);
    //wallUp
    wallUp.addComponent<TransformComponent>(2.0f, 2.0f, 128, 800, 1);
    wallUp.addComponent<ColliderComponent>("wallUp");
    wallUp.addGroup(groupWall);
    //wallLeft
    wallLeft.addComponent<TransformComponent>(2.0f, 2.0f, 800, 32, 1);
    wallLeft.addComponent<ColliderComponent>("wallLeft");
    wallLeft.addGroup(groupWall);
    //wallRight
    wallRight.addComponent<TransformComponent>(770.0f, 2.0f, 800, 32, 1);
    wallRight.addComponent<ColliderComponent>("wallRight");
    wallRight.addGroup(groupWall);
    //wallDownLeft
    wallDownLeft.addComponent<TransformComponent>(0.0f, 612.0f, 32, 352, 1);
    wallDownLeft.addComponent<ColliderComponent>("wallDownLeft");
    wallDownLeft.addGroup(groupWall);
    //wallDownRight
    wallDownRight.addComponent<TransformComponent>(450.0f, 612.0f, 32, 320, 1);
    wallDownRight.addComponent<ColliderComponent>("wallDownRight");
    wallDownRight.addGroup(groupWall);
    //bar table
    barTable.addComponent<TransformComponent>(288.0f, 98.0f, 110, 224, 1);
    barTable.addComponent<SpriteComponent>("assets/bar_table.png");
    barTable.addComponent<ColliderComponent>("barTable");
    //poker table 1
    pokerTable1.addComponent<TransformComponent>(96.f, 224.0f, 100, 192, 1);
    pokerTable1.addComponent<SpriteComponent>("assets/poker_table.png");
    pokerTable1.addComponent<ColliderComponent>("pokerTable1");
    pokerTable1.addGroup(groupPokerTables);
    //poker table 2
    pokerTable2.addComponent<TransformComponent>(512.0f, 224.0f, 100, 192, 1);
    pokerTable2.addComponent<SpriteComponent>("assets/poker_table.png");
    pokerTable2.addComponent<ColliderComponent>("pokerTable2");
    pokerTable2.addGroup(groupPokerTables);
    //slot machine1
    slotMachine1.addComponent<TransformComponent>(160.0f, 384.0f, 119, 84, 1);
    slotMachine1.addComponent<SpriteComponent>("assets/slot_machine.png");
    slotMachine1.addComponent<ColliderComponent>("slotMachine1");
    slotMachine1.addGroup(groupSlotMachines);
    //slot machine3
    slotMachine3.addComponent<TransformComponent>(544.0f, 384.0f, 119, 84, 1);
    slotMachine3.addComponent<SpriteComponent>("assets/slot_machine.png");
    slotMachine3.addComponent<ColliderComponent>("slotMachine3");
    slotMachine3.addGroup(groupSlotMachines);
    //chair1
    chair1.addComponent<TransformComponent>(64.0f, 256.0f, 36,26, 1);
    chair1.addComponent<SpriteComponent>("assets/chair_up.png");
    chair1.addComponent<ColliderComponent>("chair1");
    chair1.addGroup(groupChairs);
    //chair2
    chair2.addComponent<TransformComponent>(96.0f, 288.0f, 36,26, 1);
    chair2.addComponent<SpriteComponent>("assets/chair_up.png");
    chair2.addComponent<ColliderComponent>("chair2");
    chair2.addGroup(groupChairs);
    //chair3
    chair3.addComponent<TransformComponent>(160.0f,320.0f, 36,26, 1);
    chair3.addComponent<SpriteComponent>("assets/chair_up.png");
    chair3.addComponent<ColliderComponent>("chair3");
    chair3.addGroup(groupChairs);
    //chair4
    chair4.addComponent<TransformComponent>(224.0f, 320.0f, 36,26, 1);
    chair4.addComponent<SpriteComponent>("assets/chair_up.png");
    chair4.addComponent<ColliderComponent>("chair4");
    chair4.addGroup(groupChairs);
    //chair5
    chair5.addComponent<TransformComponent>(256.0f, 288.0f, 36,26, 1);
    chair5.addComponent<SpriteComponent>("assets/chair_up.png");
    chair5.addComponent<ColliderComponent>("chair5");
    chair5.addGroup(groupChairs);
    //chair6
    chair6.addComponent<TransformComponent>(288.0f, 256.0f, 36,26, 1);
    chair6.addComponent<SpriteComponent>("assets/chair_up.png");
    chair6.addComponent<ColliderComponent>("chair6");
    chair6.addGroup(groupChairs);
    //chair7
    chair7.addComponent<TransformComponent>(480.0f, 256.0f, 36,26, 1);
    chair7.addComponent<SpriteComponent>("assets/chair_up.png");
    chair7.addComponent<ColliderComponent>("chair7");
    chair7.addGroup(groupChairs);
    //chair8
    chair8.addComponent<TransformComponent>(512.0f, 288.0f, 36,26, 1);
    chair8.addComponent<SpriteComponent>("assets/chair_up.png");
    chair8.addComponent<ColliderComponent>("chair8");
    chair8.addGroup(groupChairs);
    //chair9
    chair9.addComponent<TransformComponent>(576.0f,320.0f, 36,26, 1);
    chair9.addComponent<SpriteComponent>("assets/chair_up.png");
    chair9.addComponent<ColliderComponent>("chair9");
    chair9.addGroup(groupChairs);
    //chair10
    chair10.addComponent<TransformComponent>(640.0f, 320.0f, 36,26, 1);
    chair10.addComponent<SpriteComponent>("assets/chair_up.png");
    chair10.addComponent<ColliderComponent>("chair10");
    chair10.addGroup(groupChairs);
    //chair11
    chair11.addComponent<TransformComponent>(672.0f, 288.0f, 36,26, 1);
    chair11.addComponent<SpriteComponent>("assets/chair_up.png");
    chair11.addComponent<ColliderComponent>("chair11");
    chair11.addGroup(groupChairs);
    //chair12
    chair12.addComponent<TransformComponent>(704.0f, 256.0f, 36,26, 1);
    chair12.addComponent<SpriteComponent>("assets/chair_up.png");
    chair12.addComponent<ColliderComponent>("chair12");
    chair12.addGroup(groupChairs);

    pokerBubbleTexture = TextureManager::LoadTexture("assets/blackjackbubble.png"); // Load bubble texture
    slotBubbleTexture = TextureManager::LoadTexture("assets/slotsbubble.png"); // Load slots bubble texture
    exitBubbleTexture = TextureManager::LoadTexture("assets/confirmation_popup.png");
    balanceBubbleTexture = TextureManager::LoadTexture("assets/balancebubble.png");

    std::string currentLang = LanguageManager::getInstance().getLanguage();

    pokerBubbleTexture = TextureManager::LoadTexture(LanguageManager::getInstance().getLocalizedResource("pokerBubble").c_str());
    slotBubbleTexture = TextureManager::LoadTexture(LanguageManager::getInstance().getLocalizedResource("slotBubble").c_str());
    exitBubbleTexture = TextureManager::LoadTexture(LanguageManager::getInstance().getLocalizedResource("exitBubble").c_str());
    balanceBubbleTexture = TextureManager::LoadTexture(LanguageManager::getInstance().getLocalizedResource("balanceBubble").c_str());

 // Notify server that the player joined lobby
    GameMessage msg;
    msg.type = GameMessage::JOIN_LOBBY;
    msg.player_id = localPlayer.getId();
    float xPos = player.getComponent<TransformComponent>().position.x;
    float yPos = player.getComponent<TransformComponent>().position.y;
    msg.data = localPlayer.getName() + " " + std::to_string(localPlayer.getSkinId()) + " " + std::to_string(xPos) + " " + std::to_string(yPos);
    gameClient.send_message(msg);
    } catch (const std::exception& e) {
        std::cerr << "Initialization error: " << e.what() << std::endl;
        clean();
        isRunning = false;
    }
}

void Game::renderText(const std::string& message, int x, int y, SDL_Color color) {
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, message.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    int textWidth = textSurface->w;  // Store width
    int textHeight = textSurface->h; // Store height

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect textRect = {x, y, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
        break;

        case SDL_KEYDOWN:
            if (showExitBubble) {
                if (event.key.keysym.sym == SDLK_y) {  // Player presses 'Y'
                    std::cout << "Exiting the casino and returning to game selection..." << std::endl;

                    // Clean up casino map resources
                    showExitBubble = false;  // Reset the bubble flag
                    clean();  // Cleanup resources of the Casino Map
                    exit(0);
                } else if (event.key.keysym.sym == SDLK_n) {  // Player presses 'N'
                    std::cout << "Exit canceled." << std::endl;
                    showExitBubble = false;  // Reset the bubble flag
                }
            }
            if (showSlotBubble) {
                if (event.key.keysym.sym == SDLK_y) {  // Player presses 'Y'
                    std::cout << "Opening Slot Machine Game..." << std::endl;
                    showSlotBubble = false;  // Reset the bubble flag
                    openSlotMachineWindow();  // Start the Slot Machine Game
                } else if (event.key.keysym.sym == SDLK_n) {  // Player presses 'N'
                    std::cout << "Readjusting player position..." << std::endl;
                    showSlotBubble = false;  // Reset the bubble flag
                    adjustPlayerPosition();  // Move player away from slot machine
                }
            } else if (showPokerBubble) {
                if (event.key.keysym.sym == SDLK_y) {  // Player presses 'Y'
                    std::cout << "Opening Poker Game..." << std::endl;
                    showPokerBubble = false;  // Reset the bubble flag
                    openBlackJackWindow();  // Start the Poker Game
                } else if (event.key.keysym.sym == SDLK_n) {  // Player presses 'N'
                    std::cout << "Readjusting player position..." << std::endl;
                    showPokerBubble = false;  // Reset the bubble flag
                    adjustPlayerPosition();  // Move player away from poker table
                }
            } else if (showBalanceBubble) {
                if (event.key.keysym.sym == SDLK_y) {  // Player presses 'Y'
                    std::cout << "Opening Balance Checking..." << std::endl;
                    showBalanceBubble = false;  // Reset the bubble flag
                    openBalanceAddWindow();  // Start the Poker Game
                } else if (event.key.keysym.sym == SDLK_n) {  // Player presses 'N'
                    std::cout << "Readjusting player position..." << std::endl;
                    showBalanceBubble = false;  // Reset the bubble flag
                    adjustPlayerPosition();  // Move player away from poker table
                }
            }
        break;

        default:
            break;
    }
}

void Game::openBalanceAddWindow() {
    try {
        // Close current window
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        std::unordered_map<int, Player> emptyMap;
        otherPlayers = emptyMap;

        // Create BalanceAddWindow instance
        BalanceAddWindow *balanceAddWindow = new BalanceAddWindow();  // Use a pointer to persist the object
        balanceAddWindow->setBalance(localPlayer.getBalance());

        if (balanceAddWindow->init("BalanceWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false)) {
            while (balanceAddWindow->isRunning()) {
                balanceAddWindow->handleEvents();
                balanceAddWindow->update();
                balanceAddWindow->render();
            }

            // Cleanup the balance add window
            localPlayer.setBalance(balanceAddWindow->getBalance());
            balanceAddWindow->close();
            // Reinitialize the original game window
            init("CasinoMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
            // Notify the server that player has rejoined the lobby
            GameMessage rejoinMsg;
            rejoinMsg.type = GameMessage::JOIN_LOBBY;
            rejoinMsg.player_id = localPlayer.getId();
            float xPos = player.getComponent<TransformComponent>().position.x;
            float yPos = player.getComponent<TransformComponent>().position.y;
            rejoinMsg.data = localPlayer.getName() + " " + std::to_string(localPlayer.getSkinId()) + " " + std::to_string(xPos) + " " + std::to_string(yPos);
            gameClient.send_message(rejoinMsg);
        }
    }
        catch (const std::exception& e) {
            std::cerr << "Initialization error: " << e.what() << std::endl;
            clean();
            isRunning = false;
        }
}


// Function to open the black jack game window
void Game::openBlackJackWindow() {
    try{
        std::unordered_map<int, Player> emptyMap;
        otherPlayers = emptyMap;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return;
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
        return;
    }
    // Close current window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);


    SDL_Window* window = SDL_CreateWindow("Blackjack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    blackjackGame game;
    Renderer render(renderer);

        std::string betText = LanguageManager::getInstance().getLocalizedResource("betText");
        if (betText.empty()) betText = "Bet: $";

        std::string balanceText = LanguageManager::getInstance().getLocalizedResource("balanceText");
        if (balanceText.empty()) balanceText = "Balance: $";

    // Load button textures from localized resources
    SDL_Texture* hitButtonTexture = render.LoadTexture(LanguageManager::getInstance().getLocalizedResource("hitButton").c_str());
    SDL_Texture* standButtonTexture = render.LoadTexture(LanguageManager::getInstance().getLocalizedResource("standButton").c_str());
    SDL_Texture* betButtonTexture = render.LoadTexture(LanguageManager::getInstance().getLocalizedResource("betButton").c_str());
    SDL_Texture* quitButtonTexture = render.LoadTexture(LanguageManager::getInstance().getLocalizedResource("quitButton").c_str());
    SDL_Texture* startButtonTexture = render.LoadTexture(LanguageManager::getInstance().getLocalizedResource("startButton").c_str());

    Button betButton(330, 510, 120, 80, betButtonTexture);
    Button standButton(260, 515, 140, 75, standButtonTexture);
    Button hitButton(400, 510, 120, 80, hitButtonTexture);
    Button quitButton(10, 10, 144, 70, quitButtonTexture);

    ChipOnTable chipsOnTable;

    SDL_Texture* chip5Texture = render.LoadTexture("assets/chipset/greenchip.png");
    SDL_Texture* chip10Texture = render.LoadTexture("assets/chipset/bluechip.png");
    SDL_Texture* chip50Texture = render.LoadTexture("assets/chipset/redchip.png");
    SDL_Texture* chip100Texture = render.LoadTexture("assets/chipset/blackchip.png");

    SDL_Texture* xButtonTexture = render.LoadTexture("assets/button_image/xButton.png");

    Button startButton(260, 200, 280, 144, startButtonTexture);

    Button xButton(10, 10, 70, 70, xButtonTexture);

    Chip chip5(550, 500, 50, 50, 5, chip5Texture);
    Chip chip10(610, 495, 50, 50, 10, chip10Texture);
    Chip chip50(670, 485, 50, 50, 50, chip50Texture);
    Chip chip100(730, 475, 50, 50, 100, chip100Texture);

    bool quit = false;
    SDL_Event e;
    bool inGame = false;
    int selectedBet = 0;
    bool showBetButton = true;
    bool showXButton = true;
    int initialBalance = localPlayer.getBalance();
    game.setBalance(initialBalance);
    bool secondDealerCardFaceUp = false;
    bool dealerTurnStarted = false;
    int currentBalance = game.GetBalance();
    SDL_Color whiteColor = {255, 255, 255, 255};
    bool playerBlackJack = false;
    bool showBettingChips = true;

     while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (!inGame) {
                    if (startButton.IsClicked(mouseX, mouseY)) {
                        Mix_PlayChannel(-1, clickSound, 0); // Play click sound
                        inGame = true;
                        game.StartNewGame();
                        selectedBet = 0;
                        secondDealerCardFaceUp = false;
                        dealerTurnStarted = false;
                    } else if (quitButton.IsClicked(mouseX, mouseY)) {
                        Mix_Chunk* clickSound2 = nullptr; // Declare a sound effect for chips
                        quit = true ;
                        SDL_DestroyWindow(window);
                        SDL_DestroyRenderer(renderer);
                        init("Casino Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
                        localPlayer.setBalance(game.GetBalance());
                        std::cout << localPlayer.getBalance();
                    }
                } else {
                    if(showXButton && xButton.IsClicked(mouseX, mouseY)) {
                        Mix_Chunk* clickSound2 = nullptr; // Declare a sound effect for chips
                        inGame = false;
                    }
                    if (chip5.IsClicked(mouseX, mouseY) && selectedBet + chip5.value <= currentBalance) {
                        selectedBet += chip5.value;
                        chipsOnTable.AddChip(chip5Texture);
                        Mix_PlayChannel(-1, chipSound, 0); // Play chip sound
                    }

                    if (chip10.IsClicked(mouseX, mouseY) && selectedBet + chip10.value <= currentBalance) {
                        selectedBet += chip10.value;
                        chipsOnTable.AddChip(chip10Texture);
                        Mix_PlayChannel(-1, chipSound, 0); // Play chip sound
                    }

                    if (chip50.IsClicked(mouseX, mouseY) && selectedBet + chip50.value <= currentBalance) {
                        selectedBet += chip50.value;
                        chipsOnTable.AddChip(chip50Texture);
                        Mix_PlayChannel(-1, chipSound, 0); // Play chip sound
                    }

                    if (chip100.IsClicked(mouseX, mouseY) && selectedBet + chip100.value <= currentBalance) {
                        selectedBet += chip100.value;
                        chipsOnTable.AddChip(chip100Texture);
                        Mix_PlayChannel(-1, chipSound, 0); // Play chip sound
                    }

                    if (showBetButton) {
                        if (betButton.IsClicked(mouseX, mouseY) && selectedBet > 0) {
                            if (selectedBet <= game.GetBalance()) {
                                Mix_PlayChannel(-1, clickSound, 0); // Play click sound
                                currentBalance -= selectedBet;
                                game.PlaceBet(selectedBet);
                                showBetButton = false;
                                showXButton = false;
                            }
                        }
                    }
                    else {
                        int playerHandValue = game.CalculateHandValue(game.GetPlayerHand());
                        int numPlayerCards = game.GetPlayerHand().size();

                        bool playerBlackjack = (playerHandValue == 21 && numPlayerCards == 2);
                        if(playerBlackjack) {
                            playerBlackJack = true;
                            game.PlayerStand();
                            dealerTurnStarted = true;
                        }
                        else if(playerHandValue <= 21) {
                            if (hitButton.IsClicked(mouseX, mouseY)) {
                                Mix_PlayChannel(-1, clickSound, 0); // Play chip sound
                                game.PlayerHit();
                                playerHandValue = game.CalculateHandValue(game.GetPlayerHand());
                                // Render the player's new card with flip sound
                                render.RenderCard(game.GetPlayerHand().back(), 330 + (30 * numPlayerCards), 375,true);

                                if (playerHandValue > 21) {
                                    game.ResolveBet();
                                    currentBalance = game.GetBalance();
                                    secondDealerCardFaceUp = true;
                                    dealerTurnStarted = true;
                                }
                            }
                            if (standButton.IsClicked(mouseX, mouseY)) {
                                Mix_PlayChannel(-1, clickSound, 0); // Play click sound
                                game.PlayerStand();
                                secondDealerCardFaceUp = true;
                                dealerTurnStarted = true;
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 254, 236, 200, 255);
        SDL_RenderClear(renderer);

        if (!inGame) {
            quitButton.Render(renderer);
            startButton.Render(renderer);
        } else {
            render.RenderTable();
            if(showXButton) {
                xButton.Render(renderer);
            }
            if(showBettingChips) {
                // Render chips
                chip5.Render(renderer);
                chip10.Render(renderer);
                chip50.Render(renderer);
                chip100.Render(renderer);
            }

            // Render  buttons
            if (showBetButton) {
                betButton.Render(renderer);
                xButton.Render(renderer);
                chipsOnTable.RenderChips(renderer);

            } else {
                render.RenderHand(game.GetPlayerHand(), 330, 375);
                render.RenderDealerHand(game.GetDealerHand(), 330, 150, secondDealerCardFaceUp);
                hitButton.Render(renderer);
                standButton.Render(renderer);
                chipsOnTable.RenderChips(renderer);
                showBettingChips = false;
            }

            int playerHandValue = game.CalculateHandValue(game.GetPlayerHand());
            int dealerHandValue = game.CalculateHandValue(game.GetDealerHand());
            int numPlayerCards = game.GetPlayerHand().size();
            int numDealerCards = game.GetDealerHand().size();

            // Check for player blackjack (21 with exactly 2 cards)
                // If dealer's turn has started, render dealer's new cards
                if (dealerTurnStarted) {
                    if (playerBlackJack) {
                        std::string balanceText = LanguageManager::getInstance().getLocalizedResource("balanceText");
                        if (balanceText.empty()) balanceText = "Balance: $";
                        std::string betText = LanguageManager::getInstance().getLocalizedResource("betText");
                        if (betText.empty()) betText = "Bet: $";
                        SDL_RenderPresent(renderer);
                        // Reveal the dealer's second card
                        secondDealerCardFaceUp = true;
                        render.RenderHand(game.GetPlayerHand(), 330, 375);
                        render.RenderDealerHand(game.GetDealerHand(), 330, 150, secondDealerCardFaceUp);
                        SDL_RenderPresent(renderer);

                        // Compare the player's blackjack to the dealer's hand
                        if (dealerHandValue == 21 && numDealerCards == 2) {
                            // Dealer has blackjack, it's a tie
                            std::string tieMessage = LanguageManager::getInstance().getLocalizedResource("tieMessage");
                            render.RenderText(tieMessage, 210, 280, {255, 255, 0, 255}, 30);
                            SDL_RenderPresent(renderer);
                        } else {
                            // Player wins with blackjack
                            std::string winblackjackMessage = LanguageManager::getInstance().getLocalizedResource("winblackjackMessage");
                            render.RenderText(winblackjackMessage, 240, 280, {0, 255, 0, 255}, 30);
                            chipsOnTable.RenderWinningChips(renderer);
                            SDL_RenderPresent(renderer);
                        }

                        game.ResolveBet();
                        currentBalance = game.GetBalance();
                        render.RenderText(balanceText + std::to_string(currentBalance), 10, 550, whiteColor, 24);
                        render.RenderText(betText + std::to_string(selectedBet), 10, 520, whiteColor, 24);
                        SDL_RenderPresent(renderer);
                        // Wait for the result to be displayed
                        SDL_Delay(2000); // Wait for 2 seconds to show the result

                        // Reset the game for the next round
                        playerBlackJack = false;
                        game.StartNewGame();
                        chipsOnTable.RemoveChips();
                        secondDealerCardFaceUp = false;
                        showBetButton = true;
                        showXButton = true;
                        selectedBet = 0;
                        dealerTurnStarted = false;
                        showBettingChips = true;

                    } else {
                    int playerHandValue = game.CalculateHandValue(game.GetPlayerHand());
                    int dealerHandValue = game.CalculateHandValue(game.GetDealerHand());

                    render.RenderDealerHand(game.GetDealerHand(), 330, 150, true);

                    if (playerHandValue > 21) {
                        render.RenderDealerHand(game.GetDealerHand(), 330, 150, secondDealerCardFaceUp);
                        std::string losebustMessage = LanguageManager::getInstance().getLocalizedResource("losebustMessage");
                        render.RenderText(losebustMessage, 300, 280, {255, 0, 0, 255}, 30);
                        chipsOnTable.RemoveChips();
                        chipsOnTable.RenderChips(renderer);
                        SDL_RenderPresent(renderer);
                        std::string balanceText = LanguageManager::getInstance().getLocalizedResource("balanceText");
                        render.RenderText(balanceText+std::to_string(currentBalance), 10, 550, {255, 255, 255, 255}, 24);
                        std::string betText = LanguageManager::getInstance().getLocalizedResource("betText");
                        render.RenderText(betText+std::to_string(selectedBet), 10, 520, {255, 255, 255, 255}, 24);
                        SDL_RenderPresent(renderer);


                        SDL_Delay(2000); // Wait for 2 seconds to show the result

                        game.StartNewGame();
                        chipsOnTable.RemoveChips();
                        secondDealerCardFaceUp = false;
                        dealerTurnStarted = false;
                        showBetButton = true;
                        showXButton = true;
                        showBettingChips = true;
                        selectedBet = 0;

                    } else {
                        std::string balanceText = LanguageManager::getInstance().getLocalizedResource("balanceText");
                        render.RenderText(balanceText+std::to_string(currentBalance), 10, 550, {255, 255, 255, 255}, 24);
                        std::string betText = LanguageManager::getInstance().getLocalizedResource("betText");
                        render.RenderText(betText+ std::to_string(selectedBet), 10, 520, {255, 255, 255, 255}, 24);
                        SDL_RenderPresent(renderer);
                        while (dealerHandValue < 17) {
                            game.DealerPlay();

                            int numDealerCards = game.GetDealerHand().size();
                            render.RenderDealerHand(game.GetDealerHand(), 330, 150, true);

                            std::vector<Card> dealersHand = game.GetDealerHand();
                            for (int i = 2; i < numDealerCards; ++i) {
                                render.RenderCard(dealersHand[i], 330 + (i * 30), 150,true);
                                SDL_Delay(500);
                                SDL_RenderPresent(renderer);
                            }
                            dealerHandValue = game.CalculateHandValue(game.GetDealerHand());
                        }

                        if (dealerHandValue > 21) {
                            std::string dealerbustMessage = LanguageManager::getInstance().getLocalizedResource("dealerbustMessage");
                            render.RenderText(dealerbustMessage, 290, 280, {0, 255, 0, 255}, 30);
                            chipsOnTable.RenderWinningChips(renderer);
                            SDL_RenderPresent(renderer);
                        } else if (playerHandValue > dealerHandValue) {
                            std::string winhigherscoreMessage = LanguageManager::getInstance().getLocalizedResource("winhigherscoreMessage");
                            render.RenderText(winhigherscoreMessage, 210, 280, {0, 255, 0, 255}, 30);
                            chipsOnTable.RenderWinningChips(renderer);
                            SDL_RenderPresent(renderer);
                        } else if (playerHandValue < dealerHandValue) {
                            std::string losedealerhigherMessage = LanguageManager::getInstance().getLocalizedResource("losedealerhigherMessage");
                            render.RenderText(losedealerhigherMessage, 210, 280, {255, 0, 0, 255}, 30);
                            chipsOnTable.RemoveChips();
                            chipsOnTable.RenderChips(renderer);
                            SDL_RenderPresent(renderer);
                        } else {
                            std::string tiebetpushesMessage = LanguageManager::getInstance().getLocalizedResource("tiebetpushesMessage");
                            render.RenderText(tiebetpushesMessage, 210, 280, {255, 255, 0, 255}, 30);                        }

                        game.ResolveBet();
                        currentBalance = game.GetBalance();

                        // Set flag to prevent any further actions until game resets

                        SDL_Delay(5000); // Wait for 2 seconds to show the result

                        render.RenderText(balanceText + std::to_string(currentBalance), 10, 550, whiteColor, 24);
                        render.RenderText(betText + std::to_string(selectedBet), 10, 520, whiteColor, 24);
                        SDL_RenderPresent(renderer);
                        chipsOnTable.RemoveChips();
                        game.StartNewGame();
                        secondDealerCardFaceUp = false;
                        dealerTurnStarted = false;
                        showBetButton = true;
                        showXButton = true;
                        showBettingChips = true;
                        selectedBet = 0;

                    }
                }
            }
        }
         std::string balanceText = LanguageManager::getInstance().getLocalizedResource("balanceText");
         render.RenderText(balanceText+std::to_string(currentBalance), 10, 550, {255, 255, 255, 255}, 24);
         std::string betText = LanguageManager::getInstance().getLocalizedResource("betText");
         render.RenderText(betText+ std::to_string(selectedBet), 10, 520, {255, 255, 255, 255}, 24);
         SDL_RenderPresent(renderer);
    }
    // After the poker game ends, we can reinitialize the main game window if needed
    // or return to the main game screen by recreating the original game window.
    init("CasinoMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
        // Notify the server that the player has rejoined the lobby
        GameMessage rejoinMsg;
        rejoinMsg.type = GameMessage::JOIN_LOBBY;
        rejoinMsg.player_id = localPlayer.getId();
        float xPos = player.getComponent<TransformComponent>().position.x;
        float yPos = player.getComponent<TransformComponent>().position.y;
        rejoinMsg.data = localPlayer.getName() + " " + std::to_string(localPlayer.getSkinId()) + " " + std::to_string(xPos) + " " + std::to_string(yPos);
        gameClient.send_message(rejoinMsg);
    }
    catch (const std::exception& e) {
        std::cerr << "Initialization error: " << e.what() << std::endl;
        clean();
        isRunning = false;
    }
}

// Function to open the slot machine game window
void Game::openSlotMachineWindow() {
    try{

        std::unordered_map<int, Player> emptyMap;
        otherPlayers = emptyMap;
    // Close current window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Create a new instance of SlotMachineGame and initialize it
    SlotMachineGame slotMachineGame;
    slotMachineGame.setBalance(localPlayer.getBalance());
    if (slotMachineGame.init("Slot Machine Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600)) {
        // Run the slot machine game
        while (slotMachineGame.running()) {
            slotMachineGame.handleEvents();
            slotMachineGame.update();
            slotMachineGame.render();
        }

        // Clean up the Slot Machine game resources
        SDL_DestroyWindow(window);
        localPlayer.setBalance(slotMachineGame.getBalance());
    }

    init("CasinoMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
        // Notify the server that the player has rejoined the lobby
        GameMessage rejoinMsg;
        rejoinMsg.type = GameMessage::JOIN_LOBBY;
        rejoinMsg.player_id = localPlayer.getId();
        float xPos = player.getComponent<TransformComponent>().position.x;
        float yPos = player.getComponent<TransformComponent>().position.y;
        rejoinMsg.data = localPlayer.getName() + " " + std::to_string(localPlayer.getSkinId()) + " " + std::to_string(xPos) + " " + std::to_string(yPos);
        gameClient.send_message(rejoinMsg);
    }
    catch (const std::exception& e) {
        std::cerr << "Initialization error: " << e.what() << std::endl;
        clean();
        isRunning = false;
    }
}

// Function to adjust the player’s position 10px from the slot machine
void Game::adjustPlayerPosition() {
    // Adjust the player’s position by 10px away from the slot machine
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, slotMachine1.getComponent<ColliderComponent>().collider)) {
        // Assuming slotMachine1 is at position (x, y)
        float slotX = slotMachine1.getComponent<TransformComponent>().position.x;
        float slotY = slotMachine1.getComponent<TransformComponent>().position.y;


        player.getComponent<TransformComponent>().position.x = slotX + 10;
        player.getComponent<TransformComponent>().position.y = slotY;
        player.getComponent<TransformComponent>().velocity.Zero();

        std::cout << "Player adjusted 10px away from slot machine!" << std::endl;
    }
}

void Game::update() {
    try{
    manager.refresh();
    manager.update();

    // Reset bubble visibility flags
    bool prevExitBubble = showExitBubble;
    bool prevSlotBubble = showSlotBubble;
    bool prevPokerBubble = showPokerBubble;
    bool prevBalanceBubble = showBalanceBubble;

    // Reset bubble visibility flags
    showPokerBubble = false;
    showSlotBubble = false;
    showBalanceBubble = false;

    //Handle collisions with bar table
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, barTable.getComponent<ColliderComponent>().collider)) {
        // Stop the player from moving
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement

        // Set bubble position near the poker table
        showBalanceBubble = true;
        bubbleDest.x = (int)barTable.getComponent<TransformComponent>().position.x + 50;
        bubbleDest.y = (int)barTable.getComponent<TransformComponent>().position.y - 60;
        bubbleDest.w = 200;
        bubbleDest.h = 80;

    }
    // Handle collisions with poker tables
    for (auto& pokerTable : pokertables) {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, pokerTable->getComponent<ColliderComponent>().collider)) {
            // Stop the player from moving
            player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
            player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
            player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement

            // Set bubble position near the poker table
            showPokerBubble = true;
            bubbleDest.x = (int)pokerTable->getComponent<TransformComponent>().position.x + 50;
            bubbleDest.y = (int)pokerTable->getComponent<TransformComponent>().position.y - 60;
            bubbleDest.w = 200;
            bubbleDest.h = 80;
            break; // Stop further checks as only one bubble is displayed at a time
        }
    }
    // Handle collisions with exit door
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, exitdoor.getComponent<ColliderComponent>().collider)) {
        // Stop the player from moving
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement

        // Set bubble position near the exit door
        showExitBubble = true;
        exitBubbleDest.x = (int)exitdoor.getComponent<TransformComponent>().position.x - 20;
        exitBubbleDest.y = (int)exitdoor.getComponent<TransformComponent>().position.y - 60;
        exitBubbleDest.w = 200;
        exitBubbleDest.h = 100;
    } else {
        // Ensure bubble disappears when no collision is detected
        showExitBubble = false;
    }
    // Handle collisions with slot machines
    for (auto& slotMachine : slotmachines) {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, slotMachine->getComponent<ColliderComponent>().collider)) {
            // Stop the player from moving
            player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
            player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
            player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement

            // Set bubble position near the slot machine
            showSlotBubble = true;
            bubbleDest.x = (int)slotMachine->getComponent<TransformComponent>().position.x + 20;
            bubbleDest.y = (int)slotMachine->getComponent<TransformComponent>().position.y - 60;
            bubbleDest.w = 200;
            bubbleDest.h = 100;
            break; // Stop further checks as only one bubble is displayed at a time
        }
    }
    // Play bubble sound only when bubble visibility changes to true
    if (showExitBubble && !prevExitBubble) {
        Mix_PlayChannel(-1, bubbleSound, 0);
    }
    if (showSlotBubble && !prevSlotBubble) {
        Mix_PlayChannel(-1, bubbleSound, 0);
    }
    if (showPokerBubble && !prevPokerBubble) {
        Mix_PlayChannel(-1, bubbleSound, 0);
    }
    if (showBalanceBubble && !prevBalanceBubble) {
        Mix_PlayChannel(-1, bubbleSound, 0);
    }

    // Wall collision checks (unchanged)
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wallLeft.getComponent<ColliderComponent>().collider)) {
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement
    }
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wallUp.getComponent<ColliderComponent>().collider)) {
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement
    }
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wallRight.getComponent<ColliderComponent>().collider)) {
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement
    }
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wallDownLeft.getComponent<ColliderComponent>().collider)) {
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement
    }
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wallDownRight.getComponent<ColliderComponent>().collider)) {
        player.getComponent<TransformComponent>().position.x -= player.getComponent<TransformComponent>().velocity.x;
        player.getComponent<TransformComponent>().position.y -= player.getComponent<TransformComponent>().velocity.y;
        player.getComponent<TransformComponent>().velocity.Zero(); // Stop movement
    }

    // Update all entities
    manager.update();

        // Process network messages
        while(gameClient.has_response()) {
            GameMessage msg = gameClient.receive_response();

            // Handle generic network messages
            handleNetworkMessage(msg);
        }


    } catch (const std::exception& e) {
        std::cerr << "Update error: " << e.what() << std::endl;
        isRunning = false; // Exit gracefully if necessary
    }
}

const char* Game::getSpritePath(int skinId) {
    switch (skinId) {
        case 0: return "assets/boychar1_anim.png";
        case 1: return "assets/boychar2_anim.png";
        case 2: return "assets/boychar3_anim.png";
        case 3: return "assets/girlchar1_anim.png";
        case 4: return "assets/girlchar2_anim.png";
        case 5: return "assets/girlchar3_anim.png";
        default: return "assets/default_char.png"; // Fallback
    }
}

void Game::handleNetworkMessage(const GameMessage& msg) {
    switch (msg.type) {
        case GameMessage::JOIN_LOBBY: {
            // Check if the player is already in the list
            if (otherPlayers.find(msg.player_id) != otherPlayers.end()) {
                std::cout << "Player with ID " << msg.player_id << " is already in the lobby!" << std::endl;
                return;
            }

            // Parse the player data
            std::istringstream iss(msg.data);
            std::string name;
            int skinId;
            float x, y;
            if (!(iss >> name >> skinId >> x >> y)) {
                std::cerr << "Failed to parse player data: " << msg.data << std::endl;
                return;
            }

            std::cout << "Received JOIN_LOBBY: " << msg.data << std::endl;
            std::cout << "Parsed as Name=" << name << ", SkinId=" << skinId << ", (x,y) = (" << x << "," << y << ")" << std::endl;

            // Add the new player to the list
            otherPlayers[msg.player_id] = Player(msg.player_id, name, skinId);

            std::cout << "Player added: ID=" << msg.player_id << ", Name=" << name << ", SkinId=" << skinId
                      << ", (x,y) = (" << x << "," << y << ")" << std::endl;

            // Create the player's entity
            Entity &newPlayer = manager.addEntity();
            newPlayer.addComponent<TransformComponent>(x,
                                                       y,
                                                       x,
                                                       y - 10,
                                                       32, 20, 10, 16, 2);
            const char *spritePath = getSpritePath(skinId);
            newPlayer.addComponent<SpriteComponent>(spritePath, true);
            newPlayer.getComponent<SpriteComponent>().Play("IdleUp");

            newPlayer.addComponent<ColliderComponent>("player" + std::to_string(msg.player_id));
            newPlayer.addGroup(groupPlayers);

            // Store the entity
            playerEntities[msg.player_id] = &newPlayer;

            std::cout << name << " joined the lobby!" << std::endl;

            break;
        }
        case GameMessage::PLAYER_MOVE:{
            // Parse the player data
            std::istringstream iss(msg.data);
            std::string name;
            int skinId;
            std::string direction;
            float x, y;
            if (!(iss >> name >> skinId >> direction >> x >> y)) {
                std::cerr << "Failed to parse player data: " << msg.data << std::endl;
                return;
            }

            std::cout << "Received PLAYER_MOVE: " << msg.data << std::endl;

            if(otherPlayers.find(msg.player_id) == otherPlayers.end()){
                otherPlayers[msg.player_id] = Player(msg.player_id, name, skinId);
                std::cout << "Player not found! Creating new Entity";
                // Create the player's entity
                Entity &newPlayer = manager.addEntity();
                newPlayer.addComponent<TransformComponent>(x,
                                                           y,
                                                           x,
                                                           y - 10,
                                                           32, 20, 10, 16, 2);
                const char *spritePath = getSpritePath(skinId);
                newPlayer.addComponent<SpriteComponent>(spritePath, true);
                newPlayer.getComponent<SpriteComponent>().Play("IdleUp");

                newPlayer.addComponent<ColliderComponent>("player" + std::to_string(msg.player_id));
                newPlayer.addGroup(groupPlayers);

                // Store the entity
                playerEntities[msg.player_id] = &newPlayer;
            }

            auto& pTransform = playerEntities[msg.player_id]->getComponent<TransformComponent>();
            auto& pSprite = playerEntities[msg.player_id]->getComponent<SpriteComponent>();
            auto& pCollider = playerEntities[msg.player_id]->getComponent<ColliderComponent>();

            if(direction == "WalkUp"){
                pTransform.velocity.y = -1;
                pSprite.Play("WalkUp");
            }
            else if(direction == "WalkLeft"){
                pTransform.velocity.x = -1;
                pSprite.Play("WalkLeft");
            }
            else if(direction == "WalkRight"){
                pTransform.velocity.x = 1;
                pSprite.Play("WalkRight");
            }
            else if(direction == "WalkDown"){
                pTransform.velocity.y = 1;
                pSprite.Play("WalkDown");
            }
            else if(direction == "IdleUp"){
                pTransform.velocity.y = 0;
                pSprite.Play("IdleUp");
            }
            else if(direction == "IdleLeft"){
                pTransform.velocity.x = 0;
                pSprite.Play("WalkUp");
            }
            else if(direction == "IdleRight"){
                pTransform.velocity.x = 0;
                pSprite.Play("IdleRight");
            }
            else if(direction == "IdleDown"){
                pTransform.velocity.y = 0;
                pSprite.Play("IdleDown");
            }

            pTransform.position.x = x;
            pTransform.position.y = y;
            break;
        }
        default:
            break;
    }

}


void Game::sendPlayerMovement(const std::string& direction){
    GameMessage msg;
    msg.type = GameMessage::PLAYER_MOVE;
    msg.player_id = localPlayer.getId();

    std::string name = localPlayer.getName();
    int skinId = localPlayer.getSkinId();
    float x, y;

    x = player.getComponent<TransformComponent>().position.x;
    y = player.getComponent<TransformComponent>().position.y;

    msg.data = name + " " + std::to_string(skinId) + " " + direction + " " + std::to_string(x) + " " + std::to_string(y);

    // Send the message to the server via GameClient
    gameClient.send_message(msg);
}
auto& tiles(manager.getGroup(groupMap));
auto& walls(manager.getGroup(groupWall));
auto& players(manager.getGroup(groupPlayers));
auto& chairs(manager.getGroup(groupChairs));


void Game::render() {
    SDL_RenderClear(renderer);
    map->DrawMap();

    for(auto& w : walls){
        w->draw();
    }
    barTable.draw();
    christmastree.draw();
    for(auto& pt: pokertables){
        pt->draw();
    }
    for(auto& sm: slotmachines){
        sm->draw();
    }
    for(auto& c: chairs){
        c->draw();
    }
    for(auto& p: players){
        p->draw();
    }

    if (showPokerBubble) {  // Render bubble if necessary
        SDL_RenderCopy(renderer, pokerBubbleTexture, nullptr, &bubbleDest);
    }
    if (showSlotBubble) {  // Render the slots bubble if necessary
        SDL_RenderCopy(renderer, slotBubbleTexture, nullptr, &bubbleDest);
    }
    if (showExitBubble) {  // Render exit confirmation bubble if necessary
        SDL_RenderCopy(renderer, exitBubbleTexture, nullptr, &exitBubbleDest);
    }
    if (showBalanceBubble) {
        SDL_RenderCopy(renderer, balanceBubbleTexture, nullptr, &bubbleDest);
    }
    // Render the guide image in the bottom-right corner
    SDL_Rect guideRect = {
        800 - 120, // Window width minus guide width
        640 - 70, // Window height minus guide height
        120,
        70
    };
    SDL_RenderCopy(renderer, guideTexture, nullptr, &guideRect);

    // Retrieve localized balance text
    std::string balanceText = LanguageManager::getInstance().getLocalizedResource("balanceText");
    int currentBalance = localPlayer.getBalance();

    // Render the balance text
    renderText(balanceText + std::to_string(currentBalance), 40, 570, {255, 255, 255});

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyTexture(pokerBubbleTexture); // Cleanup textures
    SDL_DestroyTexture(slotBubbleTexture);
    SDL_DestroyTexture(slotBubbleTexture);
    SDL_DestroyTexture(guideTexture);
    SDL_DestroyTexture(balanceBubbleTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    if (chipSound) {
        Mix_FreeChunk(chipSound);
        chipSound = nullptr;
    }
    Mix_CloseAudio();
    std::cout << "Game cleaned" << std::endl;
}

void Game::AddTile(int id, int x, int y){
    auto& tile(manager.addEntity());
    tile.addGroup(groupMap);
}