#include "dungeon-tile.hpp"
#include "../../../32blit/engine/engine.hpp"
#include "../../../32blit/graphics/font.hpp"

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

const Rect skeleton_sprite = Rect(0, 2, 1, 1);

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
    bool can_move = true;
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
    int16_t can_fire_timeout = 0;
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
};

static std::vector<Npc> npcs;

struct Projectile
{
    Rect sprite;
    Point location;
    uint8_t transform;
    int16_t lifetime = 500;
    int16_t vel_x;
    int16_t vel_y;
};

static std::vector<Projectile> projectiles;

Player player;

Tile_Data current_tile_data;

void initNpcs()
{
    Npc npc;
    npc.location = Point(100,100);
    npcs.push_back(npc);
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
        	if(tile_scanned == 0)
        	{
                tile_data.can_move = false;
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

    screen.sprites = SpriteSheet::load(packed_data);

    world.sprites = screen.sprites;

    initNpcs();
	
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
    screen.pen(RGBA(0, 0, 0));
	
    // clear the screen -- fb is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();

    screen.alpha = 255;
    screen.mask = nullptr;

    Vec2 wo(64, 40);

    world.transform =
        Mat3::identity() *
        Mat3::translation(wo) *
        Mat3::scale(Vec2(0.5, 0.5)) *
        Mat3::translation(Vec2(-128, -80));
	
    world.draw(&screen, Rect(0, 0, 320, 240), nullptr);
   
    screen.sprite(key_sprite, Point(16, 16), Point(0,0), Vec2(2,2));

	if(current_tile_data.in_water)
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

    screen.pen(RGBA(255, 255, 255));
    screen.text(tile_name, &minimal_font[0][0], Point(0, 0));

}



///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
	
	auto projectile = projectiles.begin();

    while (projectile != projectiles.end()) {
    	
        projectile->lifetime--;
        projectile->location.x += projectile->vel_x;
        projectile->location.y += projectile->vel_y;

        auto projTileData = getLocalTileData(projectile->location, sprite_width, tilemap_width);

        if (!projTileData.can_move || projectile->lifetime == 0)
        {
            projectile = projectiles.erase(projectile);
        }
        else ++projectile;

        
    }
	
	/*for (auto& projectile : projectiles)
	{
        projectile.lifetime--;
        projectile.location.x += projectile.vel_x;
        projectile.location.y += projectile.vel_y;

        auto projTileData = getLocalTileData(projectile.location, sprite_width, tilemap_width);

		if(!projTileData.can_move || projectile.lifetime == 0)
		{
            
		}
	}*/
	
    static uint16_t last_Buttons = 0;
    uint16_t changed = blit::buttons ^ last_Buttons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    int16_t x_change = 0;
    int16_t y_change = 0;	
	
    Point new_player_location = player.location;

	if(player.can_fire_timeout > 0)
	{
        player.can_fire_timeout--;
	}
    else
    {
        player.can_fire = true;
    }
	
    if (blit::buttons & blit::Button::DPAD_LEFT || joystick.x < 0) {
        x_change -= 1;
        new_player_location.x -= 1;
    }
    if (blit::buttons & blit::Button::DPAD_RIGHT || joystick.x > 0) {
        x_change += 1;
        new_player_location.x += 1;
    }
    if (blit::buttons & blit::Button::DPAD_UP || joystick.y < 0) {
        y_change -= 1;
        new_player_location.y -= 1;
    }
    if (blit::buttons & blit::Button::DPAD_DOWN || joystick.y > 0) {
        y_change += 1;
        new_player_location.y += 1;
    }
	if(blit::buttons & blit::Button::B)
	{
		if(player.can_fire)
		{
            player.can_fire = false;
            player.can_fire_timeout = player.fire_delay;
            Projectile new_projectile;

            switch(player.aim)
            {
            case 1:
                new_projectile.vel_x = -1;
                new_projectile.vel_y = 1;
                
                break;
            case 2:
                new_projectile.vel_x = 0;
                new_projectile.vel_y = 1;
                break;
            case 3:
                new_projectile.vel_x = 1;
                new_projectile.vel_y = 1;
                new_projectile.transform = SpriteTransform ::VERTICAL;
                break;
            case 4:
                new_projectile.vel_x = -1;
                new_projectile.vel_y = 0;
                new_projectile.transform = SpriteTransform::R90;
                break;
            case 6:
                new_projectile.vel_x = 1;
                new_projectile.vel_y = 0;
                new_projectile.transform = SpriteTransform::R90;
                break;
            case 7:
                new_projectile.vel_x = -1;
                new_projectile.vel_y = -1;
                new_projectile.transform = SpriteTransform::VERTICAL;
                break;
            case 8:
                new_projectile.vel_x = 0;
                new_projectile.vel_y = -1;
                break;
            case 9:
                new_projectile.vel_x = 1;
                new_projectile.vel_y = -1;
                
                break;
            default: break;
            }

            if (new_projectile.vel_x == 0 || new_projectile.vel_y == 0)
            {
                new_projectile.sprite = proj_2;
            }
            else
            {
                new_projectile.sprite = proj_2_d;
            }
/*            new_projectile.location.x = player.location. x + sprite_width / 4;
            new_projectile.location.y = player.location.y + sprite_width / 4;*/

            new_projectile.location.x = player.location.x;
            new_projectile.location.y = player.location.y;

            projectiles.push_back(new_projectile);
		}      
	}

    bool move_ok = true;

    current_tile_data = getLocalTileData(new_player_location, sprite_width, tilemap_width);

	if(x_change != 0 || y_change != 0)
	{
        if (current_tile_data.can_move)
        {
            player.location.x += (x_change);
            player.location.y += (y_change);

            if (y_change > 0 && x_change == 0)
            {
                player.aim = 2;
            }
            else if (y_change < 0 && x_change == 0)
            {
                player.aim = 8;
            }
            else if (x_change > 0 && y_change == 0)
            {
                player.aim = 6;
            }
            else if (x_change < 0 && y_change == 0)
            {
                player.aim = 4;
            }
            else if(x_change > 0 && y_change > 0)
            {
                player.aim = 3;
            }
            else if (x_change < 0 && y_change < 0)
            {
                player.aim = 7;
            }
            else if (x_change > 0 && y_change < 0)
            {
                player.aim = 9;
            }
            else if (x_change < 0 && y_change > 0)
            {
                player.aim = 1;
            }
        }

        if (x_change > 0)
        {
            player.dir = 'r';
        }
        else if (x_change < 0)
        {
            player.dir = 'l';
        }
	}
	
	
    last_Buttons = blit::buttons;
}