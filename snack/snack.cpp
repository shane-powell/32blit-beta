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
    int16_t X = 0;
    int16_t Y = 0;
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
    foodLocation.point.x = random() % (maxX - borderOffset) + 1;
    foodLocation.point.y = random() % (maxY - borderOffset) + 1;
}

void StartGame()
{
    p1.X = maxX / 2;
    p1.Y = (maxY / 2) + 10;
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
    p1.Moves[arrayPosition].point.y = p1.Y;

    if (p1.Moves[arrayPosition].Active == false)
    {
        p1.Moves[arrayPosition].Active = true;
    }
    p1.Moves[arrayPosition].point.x = p1.X;
    p1.Moves[arrayPosition].point.y = p1.Y;

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

    if (p1.X >= maxX || p1.X <= minX || p1.Y >= maxY || p1.Y <= minY)
    {
        EndGame();
    }

    //Check if hit tail
    for (auto& Move : p1.Moves)
    {

        if (Move.Active == true)
        {
            if (Move.point.x == p1.X && Move.point.y == p1.Y)
            {
                EndGame();
            }
        }
    }

    // Check if on food
    if (p1.X == foodLocation.point.x && p1.Y == foodLocation.point.y)
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

    DrawFrame();

	
    
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
    //if (gameState == 'G')
    //{
    //    arduboy.setFrameRate(30);
    //    DrawFrame();

    //    if (arduboy.pressed(RIGHT_BUTTON)) {
    //        if (p1.dir != 'L')
    //        {
    //            p1.dir = 'R';
    //        }
    //    }

    //    if (arduboy.pressed(LEFT_BUTTON)) {
    //        if (p1.dir != 'R')
    //        {
    //            p1.dir = 'L';
    //        }
    //    }

    //    if (arduboy.pressed(UP_BUTTON)) {
    //        if (p1.dir != 'D')
    //        {
    //            p1.dir = 'U';
    //        }
    //    }

    //    if (arduboy.pressed(DOWN_BUTTON)) {
    //        if (p1.dir != 'U')
    //        {
    //            p1.dir = 'D';
    //        }
    //    }

    //    switch (p1.dir)
    //    {
    //    case 'U':
    //        if (p1.Y > minY)
    //        {
    //            p1.Y -= 1;
    //        }
    //        break;
    //    case 'D':
    //        if (p1.Y < maxY)
    //        {
    //            p1.Y += 1;
    //        }
    //        break;
    //    case 'L':
    //        if (p1.X > minX)
    //        {
    //            p1.X -= 1;
    //        }
    //        break;
    //    case 'R':
    //        if (p1.X < maxX)
    //        {
    //            p1.X += 1;
    //        }

    //        break;
    //    }

    //    CollisionDetection();

    //    arduboy.drawPixel(p1.X, p1.Y, 1);
    //    arduboy.drawPixel(foodLocation.X, foodLocation.Y, 1);

    //    if (p1.X != previousPosition.X || p1.Y != previousPosition.Y)
    //    {
    //        previousPosition.X = p1.X;
    //        previousPosition.Y = p1.Y;
    //        LogMove();
    //    }
    //    DrawTail();

    //    arduboy.setCursor(63, 0);
    //    arduboy.print("Score ");

    //    char scoreBuff[5];
    //    sprintf(scoreBuff, "%05d", score);

    //    arduboy.print(scoreBuff);
    //}
    //else if (gameState == 'E')
    //{
    //    arduboy.setFrameRate(10);
    //    arduboy.setCursor(20, 20);
    //    arduboy.print("Game Over");

    //    arduboy.setCursor(20, 40);
    //    arduboy.print("Score ");

    //    char scoreBuff[5];
    //    sprintf(scoreBuff, "%05d", score);

    //    arduboy.print(scoreBuff);

    //    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
    //        gameState = 'T';
    //    }

    //}
    //else if (gameState == 'T')
    //{
    //    arduboy.setFrameRate(10);
    //    arduboy.setCursor(50, 1);
    //    arduboy.print("Snack");

    //    arduboy.setCursor(20, 15);
    //    arduboy.print("By Shane Powell");

    //    arduboy.setCursor(50, 55);
    //    arduboy.print("Sound");

    //    arduboy.setCursor(90, 55);
    //    arduboy.print("On");

    //    arduboy.setCursor(109, 55);
    //    arduboy.print("Off");

    //    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
    //        StartGame();
    //    }
    //    else if (arduboy.pressed(LEFT_BUTTON))
    //    {
    //        sound = true;
    //    }
    //    else if (arduboy.pressed(RIGHT_BUTTON))
    //    {
    //        sound = false;
    //    }

    //    if (sound == true)
    //    {
    //        arduboy.drawRect(88, 53, 18, 11, WHITE);
    //    }
    //    else
    //    {
    //        arduboy.drawRect(107, 53, 21, 11, WHITE);
    //    }
    //}
}