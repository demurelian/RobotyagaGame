#pragma once
#include <iostream>
#include <SDL2/SDL.h>         
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include "Bomb.h"

class Wolf {
public:
    const char *wolfTexture_p = "textures/Wolf.png";
    const unsigned short    WolfW = 200,
                            WolfH = 144;

    SDL_Texture *texture;
    SDL_Rect dstRect, srcRect;
    short lives;
    float dx, dy, playerBombRadius;
    bool onGround, direction;
    Center center; 

    Uint32 animationTimer;

    Wolf(SDL_Renderer *ren, Player player, SDL_DisplayMode displayMode, bool direction_);
    void update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, SDL_DisplayMode displayMode);
    void updateAnim();
    bool collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player);
    void collisionWithPlayer(Player &Player);
    void collisionWithBomb(Player &player, Bomb *bomb);
};