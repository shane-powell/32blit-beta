#include "powder-keg.hpp"

using namespace blit;

static uint8_t layer_world[] = {
  48, 50, 51, 50, 49, 50, 51, 50, 49, 51, 50, 51, 50, 49, 51, 50, 51, 49, 50, 52, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 52,
  64, 17, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  64, 65, 65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 65, 66, 35, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 65, 65, 66, 66, 65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 65, 65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 65, 66, 66, 66, 65, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 66, 66, 66, 66, 33, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 65, 66, 66, 65, 66, 66, 65, 66, 66, 65, 66, 65, 66, 66, 66, 66, 66, 66, 68, 65, 65, 66, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  64, 66, 66, 66, 35, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 65, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 65, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 65, 65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 33, 66, 66, 65, 65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 17, 66, 66, 68, 65, 65, 65, 65, 66, 65, 65, 65, 65, 65, 65, 65,
  64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 65, 65, 65, 65, 66, 65, 65, 65, 65, 65, 65, 65,
  80, 81, 82, 83, 82, 81, 82, 83, 81, 82, 83, 81, 82, 83, 81, 82, 83, 81, 82, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const uint8_t sprite_width = 16;

// Point player_location = Point(32, 20);

const uint32_t tilemap_width = 32;

const uint32_t tilemap_height = 16;

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

int8_t spriteSize = 16;

int viewX = 0;

//int8_t borderOffset = 2;

int viewPortX = 0;

struct Movement
{
    uint8_t xMovement = 0;
    uint8_t yMovement = 0;
    int8_t movementCount = 0;
    int8_t movementDelay = 20;

};

struct Player
{
    Rect sprite = playerSprite;
    char dir = 'r';
    int8_t aim = 6;
    Point location = Point(16, 16);
    bool can_fire = true;
    int16_t canFireTimeout = 0;
    int16_t fire_delay = 20;
    Movement currentMovement;
};

struct Tile_Data
{
    bool canMove = true;
    uint16_t pixels_in_water = 0;
    bool in_water = false;
    float movement_modifier = 0;
    float life_modifier = 0;
};

Player player;

Tile_Data currentTileData;


int score = 0;

bool is_Point_in_Rect(const Point& object_origin, std::vector<Rect>::value_type bounding_Rectangle)
{
    if (object_origin.x + sprite_width >= bounding_Rectangle.x && object_origin.x <= bounding_Rectangle.x + bounding_Rectangle.w && object_origin.y + sprite_width > bounding_Rectangle.y&& object_origin.y < bounding_Rectangle.y + bounding_Rectangle.h)
    {
        return true;
    }

    return false;
}

uint16_t get_tile_from_Point(const Point& Point, uint8_t tile_size, uint8_t tile_map_width)
{
    uint16_t horizontal_location = Point.x / tile_size;

    if (Point.x % tile_size > 0)
    {
        horizontal_location += 1;
    }

    uint16_t vertical_location = (Point.y / tile_size) * tile_map_width;

    if (vertical_location % tile_size > 0)
    {
        vertical_location += 1;
    }

    const uint16_t array_location = horizontal_location + vertical_location - 1;

    return array_location;
}

Tile_Data getLocalTileData(const Point& Point_to_check, uint8_t tile_size, uint8_t tile_map_width)
{
    Tile_Data tile_data;

    for (auto y = 0; y < sprite_width; y++)
    {
        for (auto x = 0; x < sprite_width; x++)
        {
            const auto array_location = get_tile_from_Point(Point(Point_to_check.x + x, Point_to_check.y + y), tile_size, tile_map_width);
            const uint8_t tile_scanned = layer_world[array_location];
            if (tile_scanned == 0)
            {
                tile_data.canMove = false;
                //return false;
            }
            else if (tile_scanned == 4)
            {
                tile_data.movement_modifier = 0.5;
                tile_data.pixels_in_water += 1;
            }

        }
    }

    if (tile_data.pixels_in_water > (sprite_width * sprite_width / 2))
    {
        tile_data.in_water = true;
    }

    return tile_data;
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
    player.location.x = maxX / 2;
    player.location.y = (maxY / 2) + 10;
    score = 0;
    gameState = 'G';
}

void EndGame()
{
    //if (sound == true)
    //{
    //    arduboy.tunes.tone(200, 200);
    //    delay(400);
    //    arduboy.tunes.tone(100, 400);
    //    delay(600);
    //    arduboy.tunes.tone(50, 2000);
    //}

    gameState = 'E';
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
        //DrawFrame();
        DrawWorld();
    	
        screen.pen = Pen(255, 255, 0);

        //screen.pixel(player.location);
        screen.sprite(player.sprite, player.location,Point(0, 0), Vec2(2, 2));

        if (player.dir == 'R')
        {
            screen.sprite(player.sprite, player.location, Point(0, 0), Vec2(2, 2));
        }
        else
        {
            screen.sprite(player.sprite, player.location, Point(0, 0), Vec2(2, 2), 1);
        }
    
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

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {

    /*updateProjectiles();
    updateNpcs();*/

    static uint16_t lastButtons = 0;
    uint16_t changed = blit::buttons ^ lastButtons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    int16_t xChange = 0;
    int16_t yChange = 0;

    Point newPlayerLocation = player.location;

    if (player.canFireTimeout > 0)
    {
        player.canFireTimeout--;
    }
    else
    {
        player.can_fire = true;
    }

    if (blit::buttons & blit::Button::DPAD_LEFT || joystick.x < 0) {
        xChange -= 1;
        newPlayerLocation.x -= 1;
    }
    if (blit::buttons & blit::Button::DPAD_RIGHT || joystick.x > 0) {
        xChange += 1;
        newPlayerLocation.x += 1;
    }
    if (blit::buttons & blit::Button::DPAD_UP || joystick.y < 0) {
        yChange -= 1;
        newPlayerLocation.y -= 1;
    }
    if (blit::buttons & blit::Button::DPAD_DOWN || joystick.y > 0) {
        yChange += 1;
        newPlayerLocation.y += 1;
    }
    if (blit::buttons & blit::Button::B)
    {
        if (player.can_fire)
        {
            //player.can_fire = false;
            //player.canFireTimeout = player.fire_delay;
            //Projectile newProjectile;

            //switch (player.aim)
            //{
            //case 1:
            //    newProjectile.vel_x = -1;
            //    newProjectile.vel_y = 1;

            //    break;
            //case 2:
            //    newProjectile.vel_x = 0;
            //    newProjectile.vel_y = 1;
            //    break;
            //case 3:
            //    newProjectile.vel_x = 1;
            //    newProjectile.vel_y = 1;
            //    newProjectile.transform = SpriteTransform::VERTICAL;
            //    break;
            //case 4:
            //    newProjectile.vel_x = -1;
            //    newProjectile.vel_y = 0;
            //    newProjectile.transform = SpriteTransform::R90;
            //    break;
            //case 6:
            //    newProjectile.vel_x = 1;
            //    newProjectile.vel_y = 0;
            //    newProjectile.transform = SpriteTransform::R90;
            //    break;
            //case 7:
            //    newProjectile.vel_x = -1;
            //    newProjectile.vel_y = -1;
            //    newProjectile.transform = SpriteTransform::VERTICAL;
            //    break;
            //case 8:
            //    newProjectile.vel_x = 0;
            //    newProjectile.vel_y = -1;
            //    break;
            //case 9:
            //    newProjectile.vel_x = 1;
            //    newProjectile.vel_y = -1;

            //    break;
            //default: break;
            //}

            //if (newProjectile.vel_x == 0 || newProjectile.vel_y == 0)
            //{
            //    newProjectile.sprite = proj_2;
            //}
            //else
            //{
            //    newProjectile.sprite = proj_2_d;
            //}
            ///*            new_projectile.location.x = player.location. x + sprite_width / 4;
            //            new_projectile.location.y = player.location.y + sprite_width / 4;*/

            //newProjectile.location.x = player.location.x;
            //newProjectile.location.y = player.location.y;

            //projectiles.push_back(newProjectile);
        }
    }

    bool move_ok = true;

    currentTileData = getLocalTileData(newPlayerLocation, sprite_width, tilemap_width);

    if (xChange != 0 || yChange != 0)
    {
        if (currentTileData.canMove)
        {
            player.location.x += (xChange);
            player.location.y += (yChange);

            if (yChange > 0 && xChange == 0)
            {
                player.aim = 2;
            }
            else if (yChange < 0 && xChange == 0)
            {
                player.aim = 8;
            }
            else if (xChange > 0 && yChange == 0)
            {
                player.aim = 6;
            }
            else if (xChange < 0 && yChange == 0)
            {
                player.aim = 4;
            }
            else if (xChange > 0 && yChange > 0)
            {
                player.aim = 3;
            }
            else if (xChange < 0 && yChange < 0)
            {
                player.aim = 7;
            }
            else if (xChange > 0 && yChange < 0)
            {
                player.aim = 9;
            }
            else if (xChange < 0 && yChange > 0)
            {
                player.aim = 1;
            }
        }

        if (xChange > 0)
        {
            player.dir = 'r';
        }
        else if (xChange < 0)
        {
            player.dir = 'l';
        }
    }


    lastButtons = blit::buttons;
}