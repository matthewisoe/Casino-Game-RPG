
#include "Map.h"
#include "Game.h"
#include <fstream>
#include "TextureManager.h"

int lvl1[20][25] = {
        {2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4},
        {9,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,5},
        {9,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,5},
        {9,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {9,0,0,0,0,0,0,0,0,0,0,14,14,14,0,0,0,0,0,0,0,0,0,0,5},
        {9,1,1,1,1,1,1,1,1,1,1,15,15,15,1,1,1,1,1,1,1,1,1,1,5},
        {8,7,7,7,7,7,7,7,7,7,10,14,14,14,11,7,7,7,7,7,7,7,7,7,6}
};

Map::Map() {
    redtile1 = TextureManager::LoadTexture("assets/red_tile1.png");
    redtile2 = TextureManager::LoadTexture("assets/red_tile2.png");
    bluetile1 = TextureManager::LoadTexture("assets/blue_tile1.png");
    bluetile2 = TextureManager::LoadTexture("assets/blue_tile2.png");
    upleftwall = TextureManager::LoadTexture("assets/up_left_wall.png");
    upwall = TextureManager::LoadTexture("assets/up_wall.png");
    uprightwall = TextureManager::LoadTexture("assets/up_right_wall.png");
    rightwall = TextureManager::LoadTexture("assets/right_wall.png");
    downrightwall = TextureManager::LoadTexture("assets/down_right_wall.png");
    downwall = TextureManager::LoadTexture("assets/down_wall.png");
    downleftwall = TextureManager::LoadTexture("assets/down_left_wall.png");
    leftwall = TextureManager::LoadTexture("assets/left_wall.png");
    leftcornerwall = TextureManager::LoadTexture("assets/left_corner_wall.png");
    rightcornerwall = TextureManager::LoadTexture("assets/right_corner_wall.png");
    wall = TextureManager::LoadTexture("assets/wall.png");
    bottomwall = TextureManager::LoadTexture("assets/wall_bottom.png");
    LoadMap(lvl1);
    src.x = src.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
    dest.x = dest.y = 0;
}

Map::~Map(){
}

void Map::LoadMap(int arr[20][25]) {
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 25; ++j) {
            map[i][j] = arr[i][j];
        }
    }
}

void Map::DrawMap() {
    int type = 0;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 25; ++j) {
            type = map[i][j];

            dest.x = j * 32;
            dest.y = i * 32;

            switch (type) {
                case 0:
                    TextureManager::Draw(redtile1, src, dest);
                    break;
                case 1:
                    TextureManager::Draw(redtile2, src, dest);
                    break;
                case 2:
                    TextureManager::Draw(upleftwall, src, dest);
                    break;
                case 3:
                    TextureManager::Draw(upwall, src, dest);
                    break;
                case 4:
                    TextureManager::Draw(uprightwall, src, dest);
                    break;
                case 5:
                    TextureManager::Draw(rightwall, src, dest);
                    break;
                case 6:
                    TextureManager::Draw(downrightwall, src, dest);
                    break;
                case 7:
                    TextureManager::Draw(downwall, src, dest);
                    break;
                case 8:
                    TextureManager::Draw(downleftwall, src, dest);
                    break;
                case 9:
                    TextureManager::Draw(leftwall, src, dest);
                    break;
                case 10:
                    TextureManager::Draw(leftcornerwall, src, dest);
                    break;
                case 11:
                    TextureManager::Draw(rightcornerwall, src, dest);
                    break;
                case 12:
                    TextureManager::Draw(wall, src, dest);
                    break;
                case 13:
                    TextureManager::Draw(bottomwall, src, dest);
                    break;
                case 15:
                    TextureManager::Draw(bluetile1, src, dest);
                    break;
                case 14:
                    TextureManager::Draw(bluetile2, src, dest);
                    break;
                default:
                    break;
            }
        }
    }
}
