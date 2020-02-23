#pragma once

// BOSS ENEMY, MOVES UP AND DOWN AND SHOOTS MULTIPLE MINES (accelerating frequency by elapsed time)

class BossAlienBehaviourComponent : public Component
{

	ObjectPool<Mine>* mines_pool;

	int vertical_direction = 1; // if direction = 1, alien moves down, if direction = -1, alien moves up
	float time_moved;	// time from the last time boss alien moved
	float time_mine_dropped;	// time from the last time the bomb was dropped
	float delay;
	float init_time;

public:
	virtual ~BossAlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Mine>* mines_pool)
	{
		Component::Create(engine, go, game_objects);
		this->mines_pool = mines_pool;
		SDL_Log("Mines_pool size: %d", mines_pool->pool.size());
	}

	virtual void Init()
	{
		time_mine_dropped = -10000.f;
		init_time = engine->getElapsedTime();
		go->verticalPosition = 160;
		go->horizontalPosition = 600;
	}

	virtual void Update(float dt)
	{

		Move(dt);
		//if (go->horizontalPosition < 500) {}
		// go->enabled = false; 

		if (CanFire())
		{
			// Access mines_pool's pool and get 3 mines
			int i = 1;
			for (auto it = mines_pool->pool.begin(); it != mines_pool->pool.end(); it++) {
				if (!(*it)->enabled)
				{
					engine->PlaySFX("data/audio/boss_shoot.wav", 0, -1);
					(*it)->Init(go->horizontalPosition, go->verticalPosition + i * 100.f);
					game_objects->insert((*it));
					i++;
				}

				// Quit the loop when we get 3 mines
				if (i > 3) break;
			}
		}

	}


	void Move(float dt)
	{
		// move the boss alien to left until it reaches 450
		if (go->horizontalPosition > 450) {
			go->horizontalPosition -= dt * BOSS_SPEED;
			delay = engine->getElapsedTime() + 2.f;
		}
		else if (delay != NULL && (engine->getElapsedTime() > delay))

		{
			go->verticalPosition += dt * BOSS_V_SPEED * vertical_direction;
			// When it reaches top or bottom limit, switch move direction
			if (go->verticalPosition > 160 || go->verticalPosition < 30) {
				vertical_direction = vertical_direction * -1;
			}
		}
	}


	bool CanFire()
	{
		// If the vertical movement delay is not set or vertical movement hasn't started yet.
		// Don't shoot.
		if (delay == NULL || (engine->getElapsedTime() < delay)) 
			return false;

		float time_alive = engine->getElapsedTime() - init_time;
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_mine_dropped) < (MINE_TIME_INTERVAL / time_alive))
			return false;

		time_mine_dropped = engine->getElapsedTime();

		SDL_Log("CanFire mine!");
		return true;
	}

};


class BossAlien : public GameObject
{
public:

	int lives;
	bool isVulnerable;

	virtual ~BossAlien() { SDL_Log("BossAlien::~BossAlien"); }

	virtual void Init()
	{
		SDL_Log("BossAlien::Init");
		GameObject::Init();
		lives = BOSS1_LIVES;
		isVulnerable = false;

	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

		if (m == HIT) {
			SDL_Log("BossAlien::HIT!");
			Send(BOSS_HIT); 
			RemoveLife();
		}
	}

	void RemoveLife()
	{
		if (horizontalPosition > 450) {}
		else 
			lives--;
		
		if (lives < 0) {
			Send(BOSS_KILLED);
			enabled = false;
		}

		SDL_Log("Boss - remaining lives %d", lives);

	}
};

