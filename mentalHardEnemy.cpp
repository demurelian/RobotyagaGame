#include "mentalHardEnemy.h"

void mentalHardEnemy::initialization(SDL_Renderer *ren, int x, int y) {
    lives = 70;

    texture = IMG_LoadTexture(ren, enemyTexture_p);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = EnemyW;
    dstRect.h = EnemyH;
    
    srcRect.x = EnemyFrameW;
    srcRect.y = EnemyFrameH * 2;
    srcRect.w = EnemyFrameW;
    srcRect.h = EnemyFrameH;

    frameOffsetX = EnemyFrameW;

    livesRect.h = barH;

    damage = 3;

    animationTimer = livesTimer = damageTimer = SDL_GetTicks();
}

void mentalHardEnemy::update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, const unsigned short frameDelay, SDL_DisplayMode displayMode) {
    collisionWithPlayer(player, displayMode, frameDelay);

    if (SDL_GetTicks() - animationTimer > 13 * frameDelay) {
        animationTimer = SDL_GetTicks();
        updateAnim(idle);
    }

    center.x = dstRect.x + dstRect.w/2;
    center.y = dstRect.y + dstRect.h/2;

    livesRect.w = lives;
    livesRect.x = dstRect.x + (dstRect.w - lives) / 2;
    livesRect.y = dstRect.y - barH;
    
    SDL_SetRenderDrawColor(ren, 93,80,108,255);
    SDL_RenderDrawRect(ren, &livesRect);
    SDL_RenderFillRect(ren, &livesRect);

    SDL_RenderCopy(ren, texture, &srcRect, &dstRect);
    
}

bool mentalHardEnemy::collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player) {
    int i, j;
    switch (code) {
    case t:
        i = (dstRect.y + player.mapOffsetY) / tileSide;
        for (j = (dstRect.x + player.mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + player.mapOffsetX) / (float)tileSide; j++)
        {
            if (map[i][j]) {
                dstRect.y = (i + 1) * tileSide - player.mapOffsetY;
                //dstRect.y -= dy;
                dy = 0;
            }
        }
        break;
    case r:
        j = (dstRect.x + dstRect.w + player.mapOffsetX) / tileSide;
        for (i = (dstRect.y + player.mapOffsetY) / tileSide; i < (dstRect.y + player.mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                dstRect.x = j * tileSide - dstRect.w - player.mapOffsetX;
                //dstRect.x -= dx;
                dx = 0;
            }
        }
        break;
    case b:
        i = (dstRect.y + player.mapOffsetY + dstRect.h) / tileSide;
        for (j = (dstRect.x + player.mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + player.mapOffsetX) / (float)tileSide; j++) {
            if (map[i][j]) {
                dstRect.y = i * tileSide - dstRect.h - player.mapOffsetY;
                //dstRect.y -= dy;
                dy = 0;
                onGround = 1;
                return true;
            } 
        }
        break;
    case l:
        j = (dstRect.x + player.mapOffsetX) / tileSide;
        for (i = (dstRect.y + player.mapOffsetY) / tileSide; i < (dstRect.y + player.mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                dstRect.x = (j + 1) * tileSide - player.mapOffsetX;
                //dstRect.x -= dx;
                dx = 0;
            }
        }
    }
    return false;
}

void mentalHardEnemy::collisionWithPlayer(Player &player, SDL_DisplayMode displayMode, const unsigned short frameDelay) {
    if (dstRect.x > 0 && dstRect.x < displayMode.w - dstRect.w && dstRect.y > 0 && dstRect.y < displayMode.h - dstRect.h) {
        if (SDL_GetTicks() - damageTimer > 6 * frameDelay) {
            damageTimer = SDL_GetTicks();
            player.lives -= damage;
        }
    }
}

void mentalHardEnemy::collisionWithPlayerAttacks(Player &player, Arrow *arrow) {
    if (arrow->rect.x > dstRect.x - arrow->rect.w && arrow->rect.x < dstRect.x + dstRect.w && arrow->rect.y > dstRect.y - arrow->rect.h && arrow->rect.y < dstRect.y + dstRect.h) {
        lives -= arrow->damage;
    }
}

void mentalHardEnemy::collisionWithPlayerAttacks(Player &player, Bomb *bomb) {
    playerBombRadius = sqrt((center.x - bomb->center.x) * (center.x - bomb->center.x) + (center.y - bomb->center.y) * (center.y - bomb->center.y));
    bomb->damage = round(bomb->maxRadius - playerBombRadius) / 4;
    if (bomb->damage < 0) {
        bomb->damage = 0;
    }
    lives -= bomb->damage;
    if (lives < 0) 
        lives = 0;
}

void mentalHardEnemy::collisionWithPlayerAttacks(Player &player, Boomerang *boomerang) {
    if (boomerang->rect.x > dstRect.x - boomerang->rect.w && boomerang->rect.x < dstRect.x + dstRect.w && boomerang->rect.y > dstRect.y - boomerang->rect.h && boomerang->rect.y < dstRect.y + dstRect.h) {
        lives -= boomerang->damage;
    }
}

void mentalHardEnemy::updateAnim(animationCode code) {
    srcRect.x += frameOffsetX;
    if (srcRect.x > 2 * EnemyFrameW) {
        frameOffsetX = -EnemyFrameW;
        srcRect.x += 2 * frameOffsetX;
    }
    if (srcRect.x < EnemyFrameW) {
        frameOffsetX = EnemyFrameW;
    }
    srcRect.y = 2* EnemyFrameH;
}

mentalHardEnemy& mentalHardEnemy::operator=(const mentalHardEnemy &obj) {
    if (this != &obj) {
        k = obj.k;
        m = obj.m;
        dstRect = obj.dstRect;
        srcRect = obj.srcRect;
        livesRect = obj.livesRect;
        lives = obj.lives;
        dx = obj.dx;
        dy = obj.dy;
        frameOffsetX = obj.frameOffsetX;
        onGround = obj.onGround;
    }
    return *this;
}