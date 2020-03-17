#pragma once
#include <cstdint>

namespace blit {
	struct Point;
}

const uint8_t sprite_width = 16;

// Point player_location = Point(32, 20);

const uint32_t tilemap_width = 32;

const uint32_t tilemap_height = 16;

struct TileData
{
    uint8_t id = 0;
    uint16_t index = 0;
    bool canMove = true;
    uint16_t pixels_in_water = 0;
    bool in_water = false;
    float movement_modifier = 0;
    float life_modifier = 0;
};

//TileData getLocalTileData(const blit::Point& Point_to_check, uint8_t tile_size, uint8_t tile_map_width);



