#include "game.hpp"

using namespace blit;

rect s_rect = rect(0, 0, 2, 2);
rect mower = rect(15, 0, 2, 2);

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
    fb.sprite(s_rect, point(0, 0));
    //fb.sprite(s_rect, point(32, 0));
    fb.sprite(mower, point(15, 0));
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
}