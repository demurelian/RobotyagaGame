#include "defaultEnemy.h"

void defaultEnemy::initialization(SDL_Renderer *ren, int x, int y) {
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

    animationTimer = livesTimer = SDL_GetTicks();
}

void defaultEnemy::update(SDL_Renderer *ren, Player &player, Uint8 *map[], const unsigned tileSide, const unsigned short frameDelay, SDL_DisplayMode displayMode) {
    collisionWithPlayer(player, frameDelay);

    if (!onGround) {
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
    if (dstRect.x >= -dstRect.w && dstRect.x <= displayMode.w && dstRect.y >= -dstRect.h && dstRect.y <= displayMode.h) /*if enemy is on screen*/ {
        if (player.dstRect.x > dstRect.x) {
            dx = 5;
        } 
        if (player.dstRect.x < dstRect.x) {
            dx = -5;
        }
    }
    dstRect.x += dx;
    
    if (collisionWithMap(l, map, tileSide, player) || collisionWithMap(r, map, tileSide, player)) {
        if (SDL_GetTicks() - animationTimer > 13 * frameDelay) {
            animationTimer = SDL_GetTicks();
            updateAnim(idle);
        }
    } else {
        if (dx > 0) {
            if (SDL_GetTicks() - animationTimer > 13 * frameDelay) {
                animationTimer = SDL_GetTicks();
                updateAnim(walkR);
            }
        }
        if (dx == 0) {
            if (SDL_GetTicks() - animationTimer > 13 * frameDelay) {
                animationTimer = SDL_GetTicks();
                updateAnim(idle);
            }
        }
        if (dx < 0) {
            if (SDL_GetTicks() - animationTimer > 13 * frameDelay) {
                animationTimer = SDL_GetTicks();
                updateAnim(walkL);
            }
        }
    }
    dstRect.y += dy;
    collisionWithMap(t, map, tileSide,player);
    if (collisionWithMap(b, map, tileSide, player)) {
        onGround = true;
    } else {
        onGround = false;
    }

    center.x = dstRect.x + dstRect.w/2;
    center.y = dstRect.y + dstRect.h/2;

    livesRect.w = lives;
    livesRect.x = dstRect.x + (dstRect.w - lives) / 2;
    livesRect.y = dstRect.y - barH;
    
    SDL_SetRenderDrawColor(ren, 0,0,0,255);
    SDL_RenderDrawRect(ren, &livesRect);
    SDL_RenderFillRect(ren, &livesRect);

    SDL_RenderCopy(ren, texture, &srcRect, &dstRect);
}

bool defaultEnemy::collisionWithMap(collisionCode code, Uint8 *map[], const unsigned tileSide, Player &player) {\
   int i, j;
    switch (code) {
    case t:
        i = (dstRect.y + player.mapOffsetY) / tileSide;
        for (j = (dstRect.x + player.mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + player.mapOffsetX) / (float)tileSide; j++)
        {
            if (map[i][j]) {
                switch (map[i][j]) {
                case ']':
                    break;
                default:
                    dstRect.y = (i + 1) * tileSide - player.mapOffsetY;
                    dy = 0;
                    return true;
                }
            }
        }
    case r:
        j = (dstRect.x + dstRect.w + player.mapOffsetX) / tileSide;
        for (i = (dstRect.y + player.mapOffsetY) / tileSide; i < (dstRect.y + player.mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                switch (map[i][j]) {
                case ']':
                    break;
                default:
                    dstRect.x -= dx;
                    dx = 0;
                    return true;
                }
            }
        }
        break;
    case l:
        j = (dstRect.x + player.mapOffsetX) / tileSide;
        for (i = (dstRect.y + player.mapOffsetY) / tileSide; i < (dstRect.y + player.mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                switch (map[i][j]) {
                case ']':
                    break;
                default:
                    dstRect.x -= dx;
                    dx = 0;
                    return true;
                }
            }
        
        }
        break;
    case b:
        i = (dstRect.y + player.mapOffsetY + dstRect.h) / tileSide;
        for (j = (dstRect.x + player.mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + player.mapOffsetX) / (float)tileSide; j++) {
            if (map[i][j]) {
                switch (map[i][j]) {
                case ']':
                    break;
                default:
                    dstRect.y = i * tileSide - dstRect.h - player.mapOffsetY;
                    dy = 0;
                    onGround = 1;
                    return true; 
                }
            } 
        }
    }
    return false;
}

void defaultEnemy::collisionWithPlayer(Player &player, const unsigned short frameDelay) {
    // if (!player.onGround && player.dstRect.y >= dstRect.y - dstRect.h && player.dstRect.x > dstRect.x - dstRect.w && player.dstRect.x < dstRect.x + dstRect.w) {
    //     if (SDL_GetTicks() - livesTimer > 6 * frameDelay) {
    //         livesTimer = SDL_GetTicks();
    //         lives -= 10;
    //     }
    //     player.dy = -20;
    // }
    if (player.dstRect.x > dstRect.x - player.dstRect.w && player.dstRect.x < dstRect.x + dstRect.w && player.dstRect.y > dstRect.y - player.dstRect.h && player.dstRect.y < dstRect.y + dstRect.h) {
        player.lives--;
    }
}

void defaultEnemy::collisionWithPlayerAttacks(Player &player, Arrow *arrow) {
    if (arrow->rect.x > dstRect.x - arrow->rect.w && arrow->rect.x < dstRect.x + dstRect.w && arrow->rect.y > dstRect.y - arrow->rect.h && arrow->rect.y < dstRect.y + dstRect.h) {
        lives -= arrow->damage;
    }
}

void defaultEnemy::collisionWithPlayerAttacks(Player &player, Bomb *bomb) {
    playerBombRadius = sqrt((center.x - bomb->center.x) * (center.x - bomb->center.x) + (center.y - bomb->center.y) * (center.y - bomb->center.y));
    bomb->damage = round(bomb->maxRadius - playerBombRadius) / 4;
    if (bomb->damage < 0) {
        bomb->damage = 0;
    }
    lives -= bomb->damage;
    if (lives < 0) 
        lives = 0;
}

void defaultEnemy::collisionWithPlayerAttacks(Player &player, Boomerang *boomerang) {
    if (boomerang->rect.x > dstRect.x - boomerang->rect.w && boomerang->rect.x < dstRect.x + dstRect.w && boomerang->rect.y > dstRect.y - boomerang->rect.h && boomerang->rect.y < dstRect.y + dstRect.h) {
        lives -= boomerang->damage;
    }
}

void defaultEnemy::updateAnim(animationCode code) {
    srcRect.x += frameOffsetX;
    if (srcRect.x > 2 * EnemyFrameW) {
        frameOffsetX = -EnemyFrameW;
        srcRect.x += 2 * frameOffsetX;
    }
    if (srcRect.x < EnemyFrameW) {
        frameOffsetX = EnemyFrameW;
    }
    switch (code) {
    case idle:
        srcRect.y = 2* EnemyFrameH;
        break;
    case walkR:
        srcRect.y = EnemyFrameH;
        break;
    case walkL:
        srcRect.y = 3 * EnemyFrameH;
    }
}

defaultEnemy& defaultEnemy::operator=(const defaultEnemy &obj) {
    if (this != &obj) {
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