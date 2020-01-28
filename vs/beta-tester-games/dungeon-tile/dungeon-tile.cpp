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
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const rect key_sprite = rect(2, 0, 1, 1);

const rect player_sprite = rect(0, 1, 1, 1);

const uint8_t sprite_width = 16;

point player_location = point(32, 20);

const uint32_t tilemap_width = 32;

const uint32_t tilemap_height = 16;

tilemap world((uint8_t*)layer_world, nullptr, size(32, 16), nullptr);

std::vector<rect> bounding_rectangles = {rect(0,0,  32 * 16, 16), rect(0, 0, 16, 20 * 16), rect(5 * 16, 3 * 16,2 * 16, 2 * 16), rect(5 * 16, 11 * 16, 2 * 16, 2 * 16) };

std::string tile_name;

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

    fb.sprite(player_sprite, player_location, point(0, 0), vec2(2, 2));

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
    static uint16_t last_buttons = 0;
    uint16_t changed = blit::buttons ^ last_buttons;
    uint16_t pressed = changed & blit::buttons;
    uint16_t released = changed & ~blit::buttons;

    point new_player_location = player_location;
	
    if (blit::buttons & blit::button::DPAD_LEFT) {
        new_player_location.x -= 1;
    }
    if (blit::buttons & blit::button::DPAD_RIGHT) {
        new_player_location.x += 1;
    }
    if (blit::buttons & blit::button::DPAD_UP) {
        new_player_location.y -= 1;
    }
    if (blit::buttons & blit::button::DPAD_DOWN) {
        new_player_location.y += 1;
    }

    bool move_ok = true;

    for (const auto bounding_rectangle : bounding_rectangles)
    {
        if (is_point_in_rect(new_player_location, bounding_rectangle))
        {
            move_ok = false;
            break;
        }
    }

    auto tile_index = get_tile_from_point(new_player_location, 16, tilemap_width);

    auto tile_scanned = layer_world[tile_index];

    tile_name = std::to_string(tile_index);

    tile_name.append(" ");
	
    switch (tile_scanned)
    {
    case 0:
        tile_name += "Wall";
    	break;
    case 1:
        tile_name += "Floor";
    	break;
    default:
        tile_name +=
            "unknown";
    	break;
    }

	if(move_ok)
	{
        player_location = new_player_location;
	}
	
    last_buttons = blit::buttons;
}