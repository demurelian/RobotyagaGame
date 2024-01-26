#include "Player.h"

void Player::initialization(SDL_Renderer *ren, SDL_DisplayMode displayMode, unsigned short mapRaws, unsigned short mapColumns, unsigned short tileSide) {//sx - starting x, sy - starting y
    texture = IMG_LoadTexture(ren, playerTexture_p);
    dstRect = {0, (displayMode.h - PlayerH) / 2, PlayerW, PlayerH};
    srcRect = {0, 0, PlayerFrameW - spriteError, PlayerFrameH - spriteError};
    frameYOffset = PlayerFrameW;
    direction = 1;
    onGround = 1;
    dx = dy = 0;
    yellowKey = false,
    blueKey = false,
    greenKey = false,
    arrowEarned = false,
    bombEarned = false,
    boomerangEarned = false,
    onLadder = false,
    dead = false,
    shopPossible = false;
    
    nBombs = nArrows = 5;

    points = 0;

    minOffsetX = tileSide;
    minOffsetY = tileSide;
    maxOffsetX = (mapColumns-1)*tileSide - displayMode.w;
    maxOffsetY = (mapRaws-1)*tileSide - displayMode.h;

    mapOffsetX = startingOffsetX;//0;//maxOffsetX;
    mapOffsetY = startingOffsetY;//0;//maxOffsetY;

    animationTimer = staminaIncreaseTimer = staminaDecreaseTimer = livesRegenTimer = SDL_GetTicks();

    lives = stamina = maxLives = maxStamina = max_lives_and_stamina;
    staminaRegeneration = 100;
    livesRegeneration = 100;
}

void Player::update(SDL_Renderer *ren, SDL_DisplayMode displayMode, const Uint8 *keyboardState, const unsigned frameDelay, Uint8 *map[], const unsigned tileSide, const unsigned mapRaws, const unsigned mapColumns) {
    //cout << "p:" << points << endl;
    if (!onGround) {
        if (dy < 50)
            dy++;
    } else {
        dy = 0;
    }
    if (stamina < maxStamina) {
        if (SDL_GetTicks() - staminaIncreaseTimer > 50) {
            staminaIncreaseTimer = SDL_GetTicks();
            stamina += staminaRegeneration;
        }
    }
    if (lives < maxLives) {
        if (SDL_GetTicks() - livesRegenTimer > 1000) {
            livesRegenTimer = SDL_GetTicks();
            lives += livesRegeneration;
        }
    }
    //KEYBOARD CHECKING
    if (!keyboardState[SDL_SCANCODE_A] && !keyboardState[SDL_SCANCODE_D]) {
        dx = 0;
        if (onGround) {
            updateAnim(stay);
        }
    }
    if(keyboardState[SDL_SCANCODE_D]) {
        direction = 1;
        dx = 4;
        if (keyboardState[SDL_SCANCODE_LSHIFT] && stamina > 10) {
            dx = 10;
            if (SDL_GetTicks() - staminaDecreaseTimer > 13 * frameDelay) {
                staminaDecreaseTimer = SDL_GetTicks();
                stamina -= 10;
            }
            if (SDL_GetTicks() - animationTimer > 6 * frameDelay && onGround) {
                animationTimer = SDL_GetTicks();
                updateAnim(run);
            }
        } else {
            if (SDL_GetTicks() - animationTimer > 6 * frameDelay && onGround) {
                animationTimer = SDL_GetTicks();
                updateAnim(walk);
            }
        }
    }
    if(keyboardState[SDL_SCANCODE_A]) {
        direction = 0;
        dx = -4; 
        if (keyboardState[SDL_SCANCODE_LSHIFT] && stamina > 10) {
            dx = -10;
            if (SDL_GetTicks() - staminaDecreaseTimer > 13 * frameDelay) {
                staminaDecreaseTimer = SDL_GetTicks();
                stamina -= 10;
            }
            if (SDL_GetTicks() - animationTimer > 6 * frameDelay && onGround) {
                animationTimer = SDL_GetTicks();
                updateAnim(run);
            }
        } else {
            if (SDL_GetTicks() - animationTimer > 6 * frameDelay && onGround) {
                animationTimer = SDL_GetTicks();
                updateAnim(walk);
            }
        }
    }
    if ((keyboardState[SDL_SCANCODE_W]) && onGround && stamina > 10) {
        if (onLadder) {
            dy = -10;
        } else {
            onGround = 0;
            stamina -= 10;
            dy = -20;
            if (SDL_GetTicks() - animationTimer > 6 * frameDelay && onGround) {
                animationTimer = SDL_GetTicks();
                updateAnim(jump);
            }
        }
    }
    if (keyboardState[SDL_SCANCODE_S] && onLadder) {
        dy = 10;
    }
    //MOVES & COLLISION
    dstRect.x += dx;
    if (dx < 0) {
        Collision(l, map, tileSide, keyboardState);
    } 
    if (dx > 0) {
        Collision(r, map, tileSide, keyboardState);
    }
    
    dstRect.y += dy;
    if (dy < 0) {
        Collision(t, map, tileSide, keyboardState);
    }
    if (dy >= 0) {
        if(Collision(b, map, tileSide, keyboardState)) {
            onGround = true;
        } else {
            onGround = false;
        }
    }

    if (dstRect.x + mapOffsetX <= tileSide + (displayMode.w - PlayerW) / 2) {
       mapOffsetX = minOffsetX;
    } else { 
        if (dstRect.x + mapOffsetX >= (mapColumns-1) * tileSide - (displayMode.w + PlayerW) / 2) {
            mapOffsetX = maxOffsetX;
        } else {
            mapOffsetX = mapOffsetX + dstRect.x - (displayMode.w - PlayerW)/2;
            dstRect.x = (displayMode.w - PlayerW)/2;
        }
    }

    if (dstRect.y + mapOffsetY <= tileSide + (displayMode.h - PlayerH) / 2) {
        mapOffsetY = minOffsetY;
    } else { 
        if (dstRect.y + mapOffsetY >= (mapRaws-1) * tileSide - (displayMode.h + PlayerH) / 2) {
            mapOffsetY = maxOffsetY;
        } else {
            mapOffsetY = mapOffsetY + dstRect.y - (displayMode.h - PlayerH)/2;
            dstRect.y = (displayMode.h - PlayerH)/2;
        }
    }

    if(direction) {
        SDL_RenderCopy(ren, texture, &srcRect, &dstRect);
    } else {
        SDL_RenderCopyEx(ren, texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}

void Player::updateAnim(playerAnimations animationCode) {
    switch (animationCode) {
    case stay: 
        srcRect.y = 0;
        srcRect.x = 0;
        break;
    case walk:
        srcRect.y = 0;
        if (srcRect.x < 3 * PlayerFrameW) {
            srcRect.x += PlayerFrameW;
        } else {
            srcRect.x = 0;
        }
        break;
    case run:
        srcRect.y = PlayerFrameH;
        srcRect.x += frameYOffset;
        if (srcRect.x == 0) {
            srcRect.x = PlayerFrameW;
        }
        if (srcRect.x > 3 * PlayerFrameW) {
            srcRect.x = 3 * PlayerFrameW;
        }
        if (srcRect.x == 3 * PlayerFrameW) {
            frameYOffset = -PlayerFrameW;
        }
        if (srcRect.x == PlayerFrameW) {
            frameYOffset = PlayerFrameW;
        }
        break;
    case jump:
        srcRect.y = 2 * PlayerFrameH;
        srcRect.x += frameYOffset;
        if (srcRect.x == 0) {
            srcRect.x = PlayerFrameW;
        }
        if (srcRect.x > 3 * PlayerFrameW) {
            srcRect.x = 3 * PlayerFrameW;
        }
        if (srcRect.x == 3 * PlayerFrameW) {
            frameYOffset = -PlayerFrameW;
        }
        if (srcRect.x == PlayerFrameW) {
            frameYOffset = PlayerFrameW;
        }
    }
}

bool Player::Collision(collisionCode code, Uint8 *map[], const unsigned tileSide, const Uint8 *keyboardState) {
    int i, j;
    onLadder = false;
    shopPossible = false;
    switch (code) {
    case t:
        i = (dstRect.y + mapOffsetY) / tileSide;
        for (j = (dstRect.x + mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + mapOffsetX) / (float)tileSide; j++)
        {
            if (map[i][j])
                switch (map[i][j]) {
                case 'R':
                case 'T':
                case 'U':
                case 'I':
                case 'G':
                    break;
                case ']':
                    points++;
                    map[i][j] = 0;
                    break;
                case '\\':
                    points += 10;
                    map[i][j] = 0;
                    break;
                case '^':
                    bombEarned = true;
                    map[i][j] = 0;
                    break;
                case '_':
                    arrowEarned = true;
                    map[i][j] = 0;
                    break;
                case 'b':
                    greenKey = true;
                    map[i][j] = 0;
                    break;
                case '`':
                    blueKey = true;
                    map[i][j] = 0;
                    break;
                case 'a':
                    yellowKey = true;
                    map[i][j] = 0;
                    break;
                case 'Z':
                case '[':
                    if (!yellowKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'X':
                case 'Y':
                    if (!greenKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'V':
                case 'W':
                    if (!blueKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    } else {
                       shopPossible = true;
                    }
                    break;
                case 'M':
                case 'N':
                    onGround = true;
                    onLadder = true;
                    break;
                case '/':
                case 'A':
                    lives--;
                    break;
                default:
                    dstRect.y -= dy;
                    dy = 0;
                }  
        }
    case r:
        j = (dstRect.x + dstRect.w + mapOffsetX) / tileSide;
        for (i = (dstRect.y + mapOffsetY) / tileSide; i < (dstRect.y + mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                switch (map[i][j]) {
                case 'R':
                case 'T':
                case 'U':
                case 'I':
                case 'G':
                    break;
                case ']':
                    points++;
                    map[i][j] = 0;
                    break;
                case '\\':
                    points += 10;
                    map[i][j] = 0;
                    break;
                case '^':
                    bombEarned = true;
                    map[i][j] = 0;
                    break;
                case '_':
                    arrowEarned = true;
                    map[i][j] = 0;
                    break;
                case 'b':
                    greenKey = true;
                    map[i][j] = 0;
                    break;
                case '`':
                    blueKey = true;
                    map[i][j] = 0;
                    break;
                case 'a':
                    yellowKey = true;
                    map[i][j] = 0;
                    break;
                case 'Z':
                case '[':
                    if (!yellowKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'X':
                case 'Y':
                    if (!greenKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'V':
                case 'W':
                    if (!blueKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    } else {
                       shopPossible = true;
                    }
                    break;
                case 'M':
                case 'N':
                    onGround = 1;
                    onLadder = true;
                    break;
                case '/':
                case 'A':
                    lives--;
                    break;
                default:
                    dstRect.x -= dx;
                    dx = 0;
                }
            }
        }
        break;
    case b:
        i = (dstRect.y + mapOffsetY + dstRect.h) / tileSide;
        for (j = (dstRect.x + mapOffsetX) / tileSide; j < (dstRect.x + dstRect.w + mapOffsetX) / (float)tileSide; j++) {
            if (map[i][j]) {
                switch (map[i][j]) {
                case 'R':
                case 'T':
                case 'U':
                case 'I':
                case 'G':
                    break;
                case ']':
                    points++;
                    map[i][j] = 0;
                    break;
                case '\\':
                    points += 10;
                    map[i][j] = 0;
                    break;
                case '^':
                    bombEarned = true;
                    map[i][j] = 0;
                    break;
                case '_':
                    arrowEarned = true;
                    map[i][j] = 0;
                    break;
                case 'b':
                    greenKey = true;
                    map[i][j] = 0;
                    break;
                case '`':
                    blueKey = true;
                    map[i][j] = 0;
                    break;
                case 'a':
                    yellowKey = true;
                    map[i][j] = 0;
                    break;
                case 'Z':
                case '[':
                    if (!yellowKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'X':
                case 'Y':
                    if (!greenKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'V':
                case 'W':
                    if (!blueKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    } else {
                       shopPossible = true;
                    }
                    break;
                case 'M':
                case 'N':
                    onGround = 1;
                    onLadder = true;
                    return true;
                    break;
                case '/':
                case 'A':
                    lives--;
                    break;

                default:
                    dstRect.y -= dy;
                    dy = 0;
                    return true;
                }
            } 
        }
        break;
    case l:
        j = (dstRect.x + mapOffsetX) / tileSide;
        for (i = (dstRect.y + mapOffsetY) / tileSide; i < (dstRect.y + mapOffsetY + dstRect.h) / (float)tileSide; i++) {
            if (map[i][j]) {
                switch (map[i][j]) {
                case 'R':
                case 'T':
                case 'U':
                case 'I':
                case 'G':
                    break;
                case ']':
                    points++;
                    map[i][j] = 0;
                    break;
                case '\\':
                    points += 10;
                    map[i][j] = 0;
                    break;
                case '^':
                    bombEarned = true;
                    map[i][j] = 0;
                    break;
                case '_':
                    arrowEarned = true;
                    map[i][j] = 0;
                    break;
                case 'b':
                    greenKey = true;
                    map[i][j] = 0;
                    break;
                case '`':
                    blueKey = true;
                    map[i][j] = 0;
                    break;
                case 'a':
                    yellowKey = true;
                    map[i][j] = 0;
                    break;
                case 'Z':
                case '[':
                    if (!yellowKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'X':
                case 'Y':
                    if (!greenKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    }
                    break;
                case 'V':
                case 'W':
                    if (!blueKey) {
                        dstRect.x -= dx;
                        dx = 0;
                    } else {
                       shopPossible = true;
                    }
                    break;
                case 'M':
                case 'N':
                    onGround = 1;
                    onLadder = true;
                    break;
                case '/':
                case 'A':
                    lives--;
                    break;
                default:
                    dstRect.x -= dx;
                    dx = 0;
                }
            }
        }
    }
    return false;
}