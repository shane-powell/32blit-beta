#include "powder-keg.hpp"

using namespace blit;

enum AIPatrolPattern
{
    LeftRight, UpDown, ClockWise, AntiClockWise
};

static uint8_t layer_world[] = {
  48, 50, 51, 50, 49, 50, 51, 50, 49, 51, 50, 51, 50, 49, 51, 50, 51, 49, 50, 52, 00, 00, 00, 00, 00, 00, 26, 27, 28, 29, 30, 52,
  64, 37, 66, 66, 66, 01, 01, 01, 01, 66, 66, 66, 66, 66, 01, 01, 66, 66, 66, 68, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
  64, 65, 01, 01, 01, 01, 01, 01, 01, 66, 66, 66, 66, 66, 01, 01, 01, 01, 37, 68, 00, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 36, 36, 36, 36, 65, 66, 66, 65, 66, 66, 66, 66, 36, 36, 36, 36, 01, 68, 00, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 36, 01, 01, 01, 65, 66, 66, 66, 66, 66, 66, 66, 01, 01, 01, 36, 01, 68, 00, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 66, 01, 66, 01, 66, 36, 36, 66, 66, 36, 36, 66, 01, 65, 01, 66, 01, 68, 00, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 66, 01, 38, 01, 66, 36, 66, 66, 66, 66, 36, 66, 01, 66, 01, 66, 01, 68, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 66, 01, 65, 01, 66, 66, 66, 66, 65, 66, 66, 66, 01, 66, 01, 66, 01, 68, 00, 65, 66, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 66, 01, 66, 01, 66, 36, 66, 66, 66, 66, 36, 66, 01, 66, 01, 66, 01, 68, 00, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  64, 01, 66, 01, 35, 01, 66, 36, 36, 66, 66, 36, 36, 66, 01, 65, 01, 66, 01, 68, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 36, 01, 01, 01, 66, 66, 66, 66, 66, 66, 66, 66, 01, 01, 01, 36, 01, 68, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 01, 36, 36, 36, 36, 65, 66, 66, 66, 66, 66, 66, 66, 36, 36, 36, 36, 01, 68, 00, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 01, 01, 01, 01, 01, 66, 66, 66, 66, 66, 66, 66, 01, 01, 01, 01, 66, 68, 00, 65, 65, 65, 66, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 01, 01, 01, 01, 66, 66, 66, 66, 66, 01, 66, 66, 66, 66, 68, 00, 65, 65, 65, 66, 65, 65, 65, 65, 65, 65, 65,
  80, 81, 82, 83, 82, 81, 82, 83, 81, 82, 83, 81, 82, 83, 81, 82, 83, 81, 82, 84, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
  00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
};

const uint8_t sprite_width = 16;

// Point player_location = Point(32, 20);

const uint32_t tilemap_width = 32;

const uint32_t tilemap_height = 16;

const int16_t respawnTime = 200;

TileMap world((uint8_t*)layer_world, nullptr, Size(tilemap_width, tilemap_height), nullptr);

int16_t maxX = 319;
int16_t minX = 0;
int16_t maxY = 229;
int16_t minY = 0;
int8_t  logCounter = 0;
char gameState = 'G';
bool sound = false;

Rect playerSprite = Rect(0, 0, 1, 1);
Rect playerSpriteUp = Rect(0, 2, 1, 1);
Rect playerSpriteDown = Rect(0, 1, 1, 1);
Rect bombSprite = Rect(6, 0, 1, 1);
Rect explosionCore = Rect(7, 0, 1, 1);
Rect explosionLine = Rect(8, 0, 1, 1);
Rect explosionEnd = Rect(9, 0, 1, 1);

Rect pirateSpriteSide = Rect(1, 6, 1, 2);
Rect pirateSpriteUp = Rect(1, 10, 1, 2);
Rect pirateSpriteDown = Rect(1, 8, 1, 2);

Rect ninjaSpriteSide = Rect(5, 6, 1, 2);
Rect ninjaSpriteUp = Rect(5, 10, 1, 2);
Rect ninjaSpriteDown = Rect(5, 8, 1, 2);

Rect p3SpriteSide = Rect(2, 6, 1, 2);
Rect p3SpriteUp = Rect(2, 10, 1, 2);
Rect p3SpriteDown = Rect(2, 8, 1, 2);

Rect p4SpriteSide = Rect(3, 6, 1, 2);
Rect p4SpriteUp = Rect(3, 10, 1, 2);
Rect p4SpriteDown = Rect(3, 8, 1, 2);

Rect chestSprite = Rect(1,0,1,1);

int8_t spriteSize = 16;

int viewX = 0;

//int8_t borderOffset = 2;

int viewPortX = 0;

class Entity
{
public:
    Rect sprite;
    Point location;
};

struct Projectile : Entity
{
    uint8_t transform = 0;
    int16_t lifetime = 300;
    int16_t vel_x = 0;
    int16_t vel_y = 0;
};

struct Explosion : Entity
{
    //Rect sprite;
    //Point location;
    uint8_t lengthUp = 0;
    uint8_t lengthDown = 0;
    uint8_t lengthLeft = 0;
    uint8_t lengthRight = 0;
    int16_t  lifeTime = 50;

};

class Frame
{
public:
    uint8_t frameCount = 0;
    uint8_t frameLength = 1;
};

class AnimationFrame : public Frame
{
    Rect sprite;
    AnimationFrame(Rect sprite, uint8_t frameLength)
    {
        this->sprite = sprite;  	
        this->frameLength = frameLength;  	
    };
};

class TileAnimationFrame : public Frame
{
public:
    uint16_t tileId;
    TileAnimationFrame(uint16_t tileId, uint8_t frameLength)
    {
        this->tileId = tileId;
        this->frameLength = frameLength;
    };
};

struct TileAnimation
{
    uint16_t tileIndex;
    uint8_t frameIndex = 0;
    std::vector<TileAnimationFrame> animationFrames;
	TileAnimation(uint16_t tileIndex)
	{
        this->tileIndex = tileIndex;
	}
};

std::vector<TileAnimation> tileAnimations;

//std::map < uint16_t, std::vector<AnimationFrame>> animations;

//struct Block
//{
//    Point position;
//    Rect sprite = chestSprite;
//};

//static std::vector<Block> blocks;

static std::vector<Projectile> projectiles;

static std::vector<Explosion> explosions;

struct Movement
{
    int8_t xMovement = 0;
    int8_t yMovement = 0;
    uint8_t movementCount = 0;
    uint8_t movementDelay = 20;

};

class Player
{
public:
	virtual ~Player() = default;
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
	
    void ProcessPlayer()
    {
        if (!this->alive)
        {
            if (this->respawnTimer == 0)
            {
                RespawnPlayer();
            }
            else
            {
                this->respawnTimer--;
            }
        }
        else
        {
            this->SetPlayerActions();
        }
    }
	
    void DropBomb()
    {
        if (this->can_fire && this->location.x % 16 == 0 and this->location.y % 16 == 0)
        {
            this->can_fire = false;
            this->canFireTimeout = this->fire_delay;
            Projectile newProjectile;

            newProjectile.sprite = bombSprite;

            newProjectile.location.x = this->location.x;
            newProjectile.location.y = this->location.y;

            projectiles.push_back(newProjectile);
        }
    }
	
    virtual void SetPlayerActions() {
        int16_t xChange = 0;
        int16_t yChange = 0;
        Point newPlayerLocation = this->location;

        if (this->canFireTimeout > 0)
        {
            this->canFireTimeout--;
        }
        else
        {
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

            if (buttons & B)
            {
                DropBomb();
            }

            if (xChange > 0)
                {
                    this->dir = 'r';
                }
                else if (xChange < 0)
                {
                    this->dir = 'l';
                }
                else if (yChange < 0)
                {
                    this->dir = 'u';

                }
                else
                {
                    this->dir = 'd';

                }

            this->ProcessPlayerMovement(xChange, yChange, newPlayerLocation);
        }

        virtual void ProcessCannotMove()
        {

        }

    void ProcessPlayerMovement(int16_t xChange, int16_t yChange, Point newPlayerLocation)
    {
        //bool move_ok = true;

        const auto currentTileData = getLocalTileData(newPlayerLocation, sprite_width, tilemap_width);

        if (xChange != 0 || yChange != 0)
        {
            if (currentTileData.canMove)
            {
                if (this->currentMovement.movementCount == 0)
                {
                    this->currentMovement.movementCount = 16;
                    this->currentMovement.xMovement = xChange;
                    this->currentMovement.yMovement = yChange;
                }

                if (yChange > 0 && xChange == 0)
                {
                    this->aim = 2;
                }
                else if (yChange < 0 && xChange == 0)
                {
                    this->aim = 8;
                }
                else if (xChange > 0 && yChange == 0)
                {
                    this->aim = 6;
                }
                else if (xChange < 0 && yChange == 0)
                {
                    this->aim = 4;
                }
                else if (xChange > 0 && yChange > 0)
                {
                    this->aim = 3;
                }
                else if (xChange < 0 && yChange < 0)
                {
                    this->aim = 7;
                }
                else if (xChange > 0 && yChange < 0)
                {
                    this->aim = 9;
                }
                else if (xChange < 0 && yChange > 0)
                {
                    this->aim = 1;
                }
            }
            else
            {
                ProcessCannotMove();
            }
            
        }

        if (this->currentMovement.movementCount > 0)
        {
            this->currentMovement.movementCount--;

            this->location.x += this->currentMovement.xMovement;
            this->location.y += this->currentMovement.yMovement;
        }
    }
};

class AIPlayer : public Player{
public:
    AIPatrolPattern movementType = UpDown;
    std::vector<char> pathToSafePlace;

    virtual void ProcessCannotMove()
    {
        switch(this->movementType)
        {
            case AntiClockWise:
                break;
            case LeftRight:
                if (this->dir != 'l' && this->dir != 'r')
                {
                    this->dir = 'l';
                }

                if (this->dir == 'r')
                {
                    this->dir = 'l';
                }
                else
                {
                    this->dir = 'r';
                }
                break;
            case UpDown:
                if(this->dir != 'u' && this->dir != 'd')
                {
                    this->dir = 'u';
                }

                if(this->dir == 'u')
                {
                    this->dir = 'd';
                }
                else
                {
                    this->dir = 'u';
                }
                break;
            case ClockWise: break;
            default: ;
        }
    }

    virtual void SetPlayerActions()
	{
        int16_t xChange = 0;
        int16_t yChange = 0;
        Point newPlayerLocation = this->location;

        if (this->canFireTimeout > 0)
        {
            this->canFireTimeout--;
        }
        else
        {
            this->can_fire = true;
        }
    	
    	switch(this->movementType)
    	{
			case AntiClockWise:
                break;
            case LeftRight:
                if (this->dir != 'l' && this->dir != 'r')
                {
                    this->dir = 'l';
                }

                if (this->dir == 'r')
                {
                    xChange += 1;
                    newPlayerLocation.x += 16;
                }
                else
                {
                    xChange -= 1;
                    newPlayerLocation.x -= 16;
                }
                break;
            case UpDown: 
    			if(this->dir != 'u' && this->dir != 'd')
    			{
	                this->dir = 'u';   				
    			}

    			if(this->dir == 'u')
    			{
                    yChange -= 1;
                    newPlayerLocation.y -= 16;
    			}
	            else
	            {
                    yChange += 1;
                    newPlayerLocation.y += 16;
	            }
    			break;
            case ClockWise: break;
            default: ;
        }

        this->ProcessPlayerMovement(xChange,yChange, newPlayerLocation);
    }
};

static std::vector<Player*> players;

//std::vector<std::reference_wrapper<Player>> players;


//struct TileData
//{
//    uint8_t id = 0;
//    uint16_t index = 0;
//    bool canMove = true;
//    uint16_t pixels_in_water = 0;
//    bool in_water = false;
//    float movement_modifier = 0;
//    float life_modifier = 0;
//    //TileData();
//};

//Player player;

// TileData currentTileData = TileData(0);

void SetPlayerActions(Player &player, int16_t &xChange, int16_t &yChange, Point &newPlayerLocation);

int score = 0;

bool is_Point_in_Rect(const Point& pointToCheck, std::vector<Rect>::value_type bounding_Rectangle)
{
    if (pointToCheck.x + sprite_width > bounding_Rectangle.x && pointToCheck.x < bounding_Rectangle.x + bounding_Rectangle.w && pointToCheck.y + sprite_width > bounding_Rectangle.y&& pointToCheck.y < bounding_Rectangle.y + bounding_Rectangle.h)
    {
        return true;
    }
    return false;
}

uint16_t getTileFromPoint(const Point& Point, uint8_t tile_size, uint8_t tile_map_width)
{
    uint16_t horizontal_location = Point.x / tile_size;

//    if (Point.x % tile_size)
//    {
//        horizontal_location += 1;
//    }

    uint16_t vertical_location = (Point.y / tile_size) * tile_map_width;

    if (vertical_location % tile_size > 0)
    {
        vertical_location += 1;
    }

    const uint16_t array_location = horizontal_location + vertical_location;

    return array_location;
}

TileData getLocalTileData(const Point& Point_to_check, uint8_t tile_size, uint8_t tile_map_width)
{
    TileData tileData;

    for (auto y = 0; y < sprite_width; y++)
    {
        for (auto x = 0; x < sprite_width; x++)
        {
            const auto array_location = getTileFromPoint(Point(Point_to_check.x + x, Point_to_check.y + y), tile_size, tile_map_width);
            const uint8_t tileScanned = layer_world[array_location];

            tileData.id = tileScanned;
            tileData.index = array_location;
        	
            switch (tileScanned)
            {
				case 17:
				case 33:
	            case 48:
	            case 49:
	            case 50:
	            case 51:
	            case 52:
                case 64:
                case 68:
                case 80:
                case 81:
                case 82:
                case 83:
                case 84:
                case 36:
                case 01:
                    tileData.canMove = false;
            	break;
                default:
            	break;
            }
        	
            //if (tile_scanned == 0)
            //{
            //    tile_data.canMove = false;
            //    //return false;
            //}
            //else if (tile_scanned == 4)
            //{
            //    tile_data.movement_modifier = 0.5;
            //    tile_data.pixels_in_water += 1;
            //}

        }
    }

    if (tileData.pixels_in_water > (sprite_width * sprite_width / 2))
    {
        tileData.in_water = true;
    }

    return tileData;
}

void DrawWorld()
{
    Vec2 wo(64, 40);

    world.transform =
        Mat3::identity() *
        Mat3::translation(wo) *
        Mat3::scale(Vec2(0.5, 0.5)) *
        Mat3::translation(Vec2(-128, -80));

    world.draw(&screen, Rect(0, 0, 320, 240), nullptr);

}


void StartGame()
{
    score = 0;
    gameState = 'G';
}

void EndGame()
{
    gameState = 'E';
}

void InitPlayers()
{
	players.clear();
	
	players.push_back(new Player());

    auto player2 = new AIPlayer();
    player2->spawnLocation = Point(288, 16);
    player2->location = player2->spawnLocation;
    player2->isPlayer = false;
    player2->spriteDown = ninjaSpriteDown;
    player2->spriteUp = ninjaSpriteUp;
    player2->spriteSide = ninjaSpriteSide;
    player2->movementType = LeftRight;
    players.push_back(player2);

    auto player3 = new AIPlayer();
    player3->spawnLocation = Point(16, 208);
    player3->location = player3->spawnLocation;
    player3->isPlayer = false;
    player3->spriteDown = p3SpriteDown;
    player3->spriteUp = p3SpriteUp;
    player3->spriteSide = p3SpriteSide;
    players.push_back(player3);

    auto player4 = new AIPlayer();
    player4->spawnLocation = Point(288, 208);
    player4->location = player4->spawnLocation;
    player4->isPlayer = false;
    player4->spriteDown = p4SpriteDown;
    player4->spriteUp = p4SpriteUp;
    player4->spriteSide = p4SpriteSide;
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

void RenderTileAnimations()
{	
    auto tile = tileAnimations.begin();

    while (tile != tileAnimations.end()) {

        bool complete = false;
    	
	    const auto currentFrame = tile->animationFrames[tile->frameIndex];
    	
        layer_world[tile->tileIndex] = currentFrame.tileId;

    	if(currentFrame.frameCount >= currentFrame.frameLength)
    	{
    		if(tile->frameIndex >= tile->animationFrames.size() - 1)
    		{  			
                complete = true;
    		}
            else
            {
                tile->frameIndex++;
            }
    	}
    	
        tile->animationFrames[tile->frameIndex].frameCount++;

        /*auto hit = is_Point_in_Rect(projectile->location, Rect(npc->location, Size(sprite_width, sprite_width)));*/
        if (complete)
        {
            tile = tileAnimations.erase(tile);

            //todo temp hack
            //player_world[tile->tileIndex] = 66;
            //projectile->lifetime = 0;
        }
        else ++tile;
        }
}

void RenderExplosions()
{
	for (const Explosion& explosion : explosions)
	{
		screen.sprite(explosionCore, explosion.location, Point(0, 0), Vec2(2, 2));

		for (int i = 1; i <= explosion.lengthRight; i++)
		{
			if(i < explosion.lengthRight)
			{
                screen.sprite(explosionLine, Point(explosion.location.x + (i * sprite_width), explosion.location.y), Point(0, 0), Vec2(2, 2));
			}
            else
            {
                screen.sprite(explosionEnd, Point(explosion.location.x + (i * sprite_width), explosion.location.y), Point(0, 0), Vec2(2, 2));
            }
		}

		for (int i = 1; i <= explosion.lengthLeft; i++)
		{
            if (i < explosion.lengthLeft)
            {
                screen.sprite(explosionLine, Point(explosion.location.x - (i * sprite_width), explosion.location.y), Point(0, 0), Vec2(2, 2));
            }
            else
            {
                screen.sprite(explosionEnd, Point(explosion.location.x - (i * sprite_width), explosion.location.y), Point(0, 0), Vec2(2, 2), SpriteTransform::R180);

            }
		}

		for (int i = 1; i <= explosion.lengthDown; i++)
		{
            if (i < explosion.lengthDown)
            {
                screen.sprite(explosionLine, Point(explosion.location.x, explosion.location.y + (i * sprite_width)), Point(0, 0), Vec2(2, 2), SpriteTransform::R270);
            }
            else
            {
                screen.sprite(explosionEnd, Point(explosion.location.x, explosion.location.y + (i * sprite_width)), Point(0, 0), Vec2(2, 2), SpriteTransform::R90);
            }
		}

		for (int i = 1; i <= explosion.lengthUp; i++)
		{
            if (i < explosion.lengthUp)
            {
                screen.sprite(explosionLine, Point(explosion.location.x, explosion.location.y - (i * sprite_width)), Point(0, 0), Vec2(2, 2), SpriteTransform::R270);
            }
            else
            {
                screen.sprite(explosionEnd, Point(explosion.location.x, explosion.location.y - (i * sprite_width)), Point(0, 0), Vec2(2, 2), SpriteTransform::R270);
            }
		}
	}
}

void RenderPlayers()
{
    auto p = players.begin();

    while (p != players.end()) {

    	
        if((*p)->alive)
        {
            if ((*p)->dir == 'r')
            {
                screen.sprite((*p)->spriteSide, (*p)->location, Point(0, 8), Vec2(2, 2));
            }
            else if ((*p)->dir == 'l')
            {
                screen.sprite((*p)->spriteSide, (*p)->location, Point(0, 8), Vec2(2, 2), 1);
            }
            else if ((*p)->dir == 'u')
            {
                screen.sprite((*p)->spriteUp, (*p)->location, Point(0, 8), Vec2(2, 2));

            }
            else if ((*p)->dir == 'd')
            {
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

        for (const Projectile& projectile : projectiles)
        {
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
    }
    else if (gameState == 'E')
    {

        screen.text("Game Over", minimal_font, Point(maxX /2, 120),true, center_h);


        screen.text("Score ", minimal_font, Point(maxX /2, 130),true, center_h);

       // char scoreBuff[5];
        //sprintf_s(scoreBuff, "%05d", score);

        screen.text(std::to_string(score), minimal_font, Point(maxX /2, 140),true, center_h);

    }
    else if (gameState == 'T')
    {

        screen.text("Snack", minimal_font, Point(maxX /2, 1),true, center_h);



        screen.text("By Shane Powell", minimal_font, Point(maxX / 2, 15), true, center_h);



        screen.text("Sound ", minimal_font, Point(50, 55));


        if (sound == true)
        {
            screen.pen = Pen(255, 255, 255);
            screen.rectangle(Rect(88, 53, 18, 11));
            screen.pen = Pen(0, 0, 0);
            screen.rectangle(Rect(89, 54, 16, 9));
        }
        else
        {
            screen.pen = Pen(255, 255, 255);
            screen.rectangle(Rect(107, 53, 21, 11));
            screen.pen = Pen(0, 0, 0);
            screen.rectangle(Rect(108, 54, 19, 9));
        }
        screen.pen = Pen(255, 255, 255);


        screen.text("On ", minimal_font, Point(90, 55));

        screen.text("Off ", minimal_font, Point(109, 55));
    }

    screen.pen = Pen(0,0, 0);
	
    
}

void CreateChestExplosion(const TileData tileData)
{	
    for (const TileAnimation& ti : tileAnimations)
    {
    	if(ti.tileIndex == tileData.index)
    	{
            return;
    	}
    }


	TileAnimation animation = TileAnimation(tileData.index);
	animation.animationFrames.emplace_back(17, 10);
	animation.animationFrames.emplace_back(33, 10);
    animation.animationFrames.emplace_back(66, 1);

    tileAnimations.push_back(animation);
}

void CheckExplosionTileData(bool& canMove, const Point& point)
{
	const auto tileData = getLocalTileData(point, sprite_width, tilemap_width);
	canMove = tileData.canMove;
		
	if(tileData.id == 1)
	{
		layer_world[tileData.index] = 66;

        CreateChestExplosion(tileData);
	}
}


void CheckIfExplosionHitPlayer(bool& canMove, const Point& point)
{
    auto p = players.begin();

    while (p != players.end()) {
	    const auto hit = is_Point_in_Rect(point, Rect((*p)->location, Size(16, 16)));

	    if((*p)->alive && hit)
        {
            (*p)->alive = false;
        }
        ++p;
    }
}


void PerformCollisionChecks(bool& canMove, const Point& point)
{
	CheckExplosionTileData(canMove, point);
	if(canMove)
	{
        CheckIfExplosionHitPlayer(canMove, point);
	}
}

void CreateExplosion(const std::vector<Projectile>::iterator& projectile)
{
	Explosion explosion;
	explosion.location = projectile->location;

	bool canMove = true;

	while (canMove)
	{
		Point point = Point(explosion.location.x + (sprite_width * (explosion.lengthRight + 1)),explosion.location.y);
		
		PerformCollisionChecks(canMove, point);
		
		if (canMove)
		{
			explosion.lengthRight++;
		}
	}

	canMove = true;

	while (canMove)
	{
		Point point = Point(explosion.location.x - (sprite_width * (explosion.lengthLeft + 1)), explosion.location.y);
        PerformCollisionChecks(canMove, point);

		if (canMove)
		{
			explosion.lengthLeft++;
		}
	}

	canMove = true;

	while (canMove)
	{
		Point point = Point(explosion.location.x, explosion.location.y + (sprite_width * (explosion.lengthDown + 1)));
        PerformCollisionChecks(canMove, point);

		if (canMove)
		{
			explosion.lengthDown ++;
		}
	}

	canMove = true;

	while (canMove)
	{
		Point point = Point(explosion.location.x, explosion.location.y - (sprite_width * (explosion.lengthUp + 1)));
        PerformCollisionChecks(canMove, point);

		if (canMove)
		{
			explosion.lengthUp++;
		}
	}            

        	          
	explosions.push_back(explosion);
}

void updateProjectiles()
{
    auto projectile = projectiles.begin();

    while (projectile != projectiles.end()) {

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

        if (projectile->lifetime <= 0)
        {
            CreateExplosion(projectile);

            projectile = projectiles.erase(projectile);
        	
        }
        else ++projectile;
    }

    auto explosion = explosions.begin();

    while (explosion != explosions.end())
    {
        explosion->lifeTime--;
	    if (explosion->lifeTime <= 0)
	    {
            explosion = explosions.erase(explosion);
	    }
        else
        {
            ++explosion;
        }
    }
}

void UpdatePlayers()
{
    static uint16_t lastButtons = 0;
    uint16_t changed = blit::buttons ^ lastButtons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    for (Player* player : players)
    {
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