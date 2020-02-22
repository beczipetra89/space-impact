#pragma once

class Alien2 : public GameObject
{
public:
	virtual ~Alien2() { SDL_Log("Alien2::~Alien2"); }

	virtual void Init(float y)
	{
		SDL_Log("Alien2::Init");
		verticalPosition = y;
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}
		if (m == HIT) {
			// Disabe alien2 when it's hit
			SDL_Log("Alien2::HIT!");
			Send(ALIEN2_HIT); // Send a message so that game can update the score and init new alien2
			enabled = false;
		}
	}
};

class Alien2BehaviourComponent : public Component
{
	ObjectPool<Bomb>* bombs_pool;

	float time_laser_shot;	// time from the last time the laser was shot
	float init_delay;       // time delay before alien start to move


public:
	virtual ~Alien2BehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		time_laser_shot = -10000.f;
		go->horizontalPosition = 640;
		// TODO: remove me
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

		Move(dt * ALIEN_SPEED);

		if (go->horizontalPosition < -40) // When alian flew out of window to the left, it disappears, 40 is alien sprite width
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

	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_laser_shot) < (ALIEN_LASER_TIME_INTERVAL / game_speed))
			return false;

		time_laser_shot = engine->getElapsedTime();

		SDL_Log("Alien fire!");
		return true;
	}


};