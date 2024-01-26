#include <iostream>
#include "Player.h"
#include "Arrow.h"
#include "Boomerang.h"
#include "Bomb.h"
#include "defaultEnemy.h"
#include "boomEnemy.h"
#include "mentalEnemy.h"
#include "mentalHardEnemy.h"
#include "Wolf.h"
#include <fstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>           //oh sh*t here we go again
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

const unsigned  FPS = 60,
                frameDelay = 1000 / FPS,
                ScreenW = 1366,
                ScreenH = 768,
                tileSide = 48,
                mapRaws = 50,
                mapColumns = 100,
                tilesNumber = 34,
                fontSize = 30,
                textYstep = 30,
                shopX = 400,
                shopIconSide = 35,
                youDiedY = 200;

const char  *maintile_p = "textures/maintile.png",
            *bg_p = "textures/bg/bg.png",
            *menuBg_p = "textures/bg/menuBg.png",
            *ingameMenuBg_p = "textures/bg/ingameMenu.png",
            *font_p = "ttf/9303.ttf",
            *shopBg_p = "textures/bg/shopBg.png",
            *shopIcon_p = "textures/shop.png",
            *livesTexture_p = "textures/heart.png",
            *staminaTexture_p = "textures/energy.png",
            *richesTexture_p = "textures/riches.png",
            *winTexture_p = "textures/youwin.png",
            *map_p = "map.txt",
            *greenKey_p = "textures/greenKey.png";

const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

struct obj {
    SDL_Rect rect;
    SDL_Texture *texture;
};

struct text {
    string msg;
    Uint8 fontSize;
    TTF_Font *font;
    SDL_Color color;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    
    text(SDL_Renderer *ren, string msg, Uint8 fontSize, SDL_Color color) {
        font = TTF_OpenFont(font_p, fontSize);
        surface = TTF_RenderText_Solid(font, msg.c_str(), color);
        texture = SDL_CreateTextureFromSurface(ren, surface);
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    }
    void update(string msg, SDL_Renderer *ren, SDL_Color color) {
        surface = TTF_RenderText_Solid(font, msg.c_str(), color);
        texture = SDL_CreateTextureFromSurface(ren, surface);
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    }
    void copy(SDL_Renderer *ren, int x, int y) {
        rect.x = x;
        rect.y = y;
        SDL_RenderCopy(ren, texture, nullptr, &rect);
    }
};

enum class menuExitCode {game, quit};

menuExitCode menu(SDL_Renderer *ren, SDL_DisplayMode displayMode, bool ingame) {
    bool menu = true;
        SDL_Texture *menuBg;
    string menuQuit;
    if (!ingame) {
        menuBg = IMG_LoadTexture(ren, menuBg_p);
        menuQuit = "quit";
    } else {
        menuBg = IMG_LoadTexture(ren, ingameMenuBg_p);
        menuQuit = "main menu";
    }
    SDL_Color   white = {255,255,255,255},
                blue = {242,225,116,255};
    text robotyaga(ren, "robotyaga", fontSize, white);
    text game(ren, "game", fontSize, white);
    text quit(ren, menuQuit, fontSize, white);
    text gameB(ren, "game", fontSize, blue);
    text quitB(ren, menuQuit, fontSize, blue);
    Uint32 changeButtonTimer = SDL_GetTicks();
    SDL_Event e;
    int pos = 0;
    while (menu) {
        while (SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) {
                menu = false;
                return menuExitCode::quit;
            }
            if (e.type == SDL_KEYDOWN) {
                if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
                    if (SDL_GetTicks() - changeButtonTimer > 10 * frameDelay) {
                        changeButtonTimer = SDL_GetTicks();
                        pos++;
                        if (pos == 2) {
                            pos = 0;
                        }
                    }
                }
                if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
                {
                    if (SDL_GetTicks() - changeButtonTimer > 10 * frameDelay) {
                        changeButtonTimer = SDL_GetTicks();
                        pos--;
                        if (pos == -1) {
                            pos = 1;
                        }
                    }
                }
                if (keyboardState[SDL_SCANCODE_RETURN]) {
                    //menu = false;
                    return (menuExitCode)pos;
                }
            }
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, menuBg, nullptr, nullptr);
        robotyaga.copy(ren, (displayMode.w - robotyaga.rect.w)/2, (displayMode.h - robotyaga.rect.h)/2);
        if (pos != 0)
            game.copy(ren, (displayMode.w - game.rect.w)/2, robotyaga.rect.y + 2* textYstep);
        else
            gameB.copy(ren, (displayMode.w - game.rect.w)/2, robotyaga.rect.y + 2* textYstep);
        if (pos != 1)
            quit.copy(ren, (displayMode.w - quit.rect.w)/2, robotyaga.rect.y + 3.1 * textYstep);
        else
            quitB.copy(ren, (displayMode.w - quit.rect.w)/2, robotyaga.rect.y + 3.1 * textYstep);
        SDL_RenderPresent(ren);
    }
}

void enemySpawn(Player player, Uint8 *map[], SDL_Renderer *ren, vector <defaultEnemy> &defaultEnemies, vector <boomEnemy> &boomEnemies, vector <mentalEnemy> &mentalEnemies, vector <mentalHardEnemy> &mentalHardEnemies) {//}, vector <mentalHardEnemy> mentalHardEnemies) {
    SDL_Rect tile;
    tile.w = tile.h = tileSide;
    for (int i = 0; i < mapRaws; i++) {
        for (int j = 0; j < mapColumns; j++) {
            tile.x = j * tileSide - player.mapOffsetX;
            tile.y = i * tileSide - player.mapOffsetY;
            if (map[i][j] == '1') {
                defaultEnemies.resize(defaultEnemies.size() + 1);
                defaultEnemies[defaultEnemies.size() - 1].initialization(ren, tile.x, tile.y);
                map[i][j] = 0;
            }
            if (map[i][j] == '2') {
                boomEnemies.resize(boomEnemies.size() + 1);
                boomEnemies[boomEnemies.size() - 1].initialization(ren, tile.x, tile.y);
                map[i][j] = 0;
            }
            if (map[i][j] == '3') {
                mentalEnemies.resize(mentalEnemies.size() + 1);
                mentalEnemies[mentalEnemies.size() - 1].initialization(ren, tile.x, tile.y);
                mentalEnemies[mentalEnemies.size() - 1].k = i;
                mentalEnemies[mentalEnemies.size() - 1].m = j;
                //map[i][j] = 0;
            }
            if (map[i][j] == '4') {
                mentalHardEnemies.resize(mentalHardEnemies.size() + 1);
                mentalHardEnemies[mentalHardEnemies.size() - 1].initialization(ren, tile.x, tile.y);
                mentalHardEnemies[mentalHardEnemies.size() - 1].k = i;
                mentalHardEnemies[mentalHardEnemies.size() - 1].m = j;
                //map[i][j] = 0;
            }
        }
    }
}

void loadTextures(SDL_Renderer *ren, Uint8 *map[], SDL_Texture *tiles[]) {
    string  commonPath = "textures/Tiles/platformPack_tile0",
            png = ".png",
            tilePath;
    for (int i = 1; i <= tilesNumber; i++) {
        tilePath = commonPath;
        if (i < 10) {
            tilePath += "0";
        }
        tilePath += to_string(i);
        tilePath += png;
        tiles[i] = IMG_LoadTexture(ren, tilePath.c_str());
    }
}

void readMap(SDL_Renderer *ren, Uint8 *map[]) {
    fstream in(map_p);
    in.seekg(0);
    for (int i = 0; i < mapRaws; i++) {
        for (int j = 0; j < mapColumns; j++) {
            char a;
            in.get(a);
            if (a != '\n') {
                if (a == ' ') {
                    map[i][j] = 0;
                } else {
                    map[i][j] = a;
                }
            }
        }
        if (i != mapRaws - 1) {
            in.ignore(1);
        }
    }
    in.close();
}

void renderMap(SDL_Renderer *ren, Player player, Uint8 *map[], SDL_Texture *tiles[], vector <mentalEnemy> &mentalEnemies, vector <mentalHardEnemy> &mentalHardEnemies, SDL_Texture *greenKey) {
    Uint8 number;
    obj tile;
    tile.rect.w = tileSide;
    tile.rect.h = tileSide;
    int p = 0, q = 0;
    for (int i = 0; i < mapRaws; i++) {
        for (int j = 0; j < mapColumns; j++) {
            tile.rect.x = j * tileSide - player.mapOffsetX;
            tile.rect.y = i * tileSide - player.mapOffsetY;
            if (map[i][j] == '3') {
                mentalEnemies[p].dstRect.x = tile.rect.x;
                mentalEnemies[p].dstRect.y = tile.rect.y;
                p++;
            } else if (map[i][j] == '4') {
                mentalHardEnemies[q].dstRect.x = tile.rect.x;
                mentalHardEnemies[q].dstRect.y = tile.rect.y;
                q++;
            } else if (map[i][j] == '/') {
                tile.texture = tiles[1];//needles texture
                SDL_RenderCopyEx(ren, tile.texture, nullptr, &tile.rect, 0, nullptr, SDL_FLIP_VERTICAL);
            } else if (map[i][j]) {
                number = map[i][j] - 64;
                if (number == 34) { //green key
                    tile.texture = greenKey;
                } else {
                    tile.texture = tiles[number];
                }
                SDL_RenderCopy(ren, tile.texture, nullptr, &tile.rect);
            }
        }
    }
}

void shop(SDL_Renderer *ren, Player &player, SDL_DisplayMode displayMode, short *price) {
    bool shop = true;
    SDL_Texture *shopBg = IMG_LoadTexture(ren, shopBg_p);
    obj shopIcon;
    shopIcon.texture = IMG_LoadTexture(ren, shopIcon_p);
    SDL_Color   white = {255,255,255,255},
                blue = {242,225,116,255},
                red = {255,0,0,255};
    string  shopStr = "shop",
            boomerangStr = "boomerang                                ",
            staminaRegen = "stamina regeneration x5    ",
            livesRegen =   "lives regeneration x2       ",
            points = " points";
    text shopText(ren, shopStr, fontSize, white);
    text boomerang(ren, boomerangStr + to_string(price[0]) + points, fontSize, white);
    text boomerangB(ren, boomerangStr + to_string(price[0]) + points, fontSize, blue);
    text boomerangR(ren, boomerangStr + to_string(price[0]) + points, fontSize, red);
    text x2staminaRegeneration(ren, staminaRegen + to_string(price[1]) + points, fontSize, white);
    text x2staminaRegenerationB(ren, staminaRegen + to_string(price[1]) + points, fontSize, blue);
    text x2staminaRegenerationR(ren, staminaRegen + to_string(price[1]) + points, fontSize, red);
    text x4livesRegen(ren, livesRegen + to_string(price[2]) + points, fontSize, white);
    text x4livesRegenB(ren, livesRegen + to_string(price[2]) + points, fontSize, blue);
    text x4livesRegenR(ren, livesRegen + to_string(price[2]) + points, fontSize, red);
    Uint32 changeButtonTimer = SDL_GetTicks();
    SDL_Event e;
    int pos = 0;
    while (shop) {
        boomerang.update(boomerangStr + to_string(price[0]) + points, ren, white);
        boomerangB.update(boomerangStr + to_string(price[0]) + points, ren, blue);
        boomerangR.update(boomerangStr + to_string(price[0]) + points, ren, red);
        x2staminaRegeneration.update(staminaRegen + to_string(price[1]) + points, ren, white);
        x2staminaRegenerationB.update(staminaRegen + to_string(price[1]) + points, ren, blue);
        x2staminaRegenerationR.update(staminaRegen + to_string(price[1]) + points, ren, red);
        x4livesRegen.update(livesRegen + to_string(price[2]) + points, ren, white);
        x4livesRegenB.update(livesRegen + to_string(price[2]) + points, ren, blue);
        x4livesRegenR.update(livesRegen + to_string(price[2]) + points, ren, red);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, shopBg, nullptr, nullptr);
        while (SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) {
                shop = false;
            }
            if (e.type == SDL_KEYDOWN) {
                if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
                    if (SDL_GetTicks() - changeButtonTimer > 10 * frameDelay) {
                        changeButtonTimer = SDL_GetTicks();
                        pos++;
                        if (pos == 3) {
                            pos = 0;
                        }
                    }
                }
                if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
                {
                    if (SDL_GetTicks() - changeButtonTimer > 10 * frameDelay) {
                        changeButtonTimer = SDL_GetTicks();
                        pos--;
                        if (pos == -1) {
                            pos = 2;
                        }
                    }
                }
                if (keyboardState[SDL_SCANCODE_ESCAPE]) {
                    shop = false;
                }
                if (keyboardState[SDL_SCANCODE_RETURN]) {
                    if (pos == 0 && price[0] <= player.points) {
                        player.points -= price[0];
                        player.boomerangEarned = true;
                        price[0] = 9999;
                    }
                    if (pos == 1 && price[1] <= player.points) {
                        player.points -= price[1];
                        player.staminaRegeneration *= 2;
                        price[1] = 200;
                    }
                    if (pos == 2 && price[1] <= player.points) {
                        player.points -= price[2];
                        player.livesRegeneration *= 4;
                        price[2] = 500;
                    }
                }
            }
        }
        shopText.copy(ren, (displayMode.w - shopText.rect.w)/2, (displayMode.h - shopText.rect.h)/2);
        shopIcon.rect.x = shopText.rect.x + shopText.rect.w;
        shopIcon.rect.y = shopText.rect.y;
        shopIcon.rect.w = shopIcon.rect.h = shopIconSide;
        SDL_RenderCopy(ren, shopIcon.texture, nullptr, &shopIcon.rect);
        if (pos != 0) {
            boomerang.copy(ren, shopX, shopText.rect.y + 2* textYstep);
        } else if (player.points >= price[0]) {
                boomerangB.copy(ren, shopX, shopText.rect.y + 2* textYstep);
            } else {
                    boomerangR.copy(ren, shopX, shopText.rect.y + 2* textYstep);
            }
        if (pos != 1) {
            x2staminaRegeneration.copy(ren, shopX, shopText.rect.y + 3 * textYstep);
        } else if (player.points >= price[1]) {
                x2staminaRegenerationB.copy(ren, shopX, shopText.rect.y + 3 * textYstep);
            } else {
                    x2staminaRegenerationR.copy(ren, shopX, shopText.rect.y + 3 * textYstep);
                }
        if (pos != 2) {
            x4livesRegen.copy(ren, shopX, shopText.rect.y + 4 * textYstep);
        } else if (player.points >= price[2]) {
                x4livesRegenB.copy(ren, shopX, shopText.rect.y + 4 * textYstep);
            } else {
                    x4livesRegenR.copy(ren, shopX, shopText.rect.y + 4 * textYstep);
                }
        SDL_RenderPresent(ren);
    }
}

void frameRate() {
    Uint32  frameStart,
            frameTime;
    frameStart = SDL_GetTicks();
    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < frameDelay) {
        SDL_Delay(frameDelay - frameTime);
    }
}

void won(SDL_Renderer *ren, SDL_DisplayMode displayMode) {
    Uint32 updateTimer = SDL_GetTicks(), winTimer = SDL_GetTicks();
    obj win;
    win.texture = IMG_LoadTexture(ren, winTexture_p);
    SDL_QueryTexture(win.texture, 0, 0, &win.rect.w, &win.rect.h);
    while (SDL_GetTicks() - winTimer < 625 * frameDelay) {
        if (SDL_GetTicks() - updateTimer > 7 *frameDelay) {
            updateTimer = SDL_GetTicks();
            win.rect.y = rand() % (displayMode.h - win.rect.h);
            win.rect.x = rand() % (displayMode.w - win.rect.w);
            SDL_RenderCopy(ren, win.texture, nullptr, &win.rect);
            SDL_RenderPresent(ren);
        }
    }
}

void dead(SDL_Renderer *ren, Player player, SDL_DisplayMode displayMode) {
    Uint32 timer = SDL_GetTicks();
    SDL_Color   white = {255,255,255,255},
                yellow = {255,255,0,255};
    text youDied(ren, "You died!", fontSize * 2, white);
    text yourScore(ren, "Score: ", fontSize, white);
    text score(ren, to_string(player.points), fontSize, yellow);
        youDied.copy(ren, (displayMode.w - youDied.rect.w)/2, youDiedY);
        yourScore.copy(ren, (displayMode.w - yourScore.rect.w - score.rect.w)/2, youDiedY + 3*youDied.rect.h);
        score.copy(ren, (displayMode.w - yourScore.rect.w - score.rect.w)/2 + yourScore.rect.w, youDiedY + 3*youDied.rect.h);
        SDL_RenderPresent(ren);
    while (SDL_GetTicks() - timer < 625*frameDelay) {
    }
}

int main() 
{
    srand(time(nullptr));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << SDL_GetError() << endl;
        return 1;
    }
    SDL_ShowCursor(SDL_DISABLE);
    if (TTF_Init() != 0) {
        cout << TTF_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        cout << Mix_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_DisplayMode displayMode;
    SDL_GetDisplayMode(0, 0, &displayMode);

    SDL_Window *win = SDL_CreateWindow("robotyaga", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *bg = IMG_LoadTexture(ren, bg_p);
    SDL_Texture *greenKey = IMG_LoadTexture(ren, greenKey_p);

    

    SDL_Event e;
    bool gameOver = true, mainMenu = true;

    const Uint8 items = 3;
    short price[items];
    price[0] = 200;//boomerang price
    price[1] = 50;//stamina regen price
    price[2] = 100;//lives regen price

    obj heart, energy, riches;

    heart.texture = IMG_LoadTexture(ren, livesTexture_p);
    energy.texture = IMG_LoadTexture(ren, staminaTexture_p);
    riches.texture = IMG_LoadTexture(ren, richesTexture_p);

    SDL_Color   white = {255,255,255,255},
                red = {255,0,0,255},
                blue = {0,0,255,255},
                yellow = {255,255,0,255};

    text    nLives(ren, "e", fontSize, white),
            nStamina(ren, "e", fontSize, white),
            nPoints(ren, "e", fontSize, white);

    Uint8 map[mapRaws][mapColumns];
    Uint8 *p[mapRaws];//for send map into functions
    for (int i = 0; i < mapRaws; i++) {
        p[i] = map[i];
    }
    SDL_Texture *tiles[tilesNumber];//tiles array

    Player player;
    vector <defaultEnemy> defaultEnemies;
    vector <boomEnemy> boomEnemies;
    vector <mentalEnemy> mentalEnemies;
    vector <mentalHardEnemy> mentalHardEnemies;

    Boomerang *boomerang;
    bool boomerangCreated = false;
    short nBoomerangs = 1;

    Bomb *bomb;
    bool bombCreated = false;

    Arrow *arrow;
    bool arrowCreated = false;

    Wolf *wolf;
    bool wolfEarned, wolfCreated = false;
    loadTextures(ren, p, tiles);
    while (mainMenu) {
        switch (menu(ren, displayMode, false)) {
        case menuExitCode::game:
            readMap(ren, p);
            boomerangCreated = false;
            bombCreated = false;
            arrowCreated = false;
            player.initialization(ren, displayMode, mapRaws, mapColumns, tileSide);
            defaultEnemies.resize(0);
            boomEnemies.resize(0);
            mentalEnemies.resize(0);
            mentalHardEnemies.resize(0);
            enemySpawn(player, p, ren, defaultEnemies, boomEnemies, mentalEnemies, mentalHardEnemies);
            gameOver = false;
            break;
        case menuExitCode::quit:
            mainMenu = false;
            gameOver = true;
            return 0;
            cout << mainMenu << " " << gameOver << endl;
        }
        while (!gameOver) {
            frameRate();
            for (int i = 0; i < defaultEnemies.size(); i++) {
                if (defaultEnemies[i].lives <= 0) {
                    defaultEnemies[i] = defaultEnemies[defaultEnemies.size() - 1];
                    defaultEnemies.resize(defaultEnemies.size() - 1);
                }
            }
            for (int i = 0; i < boomEnemies.size(); i++) {
                if (boomEnemies[i].lives <= 0) {
                    boomEnemies[i] = boomEnemies[boomEnemies.size() - 1];
                    boomEnemies.resize(boomEnemies.size() - 1);
                }
            }
            for (int i = 0; i < mentalEnemies.size(); i++) {
                if (mentalEnemies[i].lives <= 0) {
                    map[mentalEnemies[i].k][mentalEnemies[i].m] = 0;
                    for (int j = i; j < mentalEnemies.size() - 1; j++) {
                        mentalEnemies[j] = mentalEnemies[j + 1];
                    }
                    mentalEnemies.resize(mentalEnemies.size() - 1);
                }
            }
            for (int i = 0; i < mentalHardEnemies.size(); i++) {
                if (mentalHardEnemies[i].lives <= 0) {
                    map[mentalHardEnemies[i].k][mentalHardEnemies[i].m] = 0;
                    for (int j = i; j < mentalHardEnemies.size() - 1; j++) {
                        mentalHardEnemies[j] = mentalHardEnemies[j + 1];
                    }
                    mentalHardEnemies.resize(mentalHardEnemies.size() - 1);
                }
            }
            if (bombCreated && SDL_GetTicks() - bomb->timer > 1650) {
                for (int i = 0; i < defaultEnemies.size(); i++) {
                    defaultEnemies[i].collisionWithPlayerAttacks(player, bomb);
                }
                for (int i = 0; i < boomEnemies.size(); i++) {
                    boomEnemies[i].collisionWithPlayerAttacks(player, bomb);
                }
                for (int i = 0; i < mentalEnemies.size(); i++) {
                    mentalEnemies[i].collisionWithPlayerAttacks(player, bomb);
                }
                for (int i = 0; i < mentalHardEnemies.size(); i++) {
                    mentalHardEnemies[i].collisionWithPlayerAttacks(player, bomb);
                }
                if (wolfCreated) {
                    wolf->collisionWithBomb(player, bomb);
                    if (wolf->lives <= 0) {
                        wolfEarned = 1;
                    }
                }
                bombCreated = false;
                delete bomb;
            }
            if (boomerangCreated) {
                if (boomerang->returned) {
                    delete boomerang;
                    boomerangCreated = false;
                    nBoomerangs++;
                } else {
                    if (SDL_GetTicks() - boomerang->timer > 8000) {
                        delete boomerang;
                        boomerangCreated = false;
                    }
                }
            }
            if (arrowCreated) {
                if (SDL_GetTicks() - arrow->timer > 1200) {
                    delete arrow;
                    arrowCreated = false;
                }
            }
            if (wolfCreated) {
                if (wolf->lives <= 0 || (wolf->direction && wolf->dstRect.x >= displayMode.w) || (!wolf->direction && wolf->dstRect.x <= 0 - wolf->WolfW) || wolf->dstRect.y > displayMode.h - tileSide || wolf->dstRect.y < 0) {
                    delete wolf;
                    wolfCreated = false;
                }
            }
            while (SDL_PollEvent(&e)) { 
                if (e.type == SDL_QUIT) {
                    gameOver = true;
                }
                if (e.type == SDL_KEYDOWN) {
                    if(e.key.keysym.sym == SDLK_l) {
                        if (player.boomerangEarned && !boomerangCreated && nBoomerangs > 0) {
                            boomerang = new Boomerang(ren, player, player.direction);
                            boomerangCreated = true;
                            nBoomerangs--;
                        }
                    }
                    if(e.key.keysym.sym == SDLK_k) {
                        if (!bombCreated && player.bombEarned && player.nBombs > 0) {
                            bomb = new Bomb(ren, player, player.direction);
                            bombCreated = true;
                            player.nBombs--;
                        }
                    }
                    if(e.key.keysym.sym == SDLK_j) {
                        if (!arrowCreated && player.arrowEarned && player.nArrows > 0) {
                            arrow = new Arrow(ren, player, player.direction);
                            arrowCreated = true;
                            player.nArrows--;
                        }
                    }
                    if(e.key.keysym.sym == SDLK_SPACE) {
                        if (player.shopPossible) {
                            shop(ren, player, displayMode, price);
                        }
                        if (!wolfCreated) {
                        //  wolf = new Wolf(ren, player, displayMode, player.direction);
                            //wolfCreated = true;
                        }
                    }
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        switch (menu(ren, displayMode, true)) {
                        case menuExitCode::game:
                            ;
                            break;
                        case menuExitCode::quit:
                            gameOver = true;
                        }
                    }
                }
            }
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, bg, nullptr, nullptr);
            renderMap(ren, player, p, tiles, mentalEnemies, mentalHardEnemies, greenKey);
            if (boomerangCreated) {
                for (int i = 0; i < defaultEnemies.size(); i++) {
                    defaultEnemies[i].collisionWithPlayerAttacks(player, boomerang);
                }
                for (int i = 0; i < boomEnemies.size(); i++) {
                    boomEnemies[i].collisionWithPlayerAttacks(player, boomerang);
                }
                for (int i = 0; i < mentalEnemies.size(); i++) {
                    mentalEnemies[i].collisionWithPlayerAttacks(player, boomerang);
                }
                for (int i = 0; i < mentalHardEnemies.size(); i++) {
                    mentalHardEnemies[i].collisionWithPlayerAttacks(player, boomerang);
                }
            }
            if (arrowCreated) {
                for (int i = 0; i < defaultEnemies.size(); i++) {
                    defaultEnemies[i].collisionWithPlayerAttacks(player, arrow);
                }
                for (int i = 0; i < boomEnemies.size(); i++) {
                    boomEnemies[i].collisionWithPlayerAttacks(player, arrow);
                }
                for (int i = 0; i < mentalEnemies.size(); i++) {
                    mentalEnemies[i].collisionWithPlayerAttacks(player, arrow);
                }
                for (int i = 0; i < mentalHardEnemies.size(); i++) {
                    mentalHardEnemies[i].collisionWithPlayerAttacks(player, arrow);
                }
            }
            for (int i = 0; i < defaultEnemies.size(); i++) {
                defaultEnemies[i].update(ren, player, p, tileSide, frameDelay, displayMode);
            }
            for (int i = 0; i < boomEnemies.size(); i++) {
                boomEnemies[i].update(ren, player, p, tileSide, frameDelay, displayMode);
            }
            //cout << mentalEnemies.size() << endl;
            for (int i = 0; i < mentalEnemies.size(); i++) {
                mentalEnemies[i].update(ren, player, p, tileSide, frameDelay, displayMode);
            }
            for (int i = 0; i < mentalHardEnemies.size(); i++) {
                mentalHardEnemies[i].update(ren, player, p, tileSide, frameDelay, displayMode);
            }
            //if (player.lives > 0)
                player.update(ren, displayMode, keyboardState, frameDelay, p, tileSide, mapRaws, mapColumns);
            if (player.lives <= 0) {
                gameOver = true;
                dead(ren, player, displayMode);
            }
            if (player.dstRect.x == 1283 && player.mapOffsetX == 3386 && player.dstRect.y == 58 && player.mapOffsetY == 48) {
                //won(ren, displayMode);
                won(ren, displayMode);
                gameOver = true;
            }
            if (boomerangCreated)
                boomerang->update(ren, player);
            if (bombCreated)
                bomb->update(ren, player);
            if (arrowCreated)
                arrow->update(ren, player);
            if (wolfCreated) 
                wolf->update(ren, player, p, tileSide, displayMode);
            nLives.update(to_string(player.lives), ren, white);
            heart.rect.x = 0;
            heart.rect.y = displayMode.h - nLives.rect.h;
            heart.rect.w = heart.rect.h = nLives.rect.h;
            SDL_RenderCopy(ren, heart.texture, nullptr, &heart.rect);
            nLives.copy(ren, heart.rect.w, displayMode.h - nLives.rect.h);
            nStamina.update(to_string(player.stamina), ren, blue);
            energy.rect.x = 0;
            energy.rect.y = displayMode.h - nLives.rect.h - nStamina.rect.h;
            energy.rect.h = energy.rect.w = nStamina.rect.h;
            SDL_RenderCopy(ren, energy.texture, nullptr, &energy.rect);
            nStamina.copy(ren, energy.rect.w, displayMode.h - nLives.rect.h - nStamina.rect.h);
            nPoints.update(to_string(player.points), ren, white);
            riches.rect.x = 0;
            riches.rect.y = displayMode.h - nLives.rect.h - nStamina.rect.h - nPoints.rect.h;
            riches.rect.w = riches.rect.h = nPoints.rect.h;
            SDL_RenderCopy(ren, riches.texture, nullptr, &riches.rect);
            nPoints.copy(ren, riches.rect.w, riches.rect.y);
            if (!gameOver)
                SDL_RenderPresent(ren);
        }
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
    return 0;
}