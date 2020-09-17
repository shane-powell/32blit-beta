#include "dungeon-tile.hpp"
#include "../32blit/engine/engine.hpp"
#include "../32blit/graphics/font.hpp"
#include "../32blit/graphics/tilemap.hpp"
//#include "assets.hpp"
//#include <thread>

using namespace blit;

static uint8_t layer_world[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const Rect key_sprite = Rect(0, 2, 1, 1);

const Rect player_sprite = Rect(0, 1, 1, 1);

const Rect player_swim_sprite = Rect(1, 1, 1, 1);

const Rect skeleton_sprite = Rect(0, 3, 1, 1);

const Rect proj_1 = Rect(1, 1, 1, 1);

const Rect proj_1_d = Rect(2, 1, 1, 1);

const Rect proj_2 = Rect(3, 2, 1, 1);

const Rect proj_2_d = Rect(4, 2, 1, 1);

const uint8_t sprite_width = 16;

// Point player_location = Point(32, 20);

const uint32_t tilemap_width = 32;

const uint32_t tilemap_height = 16;

TileMap world((uint8_t*)layer_world, nullptr, Size(tilemap_width, tilemap_height), nullptr);

std::vector<Rect> bounding_Rectangles = {Rect(0,0,  32 * 16, 16), Rect(0, 0, 16, 20 * 16), Rect(5 * 16, 3 * 16,2 * 16, 2 * 16), Rect(5 * 16, 11 * 16, 2 * 16, 2 * 16) };

std::string tile_name;

struct Tile_Data
{
    bool canMove = true;
    uint16_t pixels_in_water = 0;
    bool in_water = false;
    float movement_modifier = 0;
    float life_modifier = 0;
};

struct Player
{
    Rect sprite = player_sprite;
    char dir = 'r';
    int8_t aim = 6;
	Point location = Point(32, 20);
    bool can_fire = true;
    int16_t canFireTimeout = 0;
    int16_t fire_delay = 20;
};

struct Npc
{
    Rect sprite = skeleton_sprite;
    char dir = 'r';
    Point location;
    bool can_fire = true;
    int16_t can_fire_timeout = 0;
    int16_t fire_delay = 20;
    uint8_t moveCounter= 0;
    uint8_t moveCountTrigger = 3;
};

static std::vector<Npc> npcs;

struct Projectile
{
    Rect sprite;
    Point location;
    uint8_t transform = 0;
    int16_t lifetime = 500;
    int16_t vel_x = 0;
    int16_t vel_y = 0;
};

std::vector<Npc>::iterator &moveNpc(std::vector<Npc>::iterator &npc);

static std::vector<Projectile> projectiles;

Player player;

Tile_Data currentTileData;

void initNpcs()
{
    Npc npc;
    npc.location = Point(150,150);
    npcs.push_back(npc);
}

uint16_t get_tile_from_Point(const Point& Point, uint8_t tile_size, uint8_t tile_map_width)
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

Tile_Data getLocalTileData(const Point& Point_to_check, uint8_t tile_size, uint8_t tile_map_width)
{
    Tile_Data tile_data;
	
    for (auto y = 0; y < sprite_width; y++)
    {
        for (auto x = 0; x < sprite_width; x++)
        {
	        const auto array_location = get_tile_from_Point(Point(Point_to_check.x + x, Point_to_check.y + y), tile_size, tile_map_width);
	        const uint8_t tile_scanned = layer_world[array_location];
        	if(tile_scanned == 0)
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

	if(tile_data.pixels_in_water > (sprite_width * sprite_width / 2))
	{
        tile_data.in_water = true;
	}

    return tile_data;
}

bool is_Point_in_Rect(const Point& object_origin, std::vector<Rect>::value_type bounding_Rectangle)
{
    if (object_origin.x + sprite_width >= bounding_Rectangle.x && object_origin.x <= bounding_Rectangle.x + bounding_Rectangle.w && object_origin.y + sprite_width > bounding_Rectangle.y&& object_origin.y < bounding_Rectangle.y + bounding_Rectangle.h)
    {
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::hires);

    //screen.sprites = SpriteSheet::load(sprites_data);

    world.sprites = screen.sprites;

    initNpcs();

    //std::chrono::milliseconds timespan(5000);
    //std::this_thread::sleep_for(timespan);
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

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    screen.alpha = 255;
    screen.pen = Pen(0, 0, 0, 0);
	
    // clear the screen -- fb is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();

    screen.alpha = 255;
    screen.mask = nullptr;

    DrawWorld();

    screen.sprite(key_sprite, Point(16, 16), Point(0, 0), Vec2(2, 2));


	if(currentTileData.in_water)
	{
        screen.sprite(player_swim_sprite, player.location, Point(0, 0), Vec2(2, 2));

	}
    else
    {
    	if(player.dir == 'r')
    	{
            screen.sprite(player.sprite, player.location, Point(0, 0), Vec2(2, 2));
    	}
        else
        {
            screen.sprite(player.sprite, player.location, Point(0, 0), Vec2(2, 2), 1);
        }
    }

    for (const Projectile& projectile : projectiles)
    {
        screen.sprite(projectile.sprite, projectile.location, Point(0, 0),Vec2(2,2), projectile.transform);
    }

    for (const Npc& npc : npcs)
    {
        screen.sprite(npc.sprite, npc.location, Point(0, 0), Vec2(2, 2));
    }

    screen.pen = Pen(255, 255, 255, 0);
    screen.text(tile_name, minimal_font, Point(0, 0));

}

void spawnNpc()
{
    auto locX = 0;
    auto locY = 0;

    bool canSpawn = false;

    while(canSpawn == false)
    {
        locX = blit::random() % tilemap_width * 16;
        locY = blit::random() % tilemap_height * 16;
        Tile_Data spawnTile = getLocalTileData(Point(locX,locY), sprite_width, tilemap_width);

        canSpawn = spawnTile.canMove;
    }

    Npc npc;
    npc.location = Point(locX,locY);
    npcs.push_back(npc);
}

void updateProjectiles()
{
	auto projectile = projectiles.begin();

	while (projectile != projectiles.end()) {

        auto npc = npcs.begin();

        while (npc != npcs.end()) {
         auto hit = is_Point_in_Rect(projectile->location, Rect(npc->location, Size(sprite_width,sprite_width)));
         if(hit)
         {
             npc = npcs.erase(npc);
             projectile->lifetime = 0;
         }
         else ++npc;
        }

		projectile->lifetime--;
		projectile->location.x += projectile->vel_x;
		projectile->location.y += projectile->vel_y;

		auto projTileData = getLocalTileData(projectile->location, sprite_width, tilemap_width);
		
		if (!projTileData.canMove || projectile->lifetime == 0)
		{
			projectile = projectiles.erase(projectile);
		}
		else ++projectile;
	}
}

void updateNpcs()
{
    spawnNpc();

    auto npc = npcs.begin();

    while (npc != npcs.end()) {

        if(npc->moveCounter < npc->moveCountTrigger)
        {
            npc->moveCounter ++;
        } else
        {
            npc = moveNpc(npc);
            npc->moveCounter = 0;
        }



        // todo Update this so npcs can die.
        if (false)
        {
            npc = npcs.erase(npc);
        }
        else ++npc;


    }
}

std::vector<Npc>::iterator &moveNpc(std::vector<Npc>::iterator &npc) {
    Point newNpcLocation = npc->location;

    int8_t xMov = 0;
    int8_t yMov = 0;


    if(npc->location.x < player.location.x)
    {
xMov = 1;
    }
else if(npc->location.x > player.location.x)
{
xMov = -1;
}

    if (npc->location.y < player.location.y)
    {
        yMov = 1;
    }
    else if (npc->location.y > player.location.y)
    {
        yMov = -1;
    }
// todo Tidy up and work out how to set an offset target if main target is not directly reachable.
// Plot route to target. Recalculate every time.
//Set maximum chase distance.
    newNpcLocation.x += xMov;
    newNpcLocation.y += yMov;

    auto npcTileData = getLocalTileData(newNpcLocation, sprite_width, tilemap_width);

    if(!npcTileData.canMove)
    {
        newNpcLocation = npc->location;
        newNpcLocation.x += xMov;

        npcTileData = getLocalTileData(newNpcLocation, sprite_width, tilemap_width);

        if(!npcTileData.canMove || xMov == 0)
        {
            newNpcLocation = npc->location;
            newNpcLocation.y += yMov;

            npcTileData = getLocalTileData(newNpcLocation, sprite_width, tilemap_width);

            if(!npcTileData.canMove || yMov == 0)
            {
                newNpcLocation = npc->location;
                newNpcLocation.y += xMov;
                newNpcLocation.x += yMov;

                npcTileData = getLocalTileData(newNpcLocation, sprite_width, tilemap_width);
            }

            if (!npcTileData.canMove)
            {
                newNpcLocation = npc->location;
                newNpcLocation.y += xMov;

                npcTileData = getLocalTileData(newNpcLocation, sprite_width, tilemap_width);

                if (!npcTileData.canMove)
                {
                    newNpcLocation = npc->location;
                    newNpcLocation.x += yMov;

                    npcTileData = getLocalTileData(newNpcLocation, sprite_width, tilemap_width);
                }
            }
        }
    }


    if(npcTileData.canMove)
    {
npc->location = newNpcLocation;
    }
    return npc;
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
    updateNpcs();
	
    static uint16_t lastButtons = 0;
    uint16_t changed = blit::buttons ^ lastButtons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    int16_t xChange = 0;
    int16_t yChange = 0;	
	
    Point newPlayerLocation = player.location;

	if(player.canFireTimeout > 0)
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
	if(blit::buttons & blit::Button::B)
	{
		if(player.can_fire)
		{
            player.can_fire = false;
            player.canFireTimeout = player.fire_delay;
            Projectile newProjectile;

            switch(player.aim)
            {
            case 1:
                newProjectile.vel_x = -1;
                newProjectile.vel_y = 1;
                
                break;
            case 2:
                newProjectile.vel_x = 0;
                newProjectile.vel_y = 1;
                break;
            case 3:
                newProjectile.vel_x = 1;
                newProjectile.vel_y = 1;
                newProjectile.transform = SpriteTransform ::VERTICAL;
                break;
            case 4:
                newProjectile.vel_x = -1;
                newProjectile.vel_y = 0;
                newProjectile.transform = SpriteTransform::R90;
                break;
            case 6:
                newProjectile.vel_x = 1;
                newProjectile.vel_y = 0;
                newProjectile.transform = SpriteTransform::R90;
                break;
            case 7:
                newProjectile.vel_x = -1;
                newProjectile.vel_y = -1;
                newProjectile.transform = SpriteTransform::VERTICAL;
                break;
            case 8:
                newProjectile.vel_x = 0;
                newProjectile.vel_y = -1;
                break;
            case 9:
                newProjectile.vel_x = 1;
                newProjectile.vel_y = -1;
                
                break;
            default: break;
            }

            if (newProjectile.vel_x == 0 || newProjectile.vel_y == 0)
            {
                newProjectile.sprite = proj_2;
            }
            else
            {
                newProjectile.sprite = proj_2_d;
            }
/*            new_projectile.location.x = player.location. x + sprite_width / 4;
            new_projectile.location.y = player.location.y + sprite_width / 4;*/

            newProjectile.location.x = player.location.x;
            newProjectile.location.y = player.location.y;

            projectiles.push_back(newProjectile);
		}      
	}

    bool move_ok = true;

    currentTileData = getLocalTileData(newPlayerLocation, sprite_width, tilemap_width);

	if(xChange != 0 || yChange != 0)
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
            else if(xChange > 0 && yChange > 0)
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