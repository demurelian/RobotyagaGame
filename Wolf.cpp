#include "Wolf.h"

Wolf::Wolf(SDL_Renderer *ren, Player player, SDL_DisplayMode displayMode, bool direction_) {
    direction = direction_;
    texture = IMG_LoadTexture(ren, wolfTexture_p);
    dstRect.w = WolfW;
    dstRect.h = WolfH;
    if (direction) {
        dstRect.x = -WolfW;
    } else {
        dstRect.x = displayMode.w;
    }
    dstRect.y = player.dstRect.y - (WolfH - player.dstRect.h);

    srcRect.x = srcRect.y = 0;
    srcRect.w = WolfW;
    srcRect.h = WolfH;

    dx = dy = 0;

    lives = 450;

    animationTimer = SDL_GetTicks();
}

void Wolf::updateAnim() {
    srcRect.x += srcRect.w;
    if (srcRect.x >= 600) {
        srcRect.y += srcRect.h;
        srcRect.x = 0;
    }
    if (srcRect.y >= 3 * srcRect.h && srcRect.x >= 2 * srcRect.h) {
        srcRect.x = srcRect.y = 0;
    }
}

void Wolf::update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, SDL_DisplayMode displayMode) {
    collisionWithPlayer(player);
    if (SDL_GetTicks() - animationTimer > 32) {
        animationTimer = SDL_GetTicks();
        updateAnim();
    }
    if (!onGround) {
        if (dy < 50)
        dy++;
    } else {
        dy = 0;
    }
    if (player.mapOffsetY > player.minOffsetY && player.mapOffsetY < player.maxOffsetY) {
        dstRect.y -= player.dy;
    }
    if (player.mapOffsetX > player.minOffsetX && player.mapOffsetX < player.maxOffsetX) {
        dstRect.x -= player.dx;
    }
    dstRect.y += dy;
    if (collisionWithMap(b, map, tileSide, player)) {
        onGround = true;
    } else {
        onGround = false;
    }
    if (direction) {
        dx = 15;
        dstRect.x += dx;
        if (dstRect.x < displayMode.w - dstRect.w)
            collisionWithMap(r, map, tileSide, player);  
        SDL_RenderCopyEx(ren, texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    } else {
        dx = -15;
        dstRect.x += dx;
        if (dstRect.x > 0)
            collisionWithMap(l, map, tileSide, player);  
        SDL_RenderCopyEx(ren, texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
    }
    
    center.x = dstRect.x + dstRect.w/2;
    center.y = dstRect.y + dstRect.h/2;
}

bool Wolf::collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player) {
    int i, j;
    switch (code) {
    case r:
        j = (dstRect.x + dstRect.w + player.mapOffsetX) / tileSide;
        for (i = (dstRect.y + player.mapOffsetY) / tileSide; i < (dstRect.y + player.mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                if (map[i][j] != 'R')
                    map[i][j] = 0;
                return true;
            }
        }
        break;
    case l:
        j = (dstRect.x + player.mapOffsetX) / tileSide;
        for (i = (dstRect.y + player.mapOffsetY) / tileSide; i < (dstRect.y + player.mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                if (map[i][j] != 'R') {
                    map[i][j] = 0;
                }
                return true;
            }
        
        }
        break;
    case b:
        i = (dstRect.y + player.mapOffsetY + dstRect.h) / tileSide;
        for (j = (dstRect.x + player.mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + player.mapOffsetX) / (float)tileSide; j++) {
            if (map[i][j]) {
                if (map[i][j] == 'H') { map[i][j] = 0; } else {
                //dstRect.y = i * tileSide - dstRect.h - player.mapOffsetY;
                    dstRect.y -= dy;
                    dy=0;
                    //onGround = true;
                    return true; 
                }
            } 
        }
    }
    return false;
}

void Wolf::collisionWithPlayer(Player &player) {
    if (player.dstRect.x > dstRect.x - player.dstRect.w && player.dstRect.x < dstRect.x + dstRect.w && player.dstRect.y > dstRect.y - player.dstRect.h && player.dstRect.y < dstRect.y + dstRect.h) {
        player.lives -= 100;
    }
}

void Wolf::collisionWithBomb(Player &player, Bomb *bomb) {
    playerBombRadius = sqrt((center.x - bomb->center.x) * (center.x - bomb->center.x) + (center.y - bomb->center.y) * (center.y - bomb->center.y));
    bomb->damage = round(bomb->maxRadius - playerBombRadius);
    if (bomb->damage < 0) {
        bomb->damage = 0;
    }
    lives -= bomb->damage;
    if (lives < 0) 
        lives = 0;
}