#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>



const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 2;

const unsigned int	MAX_NUM_ROCKETS = 100;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 10;
float				FIRE_TIME_INTERVAL = .08f;		// use .05f for testing
const float			BOMB_TIME_INTERVAL = 10.25f;
const float			BOMB_SPEED = 120.0f;
const float			PLAYER_SPEED = 120.0f;		// 160.0f
float				ROCKET_SPEED = 140.0f;		 // 160.0f

const unsigned int	MAX_NUM_ALIENLASER = 32;
float				ALIENLASER_TIME_INTERVAL = 3.25f; 
float				ALIENLASER_SPEED = 120.0f;


const float			ALIEN_SPEED = 80.0f;		// 40.0f
const float			ALIEN_V_SPEED = 40.0f;		// 40.0f
const float			ALIEN_G_SPEED = 40.0f;
const float			ALIEN_G_VERTICAL_SPEED = 20.0f;
const float         ALIEN_G_VERTICAL_MOVE_TIME_INTERNAL = 1000.f;

const float			PICKUP_SPEED = 40.0f;

const unsigned int	MAX_NUM_MINES = 32;
const float			MINE_TIME_INTERVAL = 10.25f;
const float			MINE_SPEED = 120.0f;
const float			BOSS_SPEED = 80.0f;



float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier


#include "component.h"
#include "game_object.h"


#include "rocket.h"
#include "player.h"
#include "bomb.h"
#include "alien.h"
//#include "alien_grid.h"
#include "alien_g.h"
#include "alien_g_grid.h"
#include "alienLaser.h"
#include "alien_v.h"
#include "alien_v_grid.h"
#include "pickup_life.h"

#include "mines.h"
#include "boss_alien.h"
#include "boss_alien_manager.h"

#include "game.h"




int main(int argc, char** argv)
{
	// Seed the random number generator
	srand(time(NULL));

	AvancezLib engine;

	engine.init(640, 480);
	Game game;
	game.Create(&engine);
	game.Init();

	float lastTime = engine.getElapsedTime();
	while (game.enabled)
	{
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		engine.processInput();
		game.Update(dt);
		game.Draw();
	}

	// clean up
	game.Destroy();
	engine.destroy();

	return 0;
}



