#pragma once

#include <cstdlib>
#include <ctime>

class AlienBehaviourComponent : public Component
{

	int direction = 1; // if direction = 1, alien moves right, if direction = -1, alien moves left
	ObjectPool<Bomb>* bombs_pool;
	
	float time_laser_shot;	// time from the last time the laser was shot
	float init_delay;       // time delay before alien start to move


public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		time_laser_shot = -10000.f;
		go->horizontalPosition = 640; 
		//go->verticalPosition= 300;
		init_delay = engine->getElapsedTime();
	}

	void setInitDelay(float delay)
	{
		init_delay = engine->getElapsedTime() + delay;
	}

	virtual void Update(float dt)
	{
		// If we haven't passed the delay time, skip update
		if (engine->getElapsedTime() < init_delay)
			return;

		Move(dt * ALIEN_SPEED );
		if (go->horizontalPosition < -40) // When alian flew out of window to the left, it disappears.
		{
			go->Send(ALIEN_LEVEL_CLEAR);
			go->enabled = false;
		}

		if (CanFire())
		{
			// fetches a rocket from the pool and use it in game_objects
			Bomb* bomb = bombs_pool->FirstAvailable();
			if (bomb != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				//Alien* alien = aliens_pool->SelectRandom
				bombs_pool->SelectRandom();
				engine->PlaySFX("data/audio/laser_sound.wav", 0, -1);
				bomb->Init(go->horizontalPosition, go->verticalPosition);
				game_objects->insert(bomb);
			}
		}

	}

	// move the alien to left
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		go->horizontalPosition -= move;
	}


	int RandomHeight(int min, int max)
	{
		srand((unsigned)time(0));
		int r = (int)rand() / (int)RAND_MAX;
		return min + r * (max - min);
	}


	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_laser_shot) < (BOMB_TIME_INTERVAL / game_speed))
			return false;

		time_laser_shot = engine->getElapsedTime();

		SDL_Log("bomb!");
		return true;
	}


};


class Alien : public GameObject
{
public:

	int lives;

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init(float y)
	{
		SDL_Log("Alien::Init");
		verticalPosition = y;
		GameObject::Init();
		
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
				return;
		}
		if (m == HIT) {
			// Disabe alien when it's hit
			SDL_Log("Alien::HIT!");
			Send(ALIEN_HIT); // Send a message so that game can update the score
			Send(ALIEN_LEVEL_CLEAR); // Send a message so that game can init new alien
			enabled = false;
		}
	}
};