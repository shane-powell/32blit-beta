#include "blitboing.hpp"
#include "assets.hpp"
using namespace blit;

int32_t maxX = 320;
int32_t minX = 0;
int32_t maxY = 230;
int32_t minY = 0;

int32_t PLAYER_SPEED = 6;
int32_t MAX_AI_SPEED = 6;

int8_t spriteSize = 8;

std::tuple<int, int> Normalised(int x, int y)
{
    auto length = hypot(x, y);

    return std::tuple<int, int>(x / length, y / length);
}

int16_t Sign (int16_t x)
{
	if (x < 0)
	{
        return -1;
	}
   
    return 1;
    
}

bool IsRectIntersecting(Rect rect1,Rect rect2) {
    if (rect1.x + rect1.w > rect2.x &&
        rect1.x < rect2.x + rect2.w &&
        rect1.y + rect1.h > rect2.y &&
        rect1.y < rect2.y + rect2.h) {
        return true;
    }
    return false;
}

class Actor {
public:
    virtual ~Actor() = default;
    Point loc = Point(16, 16);
    Size size;
    Rect spriteLocation;
	
    Actor() {

    }

    virtual Size GetSize()
    {
        return this->size;
    }

	virtual Rect GetSpriteLocation()
    {
        return this->spriteLocation;
    }
    
    Point GetLocation()
    {
        return this->loc;
    }
};

class Impact : Actor
{
public:
    uint8_t time = 0;
    void Update()
    {
        // todo update image
    	
        this->time += 1;
    }
};

class Bat : public Actor
{
public:
    int8_t timer = 0;
    uint8_t score = 0;
    int8_t player;
    bool isAi = false;
	
    Bat(int8_t playerIn, bool isAiIn)
    {
        this->isAi = isAiIn;
        this->player = playerIn;
	    if(player == 1)
	    {
            this->size = Size(16, 48);
            loc.x = 0;
            spriteLocation.w = size.w / 8;
            spriteLocation.h = size.h / 8;
            spriteLocation.x = 0;
            spriteLocation.y = 0;
	    }
        else
        {
            this->size = Size(16, 48);
            loc.x = maxX - size.w;
            spriteLocation.w = size.w / 8;
            spriteLocation.h = size.h / 8;
            spriteLocation.x = 0;
            spriteLocation.y = 0;
        }

        this->size.h = 48;
        this->size.w = 16;
    }
	
    void Update(Point ballLocation, int8_t aiOffset, Size ballSize)
    {
        if(timer >= 0)
        {
            timer--;
        }

        int yMovement;
    	
    	if(this->isAi)
    	{
            yMovement = this->Ai(ballLocation, aiOffset, ballSize);
    	}
        else
        {
	        
        }

        this->loc.y = std::min(maxY, std::max(0, loc.y + yMovement));

    	
    	// todo frames
    }

    int32_t Ai(Point ballLocation, int8_t aiOffset, Size ballSize)
    {
        auto xDistance = abs((ballLocation.x + (ballSize.w /2)) - this->loc.x);

        auto targetY1 = maxY / 2;

        auto targetY2 = (ballLocation.y + (ballSize.h / 2)) + aiOffset;

        auto weight1 = std::min(1, xDistance / maxX / 2);
        auto weight2 = 1 - weight1;

        auto targetY = (weight1 * targetY1) + (weight2 * targetY2);

        return std::min(MAX_AI_SPEED, std::max(-MAX_AI_SPEED, targetY - this->loc.y));
    }
};

class Ball : public Actor
{  
public:
    float dX = 0;
    float dY = 0;

    Ball()
    {
	    
    }
	
    Ball(int dxIn) : Actor()
    {
        this->dX = dxIn;
        this->loc.x = maxX / 2;
        this->loc.y = maxY / 2;

    	// update size
        this->size = Size(16, 16);

        this->spriteLocation = Rect(0, 48 / 8, size.w / 8, size.h / 8);
    }
	
    int16_t speed = 5;

    void Update(std::vector<Bat> bats)
    {
        for (int i = 0; i < speed; i++)
        {
            auto original_x = loc.x;

            loc.x += this->dX;
            loc.y += this->dY;

        	// todo collision detection

            auto ballBounds = Rect(loc, size);
        	
            for (auto bat : bats)
            {
                if(IsRectIntersecting(Rect(bat.GetLocation(), bat.GetSize()), ballBounds))
                {
                    auto newDirX = 1;

                    if (bat.GetLocation().x > maxX / 2)
                    {
                        newDirX = -1;
                    }

                    //auto differenceY = loc.y - bat.GetLocation().y;
                    auto differenceY = loc.y - (bat.GetLocation().y + (bat.GetSize().h / 2));

                    dX = -dX;

                    dY += differenceY / bat.GetSize().h;
                	
                    dY = std::min(std::max(dY, (float)-1), (float)1);

                    auto norm = Normalised(dX, dY);

                    dX = std::get<0>(norm);
                    dY = std::get<1>(norm);
                	
                    this->speed += 1;

                	// todo bat glow AI and sounds
                }                    	
            }

        	if (this->loc.y < 0 || this->loc.y >= maxY)
        	{
                this->dY = -this->dY;
                this->loc.y += this->loc.y;

        		// todo sounds
        	}
        }
    }

	bool Out()
    {
        return loc.x < 0 || loc.x > maxX;
    }
};


class Game
{
public:
    Ball ball;
	
    std::vector<Bat> bats;

    std::vector<Actor> impacts;

    int8_t aiOffset = 0;

    Game()
    {
        ball = Ball(-1);

        Bat batLeft = Bat(1, true);

        bats.emplace_back(batLeft);

        Bat batRight = Bat(2, true);

        bats.emplace_back(batRight);

    }
	
	void Update()
	{
        ball.Update(bats);

        for (Bat &bat : bats) {
            bat.Update(ball.GetLocation(), aiOffset, ball.GetSize());
        }

		// todo impacts
		
		if(ball.Out())
		{
            ball = Ball(-1);
		}
	}

	
};

int8_t noPlayers = 1;
Game game;

enum GameState {
    Menu,
    Play,
    GameOver
};

GameState state = Menu;
SpriteSheet* backGroundSurface;

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::hires);

    if (screen.sprites != nullptr)
    {
        screen.sprites->data = nullptr;
        screen.sprites = nullptr;
    }

   screen.sprites = SpriteSheet::load(sprites_data);
	  
   backGroundSurface = SpriteSheet::load(background_data);
}

void DrawMenu()
{

}

void DrawGame()
{
    screen.stretch_blit(backGroundSurface, Rect(0, 0, 128, 96), Rect(0, 0, 320, 240));
	
    screen.sprite(game.ball.GetSpriteLocation(), game.ball.GetLocation(), 0);

    for (auto bat : game.bats)
    {
        screen.sprite(bat.GetSpriteLocation(), bat.GetLocation(), 0);
    }
}

void DrawGameOver()
{

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
    /*screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(0, 0, 320, 14));
    screen.pen = Pen(0, 0, 0);
    screen.text("Hello 32blit!", minimal_font, Point(5, 4));*/
   
    switch (state)
    {
    case Menu:
        break;
    case Play:
        DrawGame();
        break;
    case GameOver:
        break;
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
	switch(state)
	{
    case Menu:
        game = Game();
        state = Play;
        break;
    case Play:
        game.Update();
        break;
    case GameOver:
        break;
	}
}