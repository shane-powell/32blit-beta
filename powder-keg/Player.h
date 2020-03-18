#pragma once
#include <cstdint>
#include "32blit.hpp"
#include "GameState.h"
#include "Movement.h"
#include "Projectile.h"
#include "TileData.h"
#include "SpriteDef.h"

using namespace blit;
using namespace GameState;

    class Player {
    public:
        virtual ~Player() = default;

        const int16_t respawnTime = 200;

        Rect spriteSide = pirateSpriteSide;
        Rect spriteUp = pirateSpriteUp;
        Rect spriteDown = pirateSpriteDown;
        char dir = 'd';
        int8_t aim = 6;
        Point location = Point(16, 16);
        bool can_fire = true;
        int16_t canFireTimeout = 0;
        int16_t fire_delay = 250;
        Movement currentMovement;
        Point spawnLocation = Point(16, 16);
        bool alive = true;
        uint16_t respawnTimer = respawnTime;
        bool isPlayer = false;
        bool isFiring = false;
        int16_t xChange = 0;
        int16_t yChange = 0;
        Point newPlayerLocation;

        void RespawnPlayer() {
            this->location = this->spawnLocation;
            this->alive = true;
            this->dir = 'd';
            this->respawnTimer = respawnTime;
            this->can_fire = true;
            this->canFireTimeout = this->fire_delay;
            this->currentMovement.movementCount = 0;
            this->currentMovement.xMovement = 0;
            this->currentMovement.yMovement = 0;
        }

        void ProcessPlayer() {
            if (!this->alive) {
                if (this->respawnTimer == 0) {
                    RespawnPlayer();
                }
                else {
                    this->respawnTimer--;
                }
            }
            else {
                this->SetPlayerActions();
            }
        }

        virtual void SetPlayerActions() {
            xChange = 0;
            yChange = 0;
            newPlayerLocation = this->location;

            if (this->canFireTimeout > 0) {
                this->canFireTimeout--;
            }
            else {
                this->can_fire = true;
            }

            if (buttons & DPAD_LEFT || joystick.x < -100) {
                xChange -= 1;
                newPlayerLocation.x -= 16;
            }
            else if (buttons & DPAD_RIGHT || joystick.x > 100) {
                xChange += 1;
                newPlayerLocation.x += 16;
            }
            else if (buttons & DPAD_UP || joystick.y < -100) {
                yChange -= 1;
                newPlayerLocation.y -= 16;
            }
            else if (buttons & DPAD_DOWN || joystick.y > 100) {
                yChange += 1;
                newPlayerLocation.y += 16;
            }

            if (buttons & B) {
                isFiring = true;
                //DropBomb();
            }

            if (xChange > 0) {
                this->dir = 'r';
            }
            else if (xChange < 0) {
                this->dir = 'l';
            }
            else if (yChange < 0) {
                this->dir = 'u';

            }
            else {
                this->dir = 'd';

            }

            //this->ProcessPlayerMovement();
        }

        virtual void ProcessCannotMove() {

        }

        void MovePlayer(const TileData &currentTileData) {
    if (this->xChange != 0 || this->yChange != 0) {
        if (currentTileData.canMove) {
            if (this->currentMovement.movementCount == 0) {
                this->currentMovement.movementCount = 16;
                this->currentMovement.xMovement = this->xChange;
                this->currentMovement.yMovement = this->yChange;


            }

            if (this->yChange > 0 && this->xChange == 0) {
                this->aim = 2;
            }
            else if (this->yChange < 0 && this->xChange == 0) {
                this->aim = 8;
            }
            else if (this->xChange > 0 && this->yChange == 0) {
                this->aim = 6;
            }
            else if (this->xChange < 0 && this->yChange == 0) {
                this->aim = 4;
            }
            else if (this->xChange > 0 && this->yChange > 0) {
                this->aim = 3;
            }
            else if (this->xChange < 0 && this->yChange < 0) {
                this->aim = 7;
            }
            else if (this->xChange > 0 && this->yChange < 0) {
                this->aim = 9;
            }
            else if (this->xChange < 0 && this->yChange > 0) {
                this->aim = 1;
            }
        }
        else {
            this->ProcessCannotMove();
        }

    }

    if (this->currentMovement.movementCount > 0) {
        if (this->currentMovement.movementStep == this->currentMovement.movementDelay) {
            this->currentMovement.movementCount--;

            this->location.x += this->currentMovement.xMovement;
            this->location.y += this->currentMovement.yMovement;

            this->currentMovement.movementStep = 0;
        }
        else {
            this->currentMovement.movementStep++;
        }


    }
}

        
    };

    enum class AIPatrolPattern {
        LeftRight, UpDown, ClockWise, AntiClockWise, WallFollowLeft, WallfollowRight
    };

    class AIPlayer : public Player {
    public:
        AIPatrolPattern movementType = AIPatrolPattern::UpDown;
        std::vector<char> pathToSafePlace;

        virtual void ProcessCannotMove() {
            switch (this->movementType) {
            case AIPatrolPattern::AntiClockWise:
                switch (this->dir) {
                case 'l':
                    this->dir = 'd';
                    break;
                case 'd':
                    this->dir = 'r';
                    break;
                case 'r':
                    this->dir = 'u';
                    break;
                case 'u':
                    this->dir = 'l';
                    break;
                default:;
                }
                break;
            case AIPatrolPattern::LeftRight:
                if (this->dir != 'l' && this->dir != 'r') {
                    this->dir = 'l';
                }

                if (this->dir == 'r') {
                    this->dir = 'l';
                }
                else {
                    this->dir = 'r';
                }
                break;
            case AIPatrolPattern::UpDown:
                if (this->dir != 'u' && this->dir != 'd') {
                    this->dir = 'u';
                }

                if (this->dir == 'u') {
                    this->dir = 'd';
                }
                else {
                    this->dir = 'u';
                }
                break;
            case AIPatrolPattern::ClockWise:
                switch (this->dir) {
                case 'l':
                    this->dir = 'u';
                    break;
                case 'd':
                    this->dir = 'l';
                    break;
                case 'r':
                    this->dir = 'd';
                    break;
                case 'u':
                    this->dir = 'r';
                    break;
                default:;
                }
                break;
                case AIPatrolPattern::WallFollowLeft:
                switch (this->dir) {
                case 'l':
                    this->dir = 'd';
                    break;
                case 'd':
                    this->dir = 'r';
                    break;
                case 'r':
                    this->dir = 'u';
                    break;
                case 'u':
                    this->dir = 'l';
                    break;
                default:;
                }
                break;
                case AIPatrolPattern::WallfollowRight:
                switch (this->dir) {
                case 'l':
                    this->dir = 'u';
                    break;
                case 'd':
                    this->dir = 'l';
                    break;
                case 'r':
                    this->dir = 'd';
                    break;
                case 'u':
                    this->dir = 'r';
                    break;
                default:;
                }
                break;
            default:;
            }
        }

        //todo only check if no movement is in progress
        void SetPlayerActions() override {
            if (this->canFireTimeout > 0) {
                this->canFireTimeout--;
            }
            else {
                this->can_fire = true;
            }

            xChange = 0;
            yChange = 0;
            newPlayerLocation = this->location;
            if (this->currentMovement.movementCount <= 0) {
                switch (this->movementType) {
                case AIPatrolPattern::AntiClockWise:
                    break;
                case AIPatrolPattern::LeftRight:
                    if (this->dir != 'l' && this->dir != 'r') {
                        this->dir = 'l';
                    }

                    break;
                case AIPatrolPattern::UpDown:
                    if (this->dir != 'u' && this->dir != 'd') {
                        this->dir = 'u';
                    }

                    break;
                case AIPatrolPattern::ClockWise:
                    break;
                default:;
                }

                switch (this->dir) {
                case 'u':
                    yChange -= 1;
                    newPlayerLocation.y -= 16;
                    break;
                case 'd':
                    yChange += 1;
                    newPlayerLocation.y += 16;
                    break;
                case 'l':
                    xChange -= 1;
                    newPlayerLocation.x -= 16;
                    break;
                case 'r':
                    xChange += 1;
                    newPlayerLocation.x += 16;
                    break;
                }

            }
            //this->ProcessPlayerMovement(xChange, yChange, newPlayerLocation);
        }

        
    };



