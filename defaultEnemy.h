#pragma once
#include <iostream>
#include <SDL2/SDL.h>           
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include "Arrow.h"
#include "Bomb.h"
#include "Boomerang.h"

class defaultEnemy {
    enum animationCode {idle, walkR, walkL};
public:
    const char *enemyTexture_p = "textures/npc/defaultEnemy.png";

    const unsigned short    EnemyW = 80,
                            EnemyH = 85,
                            EnemyFrameW = 48,
                            EnemyFrameH = 64,
                            barH = 5,
                            stepCase1 = 6,
                            stepCase2 = 2;

    SDL_Texture *texture;
    SDL_Rect dstRect, srcRect, livesRect;
    short lives;
    float dx, dy, frameOffsetX, playerBombRadius;
    bool onGround;
    Center center;

    Uint32 animationTimer, livesTimer;

    void initialization(SDL_Renderer *ren, int x, int y);
    void update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, const unsigned short frameDelay, SDL_DisplayMode displayMode);
    bool collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player);
    void collisionWithPlayer(Player &Player, const unsigned short frameDelay);
    void collisionWithPlayerAttacks(Player &player, Arrow *arrow);
    void collisionWithPlayerAttacks(Player &player, Bomb *bomb);
    void collisionWithPlayerAttacks(Player &player, Boomerang *boomerang);
    void updateAnim(animationCode code);
    defaultEnemy& operator=(const defaultEnemy &obj);
};