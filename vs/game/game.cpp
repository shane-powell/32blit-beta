#include "game.hpp"

using namespace blit;

/**
 * \brief The vertical resolution
 */
const uint16_t res_y = 240;


/**
 * \brief The horizontal resolution
 */
const uint16_t res_x = 320;

/**
 * \brief The size of a single sprite (16x16)
 */
const int8_t sprite_size = 16;

/**
 * \brief The height in sprites of the grass from the bottom of the screen.
 */
const int8_t grass_height = 10;

/**
 * \brief The location of the lawn (post lawnmower) sprite on the spritesheet.
 */
const rect lawn = rect(0, 0, 2, 2);
/**
 * \brief The location of the grass sprite on the spritesheet.
 */
const rect grass = rect(2, 0, 2, 2);
/**
 * \brief The location of the mower sprite on the spritesheet.
 */
const rect mower = rect(4, 0, 2, 2);

/**
 * \brief The location of the sky sprite on the spritesheet. (Not currently used as we just draw the background sky coloured)
 */
const rect sky = rect(6, 0, 2, 2);

std::vector<std::string> motivational_messages = {"001100111011100011110011100111100",
	"JESUS, YOU CALL THAT MOWING? I'VE SEEN\nBARBERS WHO CUT BETTER THAN THAT.SORRY.\nTHAT WASN'T FUNNY. I'LL TRY HARDER LATER.",
	"TRES MAGNIFIQUE! MEGA MOWER DUDE ALERT.\nYOU SURE ARE THE CATS RINGBINDER.\nALL SIX OF MY NIPPLES ARE TINGLING.",
	"OH NO! BY THE LOOK OF THAT LAWN\nIT LOOKS LIKE IT'S\nTIME TO CALL NATIONAL RESCUE",
	"LAWNMOWER MAN IN NEAT GRASS SHOCK\nANOTHER SUN SCOOP\nREAD MORE ON PAGE 3!",
	"OH DEAR. THAT WAS A BIT CRAP\nDON'T LET ME SEE YOU ROUND HERE AGAIN OR\nI'LL SET MY LETTUCE ON YOU.",
	"NOT BAD AT ALL\nWELL WORTH A CHEESE SARNIE\nMY HEARTY CONGRATULATIONS."
};


/**
 * \brief The comment to show on the end screen
 */
std::string end_comment;


/**
 * \brief The mowers location on the screen
 */
point mower_location;

/**
 * \brief Defines whether or not the mower is currently mowing.
 */
bool mowing = false;


enum enum_state {
	/**
     * \brief The initial scene. Mower is selected from here
     */
    title = 0,
	/**
     * \brief The game state represents the main game play loop.
     */
    game = 1,
	/**
     * \brief A scene to draw at the end of the game.
     */
    end = 2,
	/**
	 * \brief Represents a state after a mower has been selected from the title
	 */
	mower_selection = 3
};


/**
 * \brief The selected mower from title menu.
 */
uint8_t mower_choice = 0;

/**
 * \brief Defines the current state the game is in.
 * Used alter what the update and render functions do.
 */
enum_state game_state = enum_state::title;

/**
 * \brief Splits a string into a vector of strings using a given delimiter
 * \param string_to_split 
 * \param delimiter The string delimiter to split the string on.
 * \return A vector of strings.
 */
std::vector<std::string> split_string_with_delimiter(std::string string_to_split, const std::string& delimiter) {
		std::vector<std::string> list;
		size_t pos = 0;
		while ((pos = string_to_split.find(delimiter)) != std::string::npos) {
			auto token = string_to_split.substr(0, pos);
			list.push_back(token);
			string_to_split.erase(0, pos + delimiter.length());
		}
		list.push_back(string_to_split);
		return list;
	}

/**
 * \brief Resets all game values to default ready for the game to start again.
 */
void reset_game()
{
	// Set the location of the mower to the bottom right of the screen
	mower_location = point(res_x - sprite_size, res_y - sprite_size);

	// Make sure vibration is off
	blit::vibration = 0;

	// Reset mower choice to default
	mower_choice = 0;

	// Turn off the mower
	mowing = false;
}


/**
 * \brief Resets the game state and starts a new game.
 */
void new_game()
{
	// Reset the game ready for a new game
	reset_game();

	// Set vibration to mower idle
	blit::vibration = 0.2;

	// Set the game state to game
    game_state = game;
}

void end_game()
{
	// Safely shut down lawnmower
	blit::vibration = 0;
	mowing = false;

	// Set the end comment to display
	const uint8_t random_index = blit::random() % (motivational_messages.capacity() - 1);
	end_comment = motivational_messages[random_index];

	//end_comment = "Congratulations you successfully held a button down.";

	// Set the gamestate to end
	game_state = end;
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {

	blit::vibration = 0;

	// Set screen to highest resolution
    set_screen_mode(screen_mode::hires);

	// Load the spritesheet from the packed data
    fb.sprites = spritesheet::load(packed_data);

	reset_game();

	end_game();
}


void render_game()
{
	// Set pen to sky colour
	fb.pen(rgba(1, 240, 255));
	
	// Set the current position of the lawn draw location to the bottom right of the screen
	int current_pos_x = res_x - sprite_size;
	int current_pos_y = res_y - sprite_size;


	// draw lawn up to defined grass height
	for (uint8_t i = grass_height; i > 0;i--)
	{
		// Continue until we reach the left side of the screen
		while (current_pos_x >= 0)
		{
			// If the mower is to the left of the current position or above it then draw lawn
			if (mower_location.y < current_pos_y || mower_location.y == current_pos_y && mower_location.x <= current_pos_x)
			{
				// Draw cut lawn
				fb.sprite(lawn, point(current_pos_x, current_pos_y));
			}
				// Otherwise draw un cut grass
			else
			{
				// Draw grass
				fb.sprite(grass, point(current_pos_x, current_pos_y));
			}

			// Move current draw position one sprites width to the left
			current_pos_x -= sprite_size;
		}

		// Move current draw position to the far right 
		current_pos_x = res_x - sprite_size;

		// Move current draw position up one sprites height 
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
			fb.text("The Clivester", &minimal_font[0][0], point(res_x / 4 + 40, res_y / 2));
			fb.text("is in perfect working order.", &minimal_font[0][0], point(res_x / 4 + 5, res_y / 2 + 10));
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

	std::vector<std::string> message_lines = split_string_with_delimiter(end_comment, "\n");

	int8_t y_offset = 0;
	
	for (const auto& message_line : message_lines)
	{
		const uint16_t x_offset = (res_x - message_line.size() * 6) / 2;
		fb.text(message_line, &minimal_font[0][0], point(x_offset, res_y / 2 + y_offset), false);
		y_offset += 10;
	}
	
	
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

	fb.text("A Gardensoft game", &minimal_font[0][0], point(res_x / 4 + 40, 45));
	fb.text("32Blit conversion by Shane powell", &minimal_font[0][0], point(res_x / 4, 55));

	// Draw mower options and highlight the currently selected one.
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

	// Draw controls at bottom of screen
	fb.text("Controls: A = Select B = Mow", &minimal_font[0][0], point(res_x / 4 + 10, 200));

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
		// If game has just initialised or has been reset then the title screen is displayed.
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
		// If mower is selected draw mower selection screen.
		render_mower_selection();
	default:
		break;
	}
}


/**
 * \brief Reacts to input while the game is in the title state.
 * \param released Tracks any buttons released
 */
void update_title(const uint16_t released)
{
	// If A button is pressed move to mower selection screen.
	if (released & blit::button::A)
	{
		game_state = mower_selection;
	} // If dpad up is pressed then move the mower choice up one. If at top of list move to the bottom option.
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
	} // If dpad down is pressed then move the mower choice down one. If at bottom of list move to the top option.
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

/**
 * \brief Reacts to user input and changes while in the game state.
 * \param pressed Tracks any buttons pressed
 * \param released Tracks any buttons released
 */
void update_game(const uint16_t pressed, const uint16_t released)
{
	
	// If button B is pressed set mower to on and increase vibration
	if (pressed & blit::button::B)
	{
		mowing = true;
		blit::vibration = 1;
	} // If button B is released set mower to off and decrease vibration.
	else if(released & blit::button::B)
	{
		mowing = false;
		blit::vibration = 0.2;
	}

	// If mower is on then move the mower along
	if (mowing)
	{
		// If the x location of the mower is greater than 0 move the mower 2 pixels to the left
		if (mower_location.x > 0)
		{
			mower_location.x -= 2;
		}
		// Otherwise if not at top of lawn move the mowers location up by one sprites height.
		else if (mower_location.y > res_y - grass_height * 16)
		{
			mower_location.x = res_x - sprite_size;
			mower_location.y -= sprite_size;
		}
		// Otherwise we have reached the end or something really bad and unexpected has happened.
		else
		{
			end_game();
		}
	}

}


/**
 * \brief Checks for input while in the end state
 * \param released Tracks any buttons released
 */
void update_end(const uint16_t released)
{
	// If button A is pressed reset the game and return to title
	if (released & blit::button::A)
	{
		reset_game();
		game_state = title;
	}
}

/**
 * \brief Checks for input while in the mower selection state
 * \param released Tracks any buttons released
 */
void update_mower_selection(const uint16_t released)
{
	// If the A button was pressed and the mower choice was 4 the proceed to game otherwise return to title.
	if (released & blit::button::A)
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

	// Feed the button states in the current game states update function.
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

	// Remember which buttons were pressed so we can compare them in the next update.
	last_buttons = blit::buttons;

    
}