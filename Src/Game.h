
#ifndef POKERGAME_GAME_H
#define POKERGAME_GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Player.h"
#include <SDL_ttf.h>
#include "GameClient.h"
#include <unordered_map>
#include "ECS/ECS.h"

class ColliderComponent;
class TransformComponent;

class Game {
public:
    Game(Player player, GameClient& client);
    ~Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void openBlackJackWindow();
    void openSlotMachineWindow();
    void openBalanceAddWindow();
    void adjustPlayerPosition();
    void handleEvents();
    void update();
    void render();
    void clean();
    static void AddTile(int id, int x, int y);
    bool running()const { return isRunning; }

    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent*> colliders;


    const char* getSpritePath(int skinId);
    void handleNetworkMessage(const GameMessage& msg);
    void sendPlayerMovement(const std::string& direction);
    void handleCollision(TransformComponent& transform, ColliderComponent& collider, ColliderComponent& otherCollider);

private:
    bool isRunning = false;  // Whether the game loop is running
    int cnt = 0;             // Counter for custom purposes
    SDL_Window* window = nullptr; // SDL window object

    Player localPlayer;
    GameClient& gameClient;

    std::unordered_map<int, Player> otherPlayers;
    std::unordered_map<int, Entity*> playerEntities;

    TTF_Font* font = nullptr;

    void renderText(const std::string &message, int x, int y, SDL_Color color);
};


#endif // POKERGAME_GAME_H
