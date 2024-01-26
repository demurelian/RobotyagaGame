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

class mentalEnemy {
    short damage = 1;
    const char *enemyTexture_p = "textures/npc/mentalEnemy.png";
protected:
    enum animationCode {idle, walkR, walkL};
public:

    const unsigned short    EnemyW = 92,
                            EnemyH = 92,
                            EnemyFrameW = 48,
                            EnemyFrameH = 64,
                            barH = 5;

    SDL_Texture *texture;
    SDL_Rect dstRect, srcRect, livesRect;
    short lives;
    float dx, dy, frameOffsetX, playerBombRadius;
    int k,m;//pos on tilemap
    bool onGround;
    Center center;

    Uint32 animationTimer, livesTimer, damageTimer;

    void initialization(SDL_Renderer *ren, int x, int y);
    void update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, const unsigned short frameDelay, SDL_DisplayMode displayMode);
    bool collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player);
    void collisionWithPlayer(Player &Player, SDL_DisplayMode displayMode, const unsigned short frameDelay);
    void collisionWithPlayerAttacks(Player &player, Arrow *arrow);
    void collisionWithPlayerAttacks(Player &player, Bomb *bomb);
    void collisionWithPlayerAttacks(Player &player, Boomerang *boomerang);
    void updateAnim(animationCode code);
    mentalEnemy& operator=(const mentalEnemy &obj);
};