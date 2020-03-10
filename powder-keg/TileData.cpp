#include "TileData.h"

#include "types/point.hpp"
using namespace blit;

uint16_t getTileFromPoint(const Point& point, uint8_t tile_size, uint8_t tile_map_width) {
    uint16_t horizontal_location = point.x / tile_size;

    uint16_t vertical_location = (point.y / tile_size) * tile_map_width;

    if (vertical_location % tile_size > 0) {
        vertical_location += 1;
    }

    const uint16_t array_location = horizontal_location + vertical_location;

    return array_location;
}

TileData getLocalTileData(const blit::Point& Point_to_check, uint8_t tile_size, uint8_t tile_map_width) {
    TileData tileData;

    for (auto y = 0; y < sprite_width; y++) {
        for (auto x = 0; x < sprite_width; x++) {
            const auto array_location = getTileFromPoint(blit::Point(Point_to_check.x + x, Point_to_check.y + y), tile_size,
                                                         tile_map_width);
            const uint8_t tileScanned = layer_world[array_location];

            tileData.id = tileScanned;
            tileData.index = array_location;

            switch (tileScanned) {
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

    if (tileData.pixels_in_water > (sprite_width * sprite_width / 2)) {
        tileData.in_water = true;
    }

    return tileData;
}
