#include "blitboing.hpp"
#include "assets.hpp"
using namespace blit;

int32_t maxX = 320;
int32_t minX = 0;
int32_t maxY = 230;
int32_t minY = 0;

int32_t PLAYER_SPEED = 6;
int32_t MAX_AI_SPEED = 8;

float MAX_BALL_SPEED = 4;

uint8_t BAT_GLOW_TIME = 45;

int8_t spriteSize = 8;

float DEFLECTION_MIN = 0.0;
float DEFLECTION_MAX = 1.0;
float BAT_HIT_MIN = 0.0;
float BAT_HIT_MAX = 24.0;

std::tuple<float, float> Normalised(float x, float y)
{
	float length = std::hypot(x, y);
	
	return std::tuple<float, float>(x / length, y / length);
}


bool IsRectIntersecting(Rect rect1, Rect rect2) {
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
	Rect mainSprite;
	Rect hitSprite;

	Bat(int8_t playerIn, bool isAiIn)
	{
		this->isAi = isAiIn;
		this->player = playerIn;
		if (player == 1)
		{
			this->size = Size(8, 48);
			loc.x = size.w;
			mainSprite.w = size.w / 8;
			mainSprite.h = size.h / 8;
			mainSprite.x = 0;
			mainSprite.y = 0;

			hitSprite.w = 1;
			hitSprite.h = 7;
			hitSprite.x = 2;
			hitSprite.y = 0;
		}
		else
		{
			this->size = Size(8, 48);
			loc.x = maxX - (size.w * 2);
			mainSprite.w = size.w / 8;
			mainSprite.h = size.h / 8;
			mainSprite.x = 1;
			mainSprite.y = 0;

			hitSprite.w = 1;
			hitSprite.h = 7;
			hitSprite.x = 3;
			hitSprite.y = 0;
		}
	}

	void Update(Point ballLocation, int8_t aiOffset, Size ballSize)
	{
		if (timer > 0)
		{
			spriteLocation = hitSprite;
			timer--;
		}
		else
		{
			spriteLocation = mainSprite;
		}

		int yMovement;

		if (this->isAi)
		{
			yMovement = this->Ai(ballLocation, aiOffset, ballSize);
		}
		else
		{
			if (player == 1)
			{
				yMovement = PlayerUpdate(DPAD_UP, DPAD_DOWN);

				if(yMovement == 0)
				{
				yMovement =	PlayerUpdate(joystick.y);
				}
			}
			else
			{
				yMovement = PlayerUpdate(X, B);
			}
		}
		
		this->loc.y = std::min((maxY - this->size.h), std::max((int32_t)0, loc.y + yMovement));


		// todo frames
	}

	int32_t PlayerUpdate(Button upButton, Button downButton)
	{

		if (buttons & downButton)
		{
			return PLAYER_SPEED;
		}
		else if (buttons & upButton)
		{
			return PLAYER_SPEED * -1;
		}

		return 0;
	}

	int32_t PlayerUpdate(float y)
	{

		if (y > 0)
		{
			return PLAYER_SPEED;
		}
		else if (y < 0)
		{
			return PLAYER_SPEED * -1;
		}

		return 0;
	}

	int32_t Ai(Point ballLocation, int8_t aiOffset, Size ballSize)
	{
		auto xDistance = abs((ballLocation.x + (ballSize.w / 2)) - this->loc.x);

		// Only follow the ball if close
		if (xDistance < maxX / 2)
		{
			auto targetY1 = maxY / 2;

			auto targetY2 = (ballLocation.y + (ballSize.h / 2)) + aiOffset;

			auto weight1 = std::min((int32_t)1, xDistance / (maxX / (int32_t)2));
			auto weight2 = 1 - weight1;

			auto targetY = (weight1 * targetY1) + (weight2 * targetY2);

			return std::min(MAX_AI_SPEED, std::max(-MAX_AI_SPEED, targetY - this->loc.y));
		}
		// Head to center
		else
		{
			if ((this->loc.y + this->size.h / 2) < maxY / 2)
			{
				return MAX_AI_SPEED;
			}
			else if ((this->loc.y + this->size.h / 2) > maxY / 2)
			{
				return MAX_AI_SPEED * -1;
			}

		}

		return 0;
	}
};

class Ball : public Actor
{
public:
	float dX = 0.0f;
	float dY = 0.0f;

	float xFloat = 0.0f;
	float yFloat = 0.0f;
	
	Ball()
	{

	}

	Ball(int dxIn, int dYin) : Actor()
	{
		this->dX = static_cast<float>(dxIn);
		this->loc.x = maxX / 2;
		this->loc.y = maxY / 2;
		this->dY = static_cast<float>(dYin);

		this->xFloat = loc.x;
		this->yFloat = loc.y;
		
		// update size
		this->size = Size(8, 8);

		this->spriteLocation = Rect(0, 64 / 8, size.w / 8, size.h / 8);
	}

	float speed = 1;

	bool Update(std::vector<Bat>& bats)
	{
		bool hit = false;
		
		for (int i = 0; i < floor(speed); i++)
		{
			auto original_x = loc.x;

			auto prevBallBounds = Rect(loc, size);

			xFloat += this->dX;
			yFloat += this->dY;
			
			loc.x = static_cast<int>(round(this->xFloat));
			loc.y = static_cast<int>(round(this->yFloat));

			auto ballBounds = Rect(loc, size);
			
			for (auto& bat : bats)
			{
				if (IsRectIntersecting(Rect(bat.GetLocation(), bat.GetSize()), ballBounds) && !IsRectIntersecting(Rect(bat.GetLocation(), bat.GetSize()), prevBallBounds))
				{
					hit = true;
					if(bat.timer == 0)
					{
						bat.timer = BAT_GLOW_TIME;
					}
								
					auto differenceY = (loc.y + (this->size.h / 2)) - (bat.GetLocation().y + (bat.GetSize().h / 2));
					
					dX = -dX;

					dY = 0;

					int yMultiplier = 1;

					if (differenceY < 0)
					{
						differenceY = differenceY * -1;
						yMultiplier = -1;
					}

					float slope = (DEFLECTION_MAX - DEFLECTION_MIN) / (BAT_HIT_MAX - BAT_HIT_MIN);

					dY = DEFLECTION_MIN + slope * (static_cast<float>(differenceY) - BAT_HIT_MIN);

					dY = dY * static_cast<float>(yMultiplier);

					if (speed < MAX_BALL_SPEED)
					{
						this->speed += 0.1;
					}

					// todo bat glow AI and sounds				
				}
			}

			if (this->loc.y <= 0 || this->loc.y >= maxY)
			{
				this->dY = -this->dY;
				this->loc.y += static_cast<int>(round(this->dY));

				// todo sounds
			}
		}

		return hit;
	}

	bool Out()
	{
		return loc.x < 0 || loc.x > maxX;
	}
};

Ball CreateBall()
{
	auto dirX = blit::random() % 2;
	if (dirX == 0)
	{
		dirX = -1;
	}

	auto dirY = blit::random() % 3;

	if (dirY == 2)
	{
		dirY = -1;
	}

	return Ball(dirX, dirY);
}

int8_t noPlayers = 1;
uint8_t maxScore = 5;

enum GameState {
	Menu,
	Play,
	GameOver
};

GameState state = Menu;
SpriteSheet* backGroundSurface;
SpriteSheet* menu0ss;
SpriteSheet* menu1ss;
SpriteSheet* gameOver;

class Game
{
public:
	Ball ball;

	std::vector<Bat> bats;

	std::vector<Actor> impacts;

	int8_t aiOffset = 0;

	int8_t vibrationTimer = 0;

	Game()
	{

	}

	Game(int8_t noPlayers)
	{

		ball = CreateBall();

		Bat batLeft = Bat(1, false);

		bats.emplace_back(batLeft);

		Bat batRight = Bat(2, noPlayers == 1);

		bats.emplace_back(batRight);

	}

	void Update()
	{
		if(vibrationTimer > 0)
		{
			blit::vibration = 1;
			vibrationTimer--;
		}
		else
		{
			blit::vibration = 0;
		}

		const auto hit = ball.Update(bats);

		if(hit == true)
		{
			vibrationTimer = BAT_GLOW_TIME / 2;
		}

		for (Bat& bat : bats) {
			bat.Update(ball.GetLocation(), aiOffset, ball.GetSize());
		}

		// todo impacts

		if (ball.Out())
		{
			vibrationTimer = BAT_GLOW_TIME;
			
			if(ball.loc.x < maxX / 2)
			{
				bats[1].score++;
				bats[1].timer = BAT_GLOW_TIME;
			}
			else
			{
				bats[0].score++;
				bats[0].timer = BAT_GLOW_TIME;
			}

			for (Bat& bat : bats) {
				if(bat.score >= maxScore)
				{
					state = GameOver;
				}
			}
			auto dirX = blit::random() % 2;
			if(dirX == 0)
			{
				dirX = -1;
			}

			auto dirY = blit::random() % 3;

			if(dirY == 2)
			{
				dirY = -1;
			}
			
			ball = CreateBall();
		}
	}


};

Game game;

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

	menu0ss = SpriteSheet::load(menu0);
	menu1ss = SpriteSheet::load(menu1);
	gameOver = SpriteSheet::load(over);
}

void DrawMenu()
{
	switch (noPlayers)
	{
	case 1:
	default:
		screen.stretch_blit(menu0ss, Rect(0, 0, 128, 96), Rect(0, 0, 320, 240));
		break;
	case 2:
		screen.stretch_blit(menu1ss, Rect(0, 0, 128, 96), Rect(0, 0, 320, 240));
		break;
	}
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
	screen.stretch_blit(gameOver, Rect(0, 0, 128, 96), Rect(0, 0, 320, 240));
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

	switch (state)
	{
	case Menu:
		DrawMenu();
		break;
	case Play:
		DrawGame();
		break;
	case GameOver:
		DrawGameOver();
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

	static uint16_t lastButtons = 0;
	uint16_t changed = blit::buttons ^ lastButtons;
	uint16_t pressed = changed & blit::buttons;
	uint16_t released = changed & ~blit::buttons;

	switch (state)
	{
	case Menu:
		if (buttons & Button::A)
		{
			game = Game(noPlayers);
			state = Play;
		}
		else if (buttons & Button::DPAD_UP || joystick.y < 0)
		{
			noPlayers = 1;
		}
		else if (buttons & Button::DPAD_DOWN || joystick.y > 0)
		{
			noPlayers = 2;
		}
		break;
	case Play:
		if (buttons & Button::A)
		{
			game = Game(noPlayers);
			state = Play;
		}
		game.Update();
		break;
	case GameOver:
		if (buttons & Button::A)
		{
			state = Menu;
		}
		break;
	}

	lastButtons = blit::buttons;
}