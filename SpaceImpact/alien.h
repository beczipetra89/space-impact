#pragma once

#include <cstdlib>
#include <ctime>

class AlienBehaviourComponent : public Component
{

	int direction = 1; // if direction = 1, alien moves right, if direction = -1, alien moves left
	ObjectPool<Bomb>* bombs_pool;
	
	float time_bomb_dropped;	// time from the last time the bomb was dropped


public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		time_bomb_dropped = -10000.f;

		go->horizontalPosition = 600; 
	
		int min = 64;
		int max = 412;
	
		srand((unsigned)time(0));
		int r = (int)rand() / (int)RAND_MAX;
		go->verticalPosition= min + r * (max - min);


		// Init alien on random height
		
		//go->verticalPosition = (rand() % 439) + 1;
		
	//	go->verticalPosition = 64 + (std::rand() % (64 - 416 + 1)); 

	//	go->verticalPosition = RandomHeight(64, 416);



	}

	virtual void Update(float dt)
	{

		Move(dt * ALIEN_SPEED );
		
		if (CanFire())
		{
			// fetches a rocket from the pool and use it in game_objects
			Bomb* bomb = bombs_pool->FirstAvailable();
			if (bomb != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				//Alien* alien = aliens_pool->SelectRandom
				bombs_pool->SelectRandom();
				bomb->Init(go->horizontalPosition, go->verticalPosition);
				game_objects->insert(bomb);
			}
		}

		/* // fetches a rocket from the pool and use it in game_objects
				Rocket * rocket = rockets_pool->FirstAvailable();
				if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
				{
					rocket->Init(go->horizontalPosition, go->verticalPosition);
					game_objects->insert(rocket);
		*/
	


		/*	xpos = xpos - 1 * horizontialSpeed;
		if (ypos < initialYpos - 20) {
			// reached upper limit, fly downwards instead
			verticalDirection = 1;
		}
		else if (ypos > initialYpos + 20) {
			verticalDirection = -1;
		}
		ypos = ypos + verticalSpeed * verticalDirection;  */

	}

	// move the alien  to left
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
		if ((engine->getElapsedTime() - time_bomb_dropped) < (BOMB_TIME_INTERVAL / game_speed))
			return false;

		time_bomb_dropped = engine->getElapsedTime();

		SDL_Log("bomb!");
		return true;
	}

};


class Alien : public GameObject
{
public:

	int lives;

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init()
	{
		SDL_Log("Alien::Init");
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
				enabled = false;
		}
	}
};