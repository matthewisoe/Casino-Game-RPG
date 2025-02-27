
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "Game.h"

class Map {
public:
    Map();
    ~Map();

    void LoadMap(int arr[20][25]);
    void DrawMap();

private:
    SDL_Rect src, dest;
    SDL_Texture* redtile1;
    SDL_Texture* redtile2;
    SDL_Texture* bluetile1;
    SDL_Texture *bluetile2;
    SDL_Texture* upleftwall;
    SDL_Texture* upwall;
    SDL_Texture *uprightwall;
    SDL_Texture *rightwall;
    SDL_Texture *downrightwall;
    SDL_Texture *downwall;
    SDL_Texture *downleftwall;
    SDL_Texture *leftwall;
    SDL_Texture *leftcornerwall;
    SDL_Texture *rightcornerwall;
    SDL_Texture *wall;
    SDL_Texture *bottomwall;

    int map[20][25];
};


#endif //GAME_MAP_H
