#include "Arrow.h"

Arrow::Arrow(SDL_Renderer *ren, Player player, Uint8 direction_) {
    timer = SDL_GetTicks();
    direction = direction_;
    textures[0] = IMG_LoadTexture(ren, arrowTexture1_p);
    textures[1] = IMG_LoadTexture(ren, arrowTexture2_p);
    textures[2] = IMG_LoadTexture(ren, arrowTexture3_p);
    textures[3] = IMG_LoadTexture(ren, arrowTexture4_p);
    textures[4] = IMG_LoadTexture(ren, arrowTexture5_p);
    texture = textures[0];
    rect.w = player.PlayerW;
    rect.h = player.PlayerH;
    rect.y = player.dstRect.y;
    if (direction) {
        rect.x = player.dstRect.x + player.PlayerW;
    } else {
        rect.x = player.dstRect.x - rect.w;
    }
    speed = 20;
    damage = 5;
}

void Arrow::update(SDL_Renderer *ren, Player player) {
    if (SDL_GetTicks() - timer > 50) {
        texture = textures[0];
        damage = 5;
    }
    if (SDL_GetTicks() - timer > 100) {
        texture = textures[1];
        damage = 6;
    }
    if (SDL_GetTicks() - timer > 200) {
        texture = textures[2];
        damage = 7;
    }
    if (SDL_GetTicks() - timer > 400) {
        texture = textures[3];
        damage = 8;
    }
    if (SDL_GetTicks() - timer > 800) {
        texture = textures[4];
        damage = 9;
    }
     if (player.mapOffsetY > player.minOffsetY && player.mapOffsetY < player.maxOffsetY) {
        rect.y -= player.dy;
    }
    if (player.mapOffsetX > player.minOffsetX && player.mapOffsetX < player.maxOffsetX) {
        rect.x -= player.dx;
    }
    if (direction) {
        rect.x += speed;
        SDL_RenderCopy(ren, texture, nullptr, &rect);
    } else {
        rect.x -= speed;
        SDL_RenderCopyEx(ren, texture, nullptr, &rect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}