#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>          
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
using namespace std;

class Boomerang {
private:
    const char  *boomerangTexture1_p = "textures/effects/04/Water__01.png",
                *boomerangTexture2_p = "textures/effects/04/Water__02.png",
                *boomerangTexture3_p = "textures/effects/04/Water__03.png",
                *boomerangTexture4_p = "textures/effects/04/Water__04.png",
                *boomerangTexture5_p = "textures/effects/04/Water__05.png";
public:
    SDL_Rect rect;
    SDL_Texture *texture, *textures[5];
    int damage, speed;
    float angle;
    Uint8 direction;
    Uint32 timer;
    bool directionChanged, returned;
    Boomerang(SDL_Renderer *ren, Player player, Uint8 direction_);//lvl = boomerang lvl

    void update(SDL_Renderer *ren, Player player);
    void collision();
};