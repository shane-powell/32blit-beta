#include "game.hpp"

using namespace blit;

// Vertical resolution
const uint16_t res_y = 240;

// Horizontal resolution
const uint16_t res_x = 320;

const int8_t sprite_size = 16;

const int8_t grass_height = 10;

const rect lawn = rect(0, 0, 2, 2);
const rect grass = rect(2, 0, 2, 2);
const rect mower = rect(4, 0, 2, 2);
const rect sky = rect(6, 0, 2, 2);

point mower_location;

bool mowing = false;

// Keep track of game state
enum enum_state {
    title = 0,
    game = 1,
    end = 2,
	mower_selection = 3
};

uint8_t mower_choice = 0;

enum_state game_state = enum_state::game;

void reset_game()
{
	// Set the location of the mower to the bottom right of the screen
	mower_location = point(res_x - sprite_size, res_y - sprite_size);

	mower_choice = 0;
}

//size screen_size(160, 120);
void new_game()
{
	reset_game();

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

	reset_game();

	game_state = title;
	
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

void render_mower_selection()
{
	// Set colour to black for text.
	fb.pen(rgba(0, 0, 0));

	// Process mower selection (Only valid choice is 5)
	switch (mower_choice)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			fb.text("I'm Sorry but that is out of order", &minimal_font[0][0], point(res_x / 4, res_y / 2));
		break;
		case 4:
		default:
			fb.text("The Clivester", &minimal_font[0][0], point(res_x / 4, res_y / 2));
			fb.text("is in perfect working order.", &minimal_font[0][0], point(res_x / 4, res_y / 2 + 10));
		break;
	}

	// Set colour to green for background
	fb.pen(rgba(0, 138, 0));
}

void render_end()
{
	// Set colour to white for text.
	fb.pen(rgba(255, 255, 255));

	//Write end of game text
	fb.text("Congratulations you successfully held a button down.", &minimal_font[0][0], point(5, 4));
	fb.text("Press a to play again", &minimal_font[0][0], point(5, 16));

	// Set colour to green for background
	fb.pen(rgba(0, 138, 0));
}

void render_title()
{
	// Set colour to black for text.
	fb.pen(rgba(0, 0, 0));

	//Write title text
	fb.text("Advanced Lawnmower Simulator", &minimal_font[0][0], point(res_x / 4, 4));
	fb.text("---------------------------------------", &minimal_font[0][0], point(res_x / 4, 12));
	//fb.text("A Gardensoft game", &minimal_font[0][0], rect(0, 0, res_x, 12));

	fb.text("A Gardensoft game", &minimal_font[0][0], point(res_x / 4 + 40, 45));
	fb.text("32Blit conversion by Shane powell", &minimal_font[0][0], point(res_x / 4, 55));

	if(mower_choice == 0)
	{
		fb.pen(rgba(255, 255, 255));
	}
	else
	{
		fb.pen(rgba(0, 0, 0));
	}

	fb.text("1) Mega-Grass-Thwopper", &minimal_font[0][0], point(res_x / 4 + 20, 70));

	if (mower_choice == 1)
	{
		fb.pen(rgba(255, 255, 255));
	}
	else
	{
		fb.pen(rgba(0, 0, 0));
	}

	fb.text("2) Speed-Monster Ripper", &minimal_font[0][0], point(res_x / 4 + 20, 80));

	if (mower_choice == 2)
	{
		fb.pen(rgba(255, 255, 255));
	}
	else
	{
		fb.pen(rgba(0, 0, 0));
	}

	fb.text("3) The Dongster", &minimal_font[0][0], point(res_x / 4 + 40, 90));

	if (mower_choice == 3)
	{
		fb.pen(rgba(255, 255, 255));
	}
	else
	{
		fb.pen(rgba(0, 0, 0));
	}

	fb.text("4) The Campbell Cutter", &minimal_font[0][0], point(res_x / 4 + 20, 100));

	if (mower_choice == 4)
	{
		fb.pen(rgba(255, 255, 255));
	}
	else
	{
		fb.pen(rgba(0, 0, 0));
	}

	fb.text("5) The Clivester", &minimal_font[0][0], point(res_x / 4 + 40, 110));

	fb.pen(rgba(0, 0, 0));

	fb.text("?", &minimal_font[0][0], point(res_x / 2, 140));

	// Set colour to green for background
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
	case title:
		render_title();
		break;
	case game:
		// If game is running then render the action.
		render_game();
		break;
	case end:
		// If game has ended render the end screen
		render_end();
		break;
	case mower_selection:
		render_mower_selection();
	default:
		break;
	}
}

void update_title(const uint16_t released)
{
	if (released & blit::button::B)
	{
		game_state = mower_selection;
	}
	else if (released & blit::button::DPAD_UP)
	{
		if(mower_choice > 0)
		{
			mower_choice--;
		}
		else
		{
			mower_choice = 4;
		}
	}
	else if (released & blit::button::DPAD_DOWN)
	{
		if(mower_choice < 4)
		{
			mower_choice++;
		}
		else
		{
			mower_choice = 0;
		}
	}

}

void update_game(const uint16_t pressed, const uint16_t released)
{
	

	if (pressed & blit::button::B)
	{
		mowing = true;
	}
	else if(released & blit::button::B)
	{
		mowing = false;
	}


	if (mowing)
	{
		if (mower_location.x > 0)
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

void update_end(const uint16_t released)
{

	if (released & blit::button::B)
	{
		reset_game();
		game_state = title;
	}
}

void update_mower_selection(const uint16_t released)
{
	if (released & blit::button::B)
	{
		if(mower_choice == 4)
		{
			new_game();
		}
		else
		{
			game_state = title;
		}
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
	const uint16_t released = changed & ~blit::buttons;
	
	switch (game_state)
	{
	case title:
		update_title(released);
	case game:
		// If game is running then render the action.
		update_game(pressed, released);
		break;
	case end:
		// If game has ended render the end screen
		update_end(released);
		break;
	case mower_selection:
		update_mower_selection(released);
		break;
	default:
		break;
	}
	
	last_buttons = blit::buttons;

    
}