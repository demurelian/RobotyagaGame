#include "Boomerang.h"

Boomerang::Boomerang(SDL_Renderer *ren, Player player, Uint8 direction_) {
    timer = SDL_GetTicks();
    direction = direction_;
    textures[0] = IMG_LoadTexture(ren, boomerangTexture1_p);
    textures[1] = IMG_LoadTexture(ren, boomerangTexture2_p);
    textures[2] = IMG_LoadTexture(ren, boomerangTexture3_p);
    textures[3] = IMG_LoadTexture(ren, boomerangTexture4_p);
    textures[4] = IMG_LoadTexture(ren, boomerangTexture5_p);
    texture = textures[4];
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
    angle = 0;
    directionChanged = false;
    returned = false;
}

void Boomerang::update(SDL_Renderer *ren, Player player) {
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
    if (!directionChanged && SDL_GetTicks() - timer > 1000) {
        direction = !direction;
        directionChanged = true;
    }
    if (player.mapOffsetY > player.minOffsetY && player.mapOffsetY < player.maxOffsetY) {
        rect.y -= player.dy;
    }
    if (player.mapOffsetX > player.minOffsetX && player.mapOffsetX < player.maxOffsetX) {
        rect.x -= player.dx;
    }
    if (directionChanged && rect.x > player.dstRect.x - rect.w && rect.x < player.dstRect.x + player.dstRect.w && rect.y > player.dstRect.y - rect.h && rect.y < player.dstRect.y + player.dstRect.h) {
        returned = true;
    }
    angle += 20;
    if (direction) {
        rect.x += speed;
        SDL_RenderCopyEx(ren, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    } else {
        rect.x -= speed;
        SDL_RenderCopyEx(ren, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_HORIZONTAL);
    }
}