#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>           
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
using namespace std;


enum collisionCode {t, r, b, l};

struct Center {
    int x;
    int y;
};

class Player {
    enum playerAnimations {stay, walk, run, jump};
    enum attackCode {boom, drone, boomerang, arrow};
public:
    const unsigned  PlayerW = 80,
                    PlayerH = 85,
                    PlayerFrameW = 64,
                    PlayerFrameH = 68,
                    spriteError = 7,
                    barsDistance = 10,
                    numberOfAttacksTextures = 5,
                    startingOffsetX = 48,
                    startingOffsetY = 1109,
                    max_lives_and_stamina = 500;
    const char  *playerTexture_p = "textures/2player.png",
                *pointsTexture_p = "textures/Tiles/platformPack_tile029.png";


    // SDL_Texture *boomTextures[numberOfAttacksTextures];
    // SDL_Texture *droneTextures[numberOfAttacksTextures];
    // SDL_Texture *boomerangTextures[numberOfAttacksTextures];
    // SDL_Texture *arrowTextures[numberOfAttacksTextures];

    SDL_Texture *texture;
    SDL_Rect    srcRect, dstRect,
                livesRect, staminaRect,
                livesRectBg, staminaRectBg;
    struct attack {
        SDL_Rect rect;
        SDL_Texture *texture;
        int damage;
    };

    bool    yellowKey,
            blueKey,
            greenKey,
            arrowEarned,
            bombEarned,
            boomerangEarned,
            onLadder,
            dead,
            shopPossible;

    bool onGround, singleShot = true;
    short   frameYOffset, points,
            nBombs, nArrows;
    Uint8   direction;//1 = right, 0 = left
    short lives, stamina, maxLives, maxStamina, staminaRegeneration, livesRegeneration;
    float   dy,dx,//vertical and horizontal speed
            mapOffsetX, mapOffsetY,
            minOffsetX, minOffsetY,
            maxOffsetX, maxOffsetY;
    Uint32 animationTimer, staminaIncreaseTimer, staminaDecreaseTimer, livesRegenTimer;
    
    void initialization(SDL_Renderer *ren, SDL_DisplayMode displayMode, unsigned short mapRaws, unsigned short mapColumns, unsigned short tileSide);
    void update(SDL_Renderer * ren, SDL_DisplayMode displayMode, const Uint8 *keyboardState, const unsigned frameDelay, Uint8 *map[], const unsigned tileSide, const unsigned mapRaws, const unsigned mapColumns);
    void updateAnim(playerAnimations animationCode);
    bool Collision(collisionCode code, Uint8 *map[], const unsigned tileSide, const Uint8 *keyboardState);//true = player on the ground
    // void loadAttack(SDL_Renderer *ren);
    // void attack(attackCode code);
};