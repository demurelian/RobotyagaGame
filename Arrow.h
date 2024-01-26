#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
using namespace std;

class Arrow {
private:
    const char  *arrowTexture1_p = "textures/effects/01/Water__01.png",
                *arrowTexture2_p = "textures/effects/01/Water__02.png",
                *arrowTexture3_p = "textures/effects/01/Water__03.png",
                *arrowTexture4_p = "textures/effects/01/Water__04.png",
                *arrowTexture5_p = "textures/effects/01/Water__05.png";
public:
    SDL_Rect rect;
    SDL_Texture *texture, *textures[5];
    int damage, speed;
    Uint8 direction;
    Uint32 timer;
    Arrow(SDL_Renderer *ren, Player player, Uint8 direction_);//lvl = arrow lvl

    void update(SDL_Renderer *ren, Player player);
    void collision();
};