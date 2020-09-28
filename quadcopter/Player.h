#pragma once

#include <cstdint>
#include "32blit.hpp"
#include "GameState.h"
#include "Movement.h"
#include "Projectile.h"
#include "TileData.h"
#include "SpriteDef.h"
#include "Node.cpp"

using namespace blit;
using namespace GameState;

class Player {
public:
    virtual ~Player() = default;

    const int16_t respawnTime = 200;

    Node *directions = new Node();
    Rect spriteSide = quadSpriteSide;
    Rect spriteUp = quadSpriteUp;
    Rect spriteDown = QuadSpriteDown;
    char dir = 'd';
    int8_t aim = 6;
    Point lastLocation = Point(16, 16);
    Point location = Point(16, 16);
    bool can_fire = true;
    int16_t canFireTimeout = 0;
    int16_t fire_delay = 250;
    Movement currentMovement;
    Point spawnLocation = Point(32, 32);
    bool alive = true;
    uint16_t respawnTimer = respawnTime;
    bool isPlayer = false;
    bool isFiring = false;
    int16_t xChange = 0;
    int16_t yChange = 0;
    Point newPlayerLocation;
    SpriteAnimation animation;
    float xSpeed = 0.0f;
    float maxSpeed = 1.0f;
    float ySpeed = 0.0f;
    int8_t cannotMoveCount = 0;

    Player() {
        uint8_t frameLength = 1;
        animation.animationFrames.emplace_back(quadFrame1, frameLength);
        animation.animationFrames.emplace_back(quadFrame2, frameLength);
        animation.animationFrames.emplace_back(quadFrame3, frameLength);
        animation.animationFrames.emplace_back(quadFrame4, frameLength);
        animation.animationFrames.emplace_back(quadFrame5, frameLength);
        animation.animationFrames.emplace_back(quadFrame6, frameLength);
        animation.animationFrames.emplace_back(quadFrame7, frameLength);
        animation.animationFrames.emplace_back(quadFrame8, frameLength);

    }


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

    void ProcessPlayer(std::map<char, TileData> playerTileScan) {
        if (!this->alive) {
            if (this->respawnTimer == 0) {
                RespawnPlayer();
            } else {
                this->respawnTimer--;
            }
        } else {
            this->SetPlayerActions(playerTileScan);
        }
    }

    void slow_down_object()
    {
        const float slow_rate = 0.001;
    	
	    if(xChange == 0 && xSpeed != 0.0f)
	    {
		    if(xSpeed > 0.0f)
		    {
			    xSpeed-= slow_rate;
		    }
		    else
		    {
			    xSpeed += slow_rate;
		    }
	    }

	    if (yChange == 0 && ySpeed != 0.0f)
	    {
		    if (ySpeed > 0.0f)
		    {
			    ySpeed-= slow_rate;
		    }
		    else
		    {
			    ySpeed += slow_rate;
		    }

	    	if(ySpeed > 0.0f && ySpeed < 0.01f)
	    	{
                ySpeed = 0;
	    	}
	    	else if(ySpeed < 0.0f && ySpeed > -0.01)
	    	{
                ySpeed = 0;
	    	}

            if (xSpeed > 0.0f && xSpeed < 0.01f)
            {
                xSpeed = 0;
            }
            else if (xSpeed < 0.0f && xSpeed > -0.01)
            {
                xSpeed = 0;
            }
	    }
    }

    virtual void SetPlayerActions(std::map<char, TileData> playerTileScan) {

        auto const accelerationRate = 0.01f;
    	
        xChange = 0;
        yChange = 0;
        newPlayerLocation = this->location;

        if (this->canFireTimeout > 0) {
            this->canFireTimeout--;
        } else {
            this->can_fire = true;
        }

        if (buttons & DPAD_LEFT || joystick.x < 0.0f) {
            xChange -= 1;
        	if(xSpeed > maxSpeed * -1)
        	{
                xSpeed -= accelerationRate;
        	}
            //newPlayerLocation.x -= 1;
        } else if (buttons & DPAD_RIGHT || joystick.x > 0.0f) {
            xChange += 1;
        	if(xSpeed < maxSpeed)
        	{
                xSpeed += accelerationRate;
        	}
            //newPlayerLocation.x += 1;
        } if (buttons & DPAD_UP || joystick.y < 0.0f) {
            yChange -= 1;
            if (ySpeed > maxSpeed * -1)
            {
                ySpeed -= accelerationRate;
            }
            //newPlayerLocation.y -= 1;
        } else if (buttons & DPAD_DOWN || joystick.y > 0.0f) {
            yChange += 1;
            if (ySpeed < maxSpeed)
            {
                ySpeed += accelerationRate;
            }
            //newPlayerLocation.y += 1;
        }

        xSpeed += fmod(this->currentMovement.xMovement, 1);
        ySpeed += fmod(this->currentMovement.yMovement, 1);

        newPlayerLocation.x += xSpeed;
        newPlayerLocation.y += ySpeed;

    	




        this->currentMovement.xMovement = this->xSpeed;
        this->currentMovement.yMovement = this->ySpeed;




        if (buttons & B) {
            isFiring = true;
            //DropBomb();
        }

        if (xSpeed > 0.0f) {
            this->dir = 'r';
        } else if (xSpeed < 0.0f) {
            this->dir = 'l';
        } else if (ySpeed < 0.0f) {
            this->dir = 'u';

        } else if (ySpeed > 0.0f) {
            this->dir = 'd';

        }
    	
        if(xChange == 0 && yChange == 0)
        {
            slow_down_object();
        	
            xSpeed = (int)xSpeed;
            ySpeed = (int)ySpeed;
        }
    }

    virtual void ProcessCannotMove() {

        cannotMoveCount ++;
    	
        if (xSpeed >= 1 || xSpeed <= -0.1)
        {
            xSpeed = (xSpeed * -1) / 2;
        }
        /*else
            xSpeed = 0;*/

        if (ySpeed >= 1 || ySpeed <= -0.1)
        {
            ySpeed = (ySpeed * -1) / 2;
        }
        //else
        //    ySpeed = 0;
        //

    	if(lastLocation.x == location.x && lastLocation.y == location.y && cannotMoveCount > 1)
    	{
            xSpeed = 0;
            ySpeed = 0;
    	}
    }

    void MovePlayer(const TileData &currentTileData) {
        if (this->xSpeed != 0 || this->ySpeed != 0) {
            if (currentTileData.canMove) {
                //if (this->currentMovement.movementCount == 0) {
                //    this->currentMovement.movementCount = 1;
                //   



                //}

                /*this->location.x += this->currentMovement.xMovement;
                this->location.y += this->currentMovement.yMovement;*/

                this->location.x = this->newPlayerLocation.x;
                this->location.y = this->newPlayerLocation.y;

                lastLocation = location;
                cannotMoveCount = 0;


                if (this->yChange > 0 && this->xChange == 0) {
                    this->aim = 2;
                } else if (this->yChange < 0 && this->xChange == 0) {
                    this->aim = 8;
                } else if (this->xChange > 0 && this->yChange == 0) {
                    this->aim = 6;
                } else if (this->xChange < 0 && this->yChange == 0) {
                    this->aim = 4;
                } else if (this->xChange > 0 && this->yChange > 0) {
                    this->aim = 3;
                } else if (this->xChange < 0 && this->yChange < 0) {
                    this->aim = 7;
                } else if (this->xChange > 0 && this->yChange < 0) {
                    this->aim = 9;
                } else if (this->xChange < 0 && this->yChange > 0) {
                    this->aim = 1;
                }
            } else {
                this->ProcessCannotMove();
            }

        }

       /* if (this->currentMovement.movementCount > 0) {
            if (this->currentMovement.movementStep == this->currentMovement.movementDelay) {
                this->currentMovement.movementCount--;

                this->location.x += this->currentMovement.xMovement;
                this->location.y += this->currentMovement.yMovement;

                this->currentMovement.movementStep = 0;
            } else {
                this->currentMovement.movementStep++;
            }


        }*/
    }


};

enum class AIPatrolPattern {
    LeftRight, UpDown, ClockWise, AntiClockWise, WallFollowLeft, WallfollowRight
};

class AIPlayer : public Player {
public:
    AIPatrolPattern movementType = AIPatrolPattern::UpDown;
    std::vector<char> pathToSafePlace;

    void initDirections() {
        switch (movementType) {
            case AIPatrolPattern::ClockWise:
            case AIPatrolPattern::AntiClockWise:
            case AIPatrolPattern::WallFollowLeft:
            case AIPatrolPattern::WallfollowRight:
                /* code */
                break;

            default:
                break;
        }

        Node *l = new Node();
        l->data = 'l';
        l->previousNode = directions;

        Node *u = new Node();
        u->data = 'u';
        u->previousNode = l;

        l->nextNode = u;

        Node *r = new Node();
        r->data = 'r';
        r->previousNode = u;

        u->nextNode = r;
        r->nextNode = directions;

        directions->data = 'd';
        directions->nextNode = l;
        directions->previousNode = r;
    }

    AIPlayer(AIPatrolPattern pattern) {
        this->movementType = pattern;

        initDirections();
    }

    virtual void ProcessCannotMove() {
        switch (this->movementType) {
            case AIPatrolPattern::AntiClockWise:

                while (directions->data != dir) {
                    directions = directions->nextNode;
                }

                directions = directions->previousNode;
                this->dir = directions->data;
                break;
            case AIPatrolPattern::LeftRight:
                if (this->dir != 'l' && this->dir != 'r') {
                    this->dir = 'l';
                }

                if (this->dir == 'r') {
                    this->dir = 'l';
                } else {
                    this->dir = 'r';
                }
                break;
            case AIPatrolPattern::UpDown:
                if (this->dir != 'u' && this->dir != 'd') {
                    this->dir = 'u';
                }

                if (this->dir == 'u') {
                    this->dir = 'd';
                } else {
                    this->dir = 'u';
                }
                break;
            case AIPatrolPattern::ClockWise:
                while (directions->data != dir) {
                    directions = directions->nextNode;
                }

                directions = directions->nextNode;

                this->dir = directions->data;
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
    void SetPlayerActions(std::map<char, TileData> playerTileScan) override {
        if (this->canFireTimeout > 0) {
            this->canFireTimeout--;
        } else {
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



