#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>      
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
using namespace std;

class Bomb {
private:
    const char  *bombTexture1_p = "textures/effects/05/Water__01.png",
                *bombTexture2_p = "textures/effects/05/Water__02.png",
                *bombTexture3_p = "textures/effects/05/Water__03.png",
                *bombTexture4_p = "textures/effects/05/Water__04.png",
                *bombTexture5_p = "textures/effects/05/Water__05.png";
public:
    Center center;
    SDL_Rect rect;
    SDL_Texture *texture, *textures[5];
    int maxRadius, damage, angle;
    Uint8 direction;
    Uint32 timer;
    Bomb(SDL_Renderer *ren, Player player, Uint8 direction_);//lvl = bomb lvl

    void update(SDL_Renderer *ren, Player player);
    void collision();
};