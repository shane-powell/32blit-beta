#include "game.hpp"

using namespace blit;

// size(320, 240)

const int res_y = 240;
const int res_x = 320;

const int8_t sprite_size = 16;

const int8_t grass_height = 10;

const rect lawn = rect(0, 0, 2, 2);
const rect mower = rect(4, 0, 2, 2);
const rect grass = rect(2, 0, 2, 2);

point mower_location;

//size screen_size(160, 120);


///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {

    set_screen_mode(screen_mode::hires);
    
    fb.sprites = spritesheet::load(packed_data);

    mower_location = point(res_x - sprite_size, res_y - sprite_size);
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    // clear the screen -- fb is a reference to the frame buffer and can be used to draw all things with the 32blit
    fb.clear();

    // draw some text at the top of the screen
    fb.alpha = 255;
    fb.mask = nullptr;
    //fb.pen(rgba(255, 255, 255));
    // fb.rectangle(rect(0, 0, 320, 14));
    //fb.pen(rgba(0, 0, 0));
    // fb.text("Hello is anybody there?", &minimal_font[0][0], point(5, 4));
    //fb.sprite(lawn, point(15, 0));
    fb.sprite(lawn, point(0, 0));
    //fb.sprite(lawn, point(31, 0));

    fb.sprite(grass, point(31, 0));

    int current_pos_x = res_x - sprite_size;
    int current_pos_y = res_y - sprite_size;

	
	for (uint8_t i = grass_height; i > 0;i--)
	{
        while (current_pos_x >= 0)
        {
            if (mower_location.y < current_pos_y || mower_location.y == current_pos_y && mower_location.x <= current_pos_x)
            {
                fb.sprite(lawn, point(current_pos_x, current_pos_y));
            }
            else
            {
                fb.sprite(grass, point(current_pos_x, current_pos_y));
            }

            current_pos_x -= sprite_size;
        }

		current_pos_x = res_x - sprite_size;
        current_pos_y -= sprite_size;
	}

    fb.sprite(mower, mower_location);
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

    if (pressed & blit::button::B)
    {
	    if(mower_location.x > 0)
	    {
            mower_location.x -= 2;
	    }
        else
        {
            mower_location.x = res_x - sprite_size;
        	
	        if (mower_location.y > res_y - grass_height * 16)
	        {
                mower_location.y -= sprite_size;
	        }
        }
    }
}