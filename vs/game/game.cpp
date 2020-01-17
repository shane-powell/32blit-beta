#include "game.hpp"

using namespace blit;

// Vertical resolution
const int res_y = 240;

// Horizontal resolution
const int res_x = 320;

const int8_t sprite_size = 16;

const int8_t grass_height = 10;

const rect lawn = rect(0, 0, 2, 2);
const rect grass = rect(2, 0, 2, 2);
const rect mower = rect(4, 0, 2, 2);
const rect sky = rect(6, 0, 2, 2);

point mower_location;

// Keep track of game state
enum enum_state {
    title = 0,
    game = 1,
    end = 2
};
enum_state game_state = enum_state::game;

//size screen_size(160, 120);
void new_game()
{
    // Set the location of the mower to the bottom right of the screen
    mower_location = point(res_x - sprite_size, res_y - sprite_size);

    game_state = game;
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {

	// Set screen to highest resolution
    set_screen_mode(screen_mode::hires);

	// Load the spritesheet from the packed data
    fb.sprites = spritesheet::load(packed_data);

    new_game();
	
}


void render_game()
{
	// Set pen to sky colour
	fb.pen(rgba(1, 240, 255));
	
	// Set the current position of the lawn draw location to the bottom right of the screen
	int current_pos_x = res_x - sprite_size;
	int current_pos_y = res_y - sprite_size;


	// draw lawn
	for (uint8_t i = grass_height; i > 0;i--)
	{
		while (current_pos_x >= 0)
		{
			// If the mower is to the left of the current position or above it then draw lawn
			if (mower_location.y < current_pos_y || mower_location.y == current_pos_y && mower_location.x <= current_pos_x)
			{
				fb.sprite(lawn, point(current_pos_x, current_pos_y));
			}
				// Otherwise draw un cut grass
			else
			{
				fb.sprite(grass, point(current_pos_x, current_pos_y));
			}

			current_pos_x -= sprite_size;
		}

		current_pos_x = res_x - sprite_size;
		current_pos_y -= sprite_size;
	}

	// draw player
	fb.sprite(mower, mower_location);
}

void render_end()
{
	fb.pen(rgba(255, 255, 255));
	fb.text("Congratulations you successfully held a button down.", &minimal_font[0][0], point(5, 4));
	fb.text("Press a to play again", &minimal_font[0][0], point(5, 16));
	fb.pen(rgba(0, 138, 0));
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time)
{
	// clear the screen -- fb is a reference to the frame buffer and can be used to draw all things with the 32blit
	fb.clear();

	fb.alpha = 255;
	fb.mask = nullptr;
	
	switch(game_state)
	{
	case game:
		// If game is running then render the action.
		render_game();
		break;
	case end:
		// If game has ended render the end screen
		render_end();
		break;
	default:
		break;
	}
}

void update_game(const uint16_t pressed)
{
	if (pressed & blit::button::B)
	{
		if(mower_location.x > 0)
		{
			mower_location.x -= 2;
		}
		else if (mower_location.y > res_y - grass_height * 16)
		{
			mower_location.x = res_x - sprite_size;
			mower_location.y -= sprite_size;
		}
		else
		{
			game_state = end;
		}
        
	}
}

void update_end(const uint16_t pressed)
{
	if (pressed & blit::button::A)
	{
		new_game();
	}
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
	const uint16_t changed = blit::buttons ^ last_buttons;
	const uint16_t pressed = changed & blit::buttons;
	uint16_t released = changed & ~blit::buttons;
	
	switch (game_state)
	{
	case game:
		// If game is running then render the action.
		update_game(pressed);
		break;
	case end:
		// If game has ended render the end screen
		update_end(pressed);
		break;
	default:
		break;
	}
	
    

    
}