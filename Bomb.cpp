#include "Bomb.h"

Bomb::Bomb(SDL_Renderer *ren, Player player, Uint8 direction_) {
    timer = SDL_GetTicks();
    direction = direction_;
    textures[0] = IMG_LoadTexture(ren, bombTexture1_p);
    textures[1] = IMG_LoadTexture(ren, bombTexture2_p);
    textures[2] = IMG_LoadTexture(ren, bombTexture3_p);
    textures[3] = IMG_LoadTexture(ren, bombTexture4_p);
    textures[4] = IMG_LoadTexture(ren, bombTexture5_p);
    texture = textures[0];
    rect.w = player.PlayerW;
    rect.h = player.PlayerH;
    rect.y = player.dstRect.y;
    if (direction) {
        rect.x = player.dstRect.x;// + player.PlayerW;
    } else {
        rect.x = player.dstRect.x;// - rect.w;
    }
    maxRadius = 500;
    angle = 0;
}

void Bomb::update(SDL_Renderer *ren, Player player) {
    center.x = rect.x + rect.w/2;
    center.y = rect.y + rect.h/2;
    if (SDL_GetTicks() - timer > 400) {
        texture = textures[1];
    }
    if (SDL_GetTicks() - timer > 800) {
        texture = textures[2];
    }
    if (SDL_GetTicks() - timer > 1200) {
        texture = textures[3];
    }
    if (SDL_GetTicks() - timer > 1600) {
        texture = textures[4];
    }
    if (player.mapOffsetY > player.minOffsetY && player.mapOffsetY < player.maxOffsetY) {
        rect.y -= player.dy;
    }
    if (player.mapOffsetX > player.minOffsetX && player.mapOffsetX < player.maxOffsetX) {
        rect.x -= player.dx;
    }
    angle++;
    if (direction) {
        SDL_RenderCopyEx(ren, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyEx(ren, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_HORIZONTAL);
    }
}