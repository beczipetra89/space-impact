#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"
#include <set>

const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 3;
const unsigned int	BOSS1_LIVES = 80;
const unsigned int	BOSS2_LIVES = 160;
const float			BACKGROUND_SPEED = 60.f;  
const unsigned int	MAX_NUM_ROCKETS = 100;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 10;
float				FIRE_TIME_INTERVAL = .10f;	
const float			ALIEN_LASER_TIME_INTERVAL = 10.25f;		
const float			ALIEN_2_LASER_TIME_INTERVAL = 0.5f;
const float			BOMB_SPEED = 200.0f;		
const float			PLAYER_SPEED = 140.0f;		
float				ROCKET_SPEED = 140.0f;		
float				LASERBEAM_SPEED = 80;
const unsigned int	MAX_NUM_ALIENLASER = 32;
float				ALIENLASER_TIME_INTERVAL = 3.25f; 
float				ALIENLASER_SPEED = 160.0f;
const float			ALIEN_SPEED = 80.0f;			
const float			ALIEN_V_SPEED = 80.0f;	
const float			ALIEN_G_SPEED = 80.0f;	
const float			ALIEN_G_VERTICAL_SPEED = 80.0f;  
const float         ALIEN_G_VERTICAL_MOVE_TIME_INTERNAL = 1000.f; 
const float			PICKUP_SPEED = 40.0f;
const unsigned int	MAX_NUM_MINES = 32;
const float			MINE_TIME_INTERVAL = 60.f; 
const float			MINE_SPEED = 120.0f;
const float			BOSS_SPEED = 40.0f;
const float         BOSS_V_SPEED = 80.0f;

float game_speed = 1.2f;		

#include "component.h"
#include "game_object.h"
#include "vector_2d.h"

#include "bg1.h"
#include "bg2.h"
#include "bg3.h"
#include "bg4.h"
#include "banner.h"
#include "life_icon.h"
#include "pickup_life.h"

#include "rocket.h"
#include "laser_beam.h"
#include "mines.h"
#include "cube.h"
#include "laserS.h"
#include "alienLaser.h"

#include "player.h"
#include "alien.h"
#include "alien2.h"
#include "alien_g.h"
#include "alien_g_grid.h"
#include "alien_v.h"
#include "alien_v_grid.h"
#include "boss_alien.h"
#include "boss_alien2.h"

#include "game.h"


int main(int argc, char** argv)
{
	// Seed the random number generator
	srand(time(NULL));

	AvancezLib engine;

	engine.init(840, 580); //640, 480
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

