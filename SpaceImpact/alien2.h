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
	ObjectPool<LaserS>* laser_s_pool;
	float time_laser_shot;	// time from the last time the laser was shot

public:
	virtual ~Alien2BehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<LaserS>* laser_s_pool)
	{
		Component::Create(engine, go, game_objects);
		this->laser_s_pool = laser_s_pool;
	}

	virtual void Init()
	{
		time_laser_shot = -10000.f;
		go->horizontalPosition = 850;
	}

	virtual void Update(float dt)
	{
		Move(dt * ALIEN_SPEED);

		if (go->horizontalPosition < -840) // When alien2 flew out of window to the left, it disappears
		{
			go->Send(ALIEN_LEVEL_CLEAR);
			go->enabled = false;
		}

		if (CanFire())
		{
			// fetches a laser from the pool and use it in game_objects
			LaserS* laser = laser_s_pool->FirstAvailable();
			if (laser != NULL)	// laser is NULL is the object pool can not provide an object
			{
				laser_s_pool->SelectRandom();
				engine->PlaySFX("data/audio/laser_sound.wav", 0, -1);
				laser->Init(go->horizontalPosition, go->verticalPosition);
				game_objects->insert(laser);
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