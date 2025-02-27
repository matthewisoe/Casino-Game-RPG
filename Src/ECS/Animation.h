//
// Created by Matt Irv on 28/11/24.
//

#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H
struct Animation{
    int index;
    int frames;
    int speed;

    Animation(){}
    Animation(int i, int f, int s){
        index = i;
        frames = f;
        speed = s;
    }
};
#endif //GAME_ANIMATION_H
