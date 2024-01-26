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

class boomEnemy {
    enum animationCode {idle, walkR, walkL};
public:
    char  enemyTexture_p[900] = "textures/npc/boomEnemy.png";
    char boomTexture_p[900] = "textures/npc/Explosion.png";

    const unsigned short    EnemyW = 80,
                            EnemyH = 85,
                            EnemyFrameW = 48,
                            EnemyFrameH = 64,
                            barH = 5,
                            ExplosiveW = 256,
                            ExplosiveH = 256;

    SDL_Texture *texture, *boomTexture;
    SDL_Rect dstRect, srcRect, livesRect;
    short lives;
    float dx, dy, frameOffsetX, damage, maxDamageRadius, boomRadius, playerBombRadius;
    bool onGround, boomIsComing, boomHappend, damageGiven;

    Center boomCenter, playerCenter;
    
    Uint32 animationTimer, livesTimer, boomTimer;

    void initialization(SDL_Renderer *ren, int x, int y);
    void update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, const unsigned short frameDelay, SDL_DisplayMode displayMode);
    bool collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player);
    void collisionWithPlayer(Player &Player);
    void collisionWithPlayerAttacks(Player &player, Arrow *arrow);
    void collisionWithPlayerAttacks(Player &player, Bomb *bomb);
    void collisionWithPlayerAttacks(Player &player, Boomerang *boomerang);
    void updateAnim(animationCode code);
    void boom(const unsigned short frameDelay);
    boomEnemy& operator=(const boomEnemy &obj);
};