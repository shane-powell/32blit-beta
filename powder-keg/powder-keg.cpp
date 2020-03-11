#include "powder-keg.hpp"
#include "Explosion.h"
#include "Projectile.h"
#include "TileAnimationFrame.h"
#include "TileData.h"
#include "GameState.h"
#include "Player.h"

using namespace blit;
using namespace GameState;

TileMap world((uint8_t *) layer_world, nullptr, Size(tilemap_width, tilemap_height), nullptr);

int16_t maxX = 319;
int16_t minX = 0;
int16_t maxY = 229;
int16_t minY = 0;
int8_t logCounter = 0;
char gameState = 'G';
bool sound = false;


int8_t spriteSize = 16;

int viewX = 0;

//int8_t borderOffset = 2;

int viewPortX = 0;













struct TileAnimation {
    uint16_t tileIndex;
    uint8_t frameIndex = 0;
    std::vector<TileAnimationFrame> animationFrames;

    TileAnimation(uint16_t tileIndex) {
        this->tileIndex = tileIndex;
    }
};

std::vector<TileAnimation> tileAnimations;


static std::vector<Player*> players;

int score = 0;

bool is_Point_in_Rect(const Point &pointToCheck, std::vector<Rect>::value_type bounding_Rectangle) {
    if (pointToCheck.x + sprite_width > bounding_Rectangle.x &&
        pointToCheck.x < bounding_Rectangle.x + bounding_Rectangle.w &&
        pointToCheck.y + sprite_width > bounding_Rectangle.y &&
        pointToCheck.y < bounding_Rectangle.y + bounding_Rectangle.h) {
        return true;
    }
    return false;
}

void DrawWorld() {
    Vec2 wo(64, 40);

    world.transform =
            Mat3::identity() *
            Mat3::translation(wo) *
            Mat3::scale(Vec2(0.5, 0.5)) *
            Mat3::translation(Vec2(-128, -80));

    world.draw(&screen, Rect(0, 0, 320, 240), nullptr);

}


void StartGame() {
    score = 0;
    gameState = 'G';
}

void EndGame() {
    gameState = 'E';
}

void InitPlayers() {
    players.clear();

    players.push_back(new Player());

    auto player2 = new AIPlayer();
    player2->spawnLocation = Point(288, 16);
    player2->location = player2->spawnLocation;
    player2->isPlayer = false;
    player2->spriteDown = ninjaSpriteDown;
    player2->spriteUp = ninjaSpriteUp;
    player2->spriteSide = ninjaSpriteSide;
    player2->currentMovement.movementDelay = 1;
    player2->movementType = AIPatrolPattern::ClockWise;
    players.push_back(player2);

    auto player3 = new AIPlayer();
    player3->spawnLocation = Point(16, 208);
    player3->location = player3->spawnLocation;
    player3->isPlayer = false;
    player3->spriteDown = p3SpriteDown;
    player3->spriteUp = p3SpriteUp;
    player3->spriteSide = p3SpriteSide;
    player3->currentMovement.movementDelay = 5;
    player3->movementType = AIPatrolPattern::AntiClockWise;
    players.push_back(player3);

    auto player4 = new AIPlayer();
    player4->spawnLocation = Point(288, 208);
    player4->location = player4->spawnLocation;
    player4->isPlayer = false;
    player4->spriteDown = p4SpriteDown;
    player4->spriteUp = p4SpriteUp;
    player4->spriteSide = p4SpriteSide;
    player4->currentMovement.movementDelay = 2;
    player4->movementType = AIPatrolPattern::ClockWise;
    players.push_back(player4);


}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::hires);

    screen.sprites = SpriteSheet::load(packed_data);

    world.sprites = screen.sprites;

    InitPlayers();

}

void RenderTileAnimations() {
    auto tile = tileAnimations.begin();

    while (tile != tileAnimations.end()) {

        bool complete = false;

        const auto currentFrame = tile->animationFrames[tile->frameIndex];

        layer_world[tile->tileIndex] = currentFrame.tileId;

        if (currentFrame.frameCount >= currentFrame.frameLength) {
            if (tile->frameIndex >= tile->animationFrames.size() - 1) {
                complete = true;
            } else {
                tile->frameIndex++;
            }
        }

        tile->animationFrames[tile->frameIndex].frameCount++;

        /*auto hit = is_Point_in_Rect(projectile->location, Rect(npc->location, Size(sprite_width, sprite_width)));*/
        if (complete) {
            tile = tileAnimations.erase(tile);

            //todo temp hack
            //player_world[tile->tileIndex] = 66;
            //projectile->lifetime = 0;
        } else ++tile;
    }
}

void RenderExplosions() {
    for (const Explosion &explosion : GameState::explosions) {
        screen.sprite(explosionCore, explosion.location, Point(0, 0), Vec2(2, 2));

        for (int i = 1; i <= explosion.lengthRight; i++) {
            if (i < explosion.lengthRight) {
                screen.sprite(explosionLine, Point(explosion.location.x + (i * sprite_width), explosion.location.y),
                              Point(0, 0), Vec2(2, 2));
            } else {
                screen.sprite(explosionEnd, Point(explosion.location.x + (i * sprite_width), explosion.location.y),
                              Point(0, 0), Vec2(2, 2));
            }
        }

        for (int i = 1; i <= explosion.lengthLeft; i++) {
            if (i < explosion.lengthLeft) {
                screen.sprite(explosionLine, Point(explosion.location.x - (i * sprite_width), explosion.location.y),
                              Point(0, 0), Vec2(2, 2));
            } else {
                screen.sprite(explosionEnd, Point(explosion.location.x - (i * sprite_width), explosion.location.y),
                              Point(0, 0), Vec2(2, 2), SpriteTransform::R180);

            }
        }

        for (int i = 1; i <= explosion.lengthDown; i++) {
            if (i < explosion.lengthDown) {
                screen.sprite(explosionLine, Point(explosion.location.x, explosion.location.y + (i * sprite_width)),
                              Point(0, 0), Vec2(2, 2), SpriteTransform::R270);
            } else {
                screen.sprite(explosionEnd, Point(explosion.location.x, explosion.location.y + (i * sprite_width)),
                              Point(0, 0), Vec2(2, 2), SpriteTransform::R90);
            }
        }

        for (int i = 1; i <= explosion.lengthUp; i++) {
            if (i < explosion.lengthUp) {
                screen.sprite(explosionLine, Point(explosion.location.x, explosion.location.y - (i * sprite_width)),
                              Point(0, 0), Vec2(2, 2), SpriteTransform::R270);
            } else {
                screen.sprite(explosionEnd, Point(explosion.location.x, explosion.location.y - (i * sprite_width)),
                              Point(0, 0), Vec2(2, 2), SpriteTransform::R270);
            }
        }
    }
}

void RenderPlayers() {
    auto p = players.begin();

    while (p != players.end()) {


        if ((*p)->alive) {
            if ((*p)->dir == 'r') {
                screen.sprite((*p)->spriteSide, (*p)->location, Point(0, 8), Vec2(2, 2));
            } else if ((*p)->dir == 'l') {
                screen.sprite((*p)->spriteSide, (*p)->location, Point(0, 8), Vec2(2, 2), 1);
            } else if ((*p)->dir == 'u') {
                screen.sprite((*p)->spriteUp, (*p)->location, Point(0, 8), Vec2(2, 2));

            } else if ((*p)->dir == 'd') {
                screen.sprite((*p)->spriteDown, (*p)->location, Point(0, 8), Vec2(2, 2));

            }
        }


        ++p;
    }


}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();

    // draw some text at the top of the screen
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(255, 255, 255);
    //screen.rectangle(Rect(0, 0, 320, 14));
    screen.pen = Pen(0, 0, 0);
    //screen.text("Hello 32blit!", minimal_font, Point(5, 4));
    if (gameState == 'G') {
        DrawWorld();

        screen.pen = Pen(255, 255, 0);

        //screen.sprite(player.sprite, player.location,Point(0, 0), Vec2(2, 2));

        //screen.sprite(ninjaSpriteSide, player.location, Point(0, 8), Vec2(2, 2));

        RenderExplosions();

        RenderPlayers();

        for (const Projectile &projectile : projectiles) {
            screen.sprite(projectile.sprite, projectile.location, Point(0, 0), Vec2(2, 2), projectile.transform);
        }


        /*for (const Npc& npc : npcs)
        {
            screen.sprite(npc.sprite, npc.location, Point(0, 0), Vec2(2, 2));
        }*/

        //screen.text("Score ", minimal_font, Point(63, 0));

        //char scoreBuff[5];
        //sprintf_s(scoreBuff, "%05d", score);

        //screen.text(std::to_string(score), minimal_font, Point(95, 0));
    } else if (gameState == 'E') {

        screen.text("Game Over", minimal_font, Point(maxX / 2, 120), true, center_h);


        screen.text("Score ", minimal_font, Point(maxX / 2, 130), true, center_h);

        // char scoreBuff[5];
        //sprintf_s(scoreBuff, "%05d", score);

        screen.text(std::to_string(score), minimal_font, Point(maxX / 2, 140), true, center_h);

    } else if (gameState == 'T') {

        screen.text("Snack", minimal_font, Point(maxX / 2, 1), true, center_h);


        screen.text("By Shane Powell", minimal_font, Point(maxX / 2, 15), true, center_h);


        screen.text("Sound ", minimal_font, Point(50, 55));


        if (sound == true) {
            screen.pen = Pen(255, 255, 255);
            screen.rectangle(Rect(88, 53, 18, 11));
            screen.pen = Pen(0, 0, 0);
            screen.rectangle(Rect(89, 54, 16, 9));
        } else {
            screen.pen = Pen(255, 255, 255);
            screen.rectangle(Rect(107, 53, 21, 11));
            screen.pen = Pen(0, 0, 0);
            screen.rectangle(Rect(108, 54, 19, 9));
        }
        screen.pen = Pen(255, 255, 255);


        screen.text("On ", minimal_font, Point(90, 55));

        screen.text("Off ", minimal_font, Point(109, 55));
    }

    screen.pen = Pen(0, 0, 0);


}

void CreateChestExplosion(const TileData tileData) {
    for (const TileAnimation &ti : tileAnimations) {
        if (ti.tileIndex == tileData.index) {
            return;
        }
    }


    TileAnimation animation = TileAnimation(tileData.index);
    animation.animationFrames.emplace_back(17, 10);
    animation.animationFrames.emplace_back(33, 10);
    animation.animationFrames.emplace_back(66, 1);

    tileAnimations.push_back(animation);
}

void CheckExplosionTileData(bool &canMove, const Point &point) {
    const auto tileData = getLocalTileData(point, sprite_width, tilemap_width);
    canMove = tileData.canMove;

    if (tileData.id == 1) {
        layer_world[tileData.index] = 66;

        CreateChestExplosion(tileData);
    }
}


void CheckIfExplosionHitPlayer(bool &canMove, const Point &point) {
    auto p = players.begin();

    while (p != players.end()) {
        const auto hit = is_Point_in_Rect(point, Rect((*p)->location, Size(16, 16)));

        if ((*p)->alive && hit) {
            (*p)->alive = false;
        }
        ++p;
    }
}


void PerformCollisionChecks(bool &canMove, const Point &point) {
    CheckExplosionTileData(canMove, point);
    if (canMove) {
        CheckIfExplosionHitPlayer(canMove, point);
    }
}

void CreateExplosion(const std::vector<Projectile>::iterator &projectile) {
    Explosion explosion;
    explosion.location = projectile->location;

    bool canMove = true;

    while (canMove) {
        Point point = Point(explosion.location.x + (sprite_width * (explosion.lengthRight + 1)), explosion.location.y);

        PerformCollisionChecks(canMove, point);

        if (canMove) {
            explosion.lengthRight++;
        }
    }

    canMove = true;

    while (canMove) {
        Point point = Point(explosion.location.x - (sprite_width * (explosion.lengthLeft + 1)), explosion.location.y);
        PerformCollisionChecks(canMove, point);

        if (canMove) {
            explosion.lengthLeft++;
        }
    }

    canMove = true;

    while (canMove) {
        Point point = Point(explosion.location.x, explosion.location.y + (sprite_width * (explosion.lengthDown + 1)));
        PerformCollisionChecks(canMove, point);

        if (canMove) {
            explosion.lengthDown++;
        }
    }

    canMove = true;

    while (canMove) {
        Point point = Point(explosion.location.x, explosion.location.y - (sprite_width * (explosion.lengthUp + 1)));
        PerformCollisionChecks(canMove, point);

        if (canMove) {
            explosion.lengthUp++;
        }
    }


    GameState::explosions.push_back(explosion);
}

void updateProjectiles() {
    auto projectile = GameState::projectiles.begin();

    while (projectile != GameState::projectiles.end()) {

        /*auto npc = npcs.begin();

        while (npc != npcs.end()) {
            auto hit = is_Point_in_Rect(projectile->location, Rect(npc->location, Size(sprite_width, sprite_width)));
            if (hit)
            {
                npc = npcs.erase(npc);
                projectile->lifetime = 0;
            }
            else ++npc;
        }*/

        projectile->lifetime--;
        projectile->location.x += projectile->vel_x;
        projectile->location.y += projectile->vel_y;

        auto projTileData = getLocalTileData(projectile->location, sprite_width, tilemap_width);

        if (projectile->lifetime <= 0) {
            CreateExplosion(projectile);

            projectile = projectiles.erase(projectile);

        } else ++projectile;
    }

    auto explosion = explosions.begin();

    while (explosion != explosions.end()) {
        explosion->lifeTime--;
        if (explosion->lifeTime <= 0) {
            explosion = explosions.erase(explosion);
        } else {
            ++explosion;
        }
    }
}

void UpdatePlayers() {
    static uint16_t lastButtons = 0;
    uint16_t changed = blit::buttons ^lastButtons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    for (Player *player : players) {
        player->ProcessPlayer();
    }

    lastButtons = blit::buttons;
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {

    updateProjectiles();
    //updateNpcs();

    RenderTileAnimations();

    UpdatePlayers();
}