#include "snack.hpp"

using namespace blit;

int16_t maxX = 320;
int16_t minX = 0;
int16_t maxY = 230;
int16_t minY = 10;
char gameState = 'T';
bool sound = false;

int8_t borderOffset = 2;

struct coords
{
public:
    Point point;
    bool Active = false;
};

typedef struct coords Coords;

struct player
{
    Point location = Point(0,0);
    char dir = 'R';
    int Len = 1;
    static const int MaxLength = 100;
    Coords Moves[MaxLength];
};

typedef struct player Player;

Player p1;
Coords foodLocation;
Coords previousPosition;


int score = 0;
int arrayPosition = 0;

void GenerateFood()
{
    foodLocation.point.x = blit::random() % (maxX - borderOffset) + 1;
    foodLocation.point.y = blit::random() % (maxY - borderOffset) + 1;
}

void StartGame()
{
    p1.location.x = maxX / 2;
    p1.location.y = (maxY / 2) + 10;
    GenerateFood();
    score = 0;
    arrayPosition = 0;
    p1.Len = 1;
    for (int i = 0; i < p1.MaxLength; i++) {
        p1.Moves[i].Active = false;
    }


    gameState = 'G';
}

void LogMove()
{
    p1.Moves[arrayPosition].point.y = p1.location.y;

    if (p1.Moves[arrayPosition].Active == false)
    {
        p1.Moves[arrayPosition].Active = true;
    }
    p1.Moves[arrayPosition].point.x = p1.location.x;
    p1.Moves[arrayPosition].point.y = p1.location.y;

    if (arrayPosition < p1.Len - 1)
    {
        arrayPosition += 1;

    }
    else
    {
        arrayPosition = 0;
    }
}

void EndGame()
{
    //if (sound == true)
    //{
    //    arduboy.tunes.tone(200, 200);
    //    delay(400);
    //    arduboy.tunes.tone(100, 400);
    //    delay(600);
    //    arduboy.tunes.tone(50, 2000);
    //}

    gameState = 'E';
}

void CollisionDetection()
{

    //Check if hit wall

    if (p1.location.x >= maxX || p1.location.x <= minX || p1.location.y >= maxY || p1.location.y <= minY)
    {
        EndGame();
    }

    //Check if hit tail
    for (auto& Move : p1.Moves)
    {

        if (Move.Active == true)
        {
            if (Move.point.x == p1.location.x && Move.point.y == p1.location.y)
            {
                EndGame();
            }
        }
    }

    // Check if on food
    if (p1.location.x == foodLocation.point.x && p1.location.y == foodLocation.point.y)
    {
        if (p1.Len < Player::MaxLength)
        {
            p1.Len += 1;
        }
        if (sound == true)
        {
            // arduboy.tunes.tone(987, 160);
        }

        score += 1;
        LogMove();
        GenerateFood();
    }
}

void DrawTail()
{
    int tailPosition = arrayPosition;
    for (int i = 0; i < p1.Len; i++) {
        if (p1.Moves[tailPosition].Active == true)
        {
            screen.pixel(p1.Moves[tailPosition].point);
        }
        else
        {
            return;
        }


        if (tailPosition > 0)
        {
            tailPosition -= 1;
        }
        else
        {
            tailPosition = p1.Len - 1;
        }
    }
}

void DrawFrame()
{
    screen.pen(RGBA(255, 255, 255));
    screen.rectangle(Rect(minX, minY, maxX, maxY));
    screen.pen(RGBA(0, 0, 0));
    screen.rectangle(Rect(minX +1, minY + 1, maxX - borderOffset, maxY - borderOffset));
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::hires);
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();

    // draw some text at the top of the screen
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen(RGBA(255, 255, 255));
    //screen.rectangle(Rect(0, 0, 320, 14));
    //screen.pen(RGBA(0, 0, 0));
    //screen.text("Hello 32blit!", &minimal_font[0][0], Point(5, 4));
    if (gameState == 'G') {
        DrawFrame();
        screen.pen(RGBA(255, 0, 0));

        screen.pixel(p1.location);
        screen.pixel(foodLocation.point);

        if (p1.location.x != previousPosition.point.x || p1.location.y != previousPosition.point.y)
        {
            previousPosition.point.x = p1.location.x;
            previousPosition.point.y = p1.location.y;
            LogMove();
        }
        DrawTail();


        screen.text("Score ", &minimal_font[0][0], Point(63, 0));

        //char scoreBuff[5];
        //sprintf_s(scoreBuff, "%05d", score);
        
        screen.text(std::to_string(score), &minimal_font[0][0], Point(95, 0));
    }
    else if (gameState == 'E')
    {

        screen.text("Game Over", &minimal_font[0][0], Point(160, 120));


        screen.text("Score ", &minimal_font[0][0], Point(160, 130));

       // char scoreBuff[5];
        //sprintf_s(scoreBuff, "%05d", score);

        screen.text(std::to_string(score), &minimal_font[0][0], Point(160, 140));

    }
    else if (gameState == 'T')
    {

        screen.text("Snack", &minimal_font[0][0], Point(maxX /2, 1),true, center_h);



        screen.text("By Shane Powell", &minimal_font[0][0], Point(maxX / 2, 15), true, center_h);



        screen.text("Sound ", &minimal_font[0][0], Point(50, 55));


        if (sound == true)
        {
            screen.pen(RGBA(255, 255, 255));
            screen.rectangle(Rect(88, 53, 18, 11));
            screen.pen(RGBA(0, 0, 0));
            screen.rectangle(Rect(89, 54, 16, 9));
        }
        else
        {
            screen.pen(RGBA(255, 255, 255));
            screen.rectangle(Rect(107, 53, 21, 11));
            screen.pen(RGBA(0, 0, 0));
            screen.rectangle(Rect(108, 54, 19, 9));
        }
        screen.pen(RGBA(255, 255, 255));


        screen.text("On ", &minimal_font[0][0], Point(90, 55));

        screen.text("Off ", &minimal_font[0][0], Point(109, 55));
    }

    screen.pen(RGBA(0,0, 0));
	
    
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
    uint16_t changed = buttons ^ last_buttons;
    uint16_t pressed = changed & buttons;
    uint16_t released = changed & ~buttons;

    if (gameState == 'G')
    {
        DrawFrame();

        if (pressed & Button::DPAD_RIGHT) {
            if (p1.dir != 'L')
            {
                p1.dir = 'R';
            }
        }

        if (pressed & Button::DPAD_LEFT) {
            if (p1.dir != 'R')
            {
                p1.dir = 'L';
            }
        }

        if (pressed & Button::DPAD_UP) {
            if (p1.dir != 'D')
            {
                p1.dir = 'U';
            }
        }

        if (pressed & Button::DPAD_DOWN) {
            if (p1.dir != 'U')
            {
                p1.dir = 'D';
            }
        }

        switch (p1.dir)
        {
        case 'U':
            if (p1.location.y > minY)
            {
                p1.location.y -= 1;
            }
            break;
        case 'D':
            if (p1.location.y < maxY)
            {
                p1.location.y += 1;
            }
            break;
        case 'L':
            if (p1.location.x > minX)
            {
                p1.location.x -= 1;
            }
            break;
        case 'R':
            if (p1.location.x < maxX)
            {
                p1.location.x += 1;
            }

            break;
        }
        CollisionDetection();
    }
    else if (gameState == 'E')
    {

        if (pressed & Button::A || pressed & Button::B) {
            gameState = 'T';
        }

    }
    else if (gameState == 'T')
    {

        if (pressed & Button::A || pressed & Button::B) {
            StartGame();
        }
        else if (pressed & Button::DPAD_LEFT)
        {
            sound = true;
        }
        else if (pressed & Button::DPAD_RIGHT)
        {
            sound = false;
        }
    }

    last_buttons = buttons;
    
}