#include "dungeon-tile.hpp"
#include "../../../32blit/engine/engine.hpp"
#include "../../../32blit/graphics/font.hpp"

using namespace blit;

// 16 wide example
//static uint8_t layer_world[] = {
//  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//};

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

const rect key_sprite = rect(0, 2, 1, 1);

const rect player_sprite = rect(0, 1, 1, 1);

const rect player_swim_sprite = rect(1, 1, 1, 1);

const rect skeleton_sprite = rect(0, 2, 1, 1);

const rect proj_1 = rect(1, 1, 1, 1);

const rect proj_1_d = rect(2, 1, 1, 1);

const rect proj_2 = rect(3, 2, 1, 1);

const rect proj_2_d = rect(4, 2, 1, 1);

const uint8_t sprite_width = 16;

// point player_location = point(32, 20);

const uint32_t tilemap_width = 32;

const uint32_t tilemap_height = 16;

tilemap world((uint8_t*)layer_world, nullptr, size(tilemap_width, tilemap_height), nullptr);

std::vector<rect> bounding_rectangles = {rect(0,0,  32 * 16, 16), rect(0, 0, 16, 20 * 16), rect(5 * 16, 3 * 16,2 * 16, 2 * 16), rect(5 * 16, 11 * 16, 2 * 16, 2 * 16) };

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
    rect sprite = player_sprite;
    char dir = 'r';
	point location = point(32, 20);
    bool can_fire = true;
    int16_t can_fire_timeout = 0;
    int16_t fire_delay = 20;
};

struct Projectile
{
    rect sprite;
    point location;
    uint8_t transform;
    uint8_t lifetime = 100;
    int8_t vel_x;
    int8_t vel_y;
};

static std::vector<Projectile> projectiles;

Player player;

Tile_Data current_tile_data;

uint16_t get_tile_from_point(const point& point, uint8_t tile_size, uint8_t tile_map_width)
{	
    uint16_t horizontal_location = point.x / tile_size;

    if (point.x % tile_size > 0)
    {
        horizontal_location += 1;
    }

    uint16_t vertical_location = (point.y / tile_size) * tile_map_width;

    if (vertical_location % tile_size > 0)
    {
        vertical_location += 1;
    }

    const uint16_t array_location = horizontal_location + vertical_location - 1;

    return array_location;
}

Tile_Data get_local_tile_data(const point& point_to_check, uint8_t tile_size, uint8_t tile_map_width)
{
    Tile_Data tile_data;
	
    for (auto y = 0; y < sprite_width; y++)
    {
        for (auto x = 0; x < sprite_width; x++)
        {
            auto array_location = get_tile_from_point(point(point_to_check.x + x, point_to_check.y + y), tile_size, tile_map_width);
            uint8_t tile_scanned = layer_world[array_location];
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

bool is_point_in_rect(const point& object_origin, std::vector<rect>::value_type bounding_rectangle)
{
    if (object_origin.x + sprite_width >= bounding_rectangle.x && object_origin.x <= bounding_rectangle.x + bounding_rectangle.w && object_origin.y + sprite_width > bounding_rectangle.y&& object_origin.y < bounding_rectangle.y + bounding_rectangle.h)
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
    set_screen_mode(screen_mode::hires);

    fb.sprites = spritesheet::load(packed_data);

    world.sprites = fb.sprites;

   //auto tile_index = get_tile_from_point(point(0, 2), 16, tilemap_width);

	
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    fb.alpha = 255;
    fb.pen(rgba(0, 0, 0));
	
    // clear the screen -- fb is a reference to the frame buffer and can be used to draw all things with the 32blit
    fb.clear();

    fb.alpha = 255;
    fb.mask = nullptr;

    vec2 wo(64, 40);

    world.transform =
        mat3::identity() *
        mat3::translation(wo) *
        mat3::scale(vec2(0.5, 0.5)) *
        mat3::translation(vec2(-128, -80));
	
    world.draw(&fb, rect(0, 0, 320, 240), nullptr);
   
    fb.sprite(key_sprite, point(16, 16), point(0,0), vec2(2,2));

	if(current_tile_data.in_water)
	{
        fb.sprite(player_swim_sprite, player.location, point(0, 0), vec2(2, 2));

	}
    else
    {
    	if(player.dir == 'r')
    	{
            fb.sprite(player.sprite, player.location, point(0, 0), vec2(2, 2));
    	}
        else
        {
            fb.sprite(player.sprite, player.location, point(0, 0), vec2(2, 2), 1);
        }
    }

    for (const Projectile& projectile : projectiles)
    {
        fb.sprite(projectile.sprite, projectile.location, point(0, 0));
    }
	
    fb.pen(rgba(255, 255, 255));
    fb.text(tile_name, &minimal_font[0][0], point(0, 0));

}



///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
	for (auto projectile : projectiles)
	{
        projectile.lifetime--;
        projectile.location.x += projectile.vel_x;
        projectile.location.y += projectile.vel_y;
	}
	
    static uint16_t last_buttons = 0;
    uint16_t changed = blit::buttons ^ last_buttons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    int16_t x_change = 0;
    int16_t y_change = 0;	
	
    point new_player_location = player.location;

	if(player.can_fire_timeout > 0)
	{
        player.can_fire_timeout--;
	}
    else if(player.can_fire == false)
    {
        player.can_fire = true;
    }
	
    if (blit::buttons & blit::button::DPAD_LEFT || joystick.x < 0) {
        x_change -= 1;
        new_player_location.x -= 1;
    }
    if (blit::buttons & blit::button::DPAD_RIGHT || joystick.x > 0) {
        x_change += 1;
        new_player_location.x += 1;
    }
    if (blit::buttons & blit::button::DPAD_UP || joystick.y < 0) {
        y_change -= 1;
        new_player_location.y -= 1;
    }
    if (blit::buttons & blit::button::DPAD_DOWN || joystick.y > 0) {
        y_change += 1;
        new_player_location.y += 1;
    }
	if(blit::buttons & blit::button::B)
	{
		if(player.can_fire)
		{
            player.can_fire = false;
            player.can_fire_timeout = player.fire_delay;
            Projectile new_projectile;
            new_projectile.vel_x = x_change;
            new_projectile.vel_y = y_change;
            if (new_projectile.vel_x == 0 || new_projectile.vel_y == 0)
            {
                new_projectile.sprite = proj_2;
            }
            else
            {
                new_projectile.sprite = proj_2_d;
            }
            new_projectile.location = player.location;

            projectiles.push_back(new_projectile);
		}      
	}

    bool move_ok = true;

    /*for (const auto bounding_rectangle : bounding_rectangles)
    {
        if (is_point_in_rect(new_player_location, bounding_rectangle))
        {
            move_ok = false;
            break;
        }
    }
    */

    current_tile_data = get_local_tile_data(new_player_location, sprite_width, tilemap_width);
	
	if(current_tile_data.can_move)
	{
        player.location.x += (x_change);
        player.location.y += (y_change);
	}

	if(x_change > 0)
	{
        player.dir = 'r';
	}
    else if (x_change < 0)
    {
        player.dir = 'l';
    }
	
    last_buttons = blit::buttons;
}