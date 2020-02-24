#pragma once

// BOSS ENEMY 2, MOVES UP AND DOWN AND SHOOTS MULTIPLE MINES (accelerating frequency by elapsed time)

class BossAlien2BehaviourComponent : public Component
{

	ObjectPool<Cube>* cubes_pool;

	int vertical_direction = 1; // if direction = 1, alien moves down, if direction = -1, alien moves up
	float time_moved;	// time from the last time boss alien moved
	float time_mine_dropped;	// time from the last time the bomb was dropped
	float delay;
	float init_time;

public:
	virtual ~BossAlien2BehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Cube>* cubes_pool)
	{
		Component::Create(engine, go, game_objects);
		this->cubes_pool = cubes_pool;
		SDL_Log("Cubes_pool size: %d", cubes_pool->pool.size());
	}

	virtual void Init()
	{
		time_mine_dropped = -10000.f;
		init_time = engine->getElapsedTime();
		go->verticalPosition = 120;
		go->horizontalPosition = 840;
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
			for (auto it = cubes_pool->pool.begin(); it != cubes_pool->pool.end(); it++) {
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
		// move the boss alien to left until it reaches 600
		if (go->horizontalPosition > 600) {
			go->horizontalPosition -= dt * BOSS_SPEED;
			delay = engine->getElapsedTime() + 2.f;
		}
		else if (delay != NULL && (engine->getElapsedTime() > delay))

		{
			go->verticalPosition += dt * BOSS_V_SPEED * vertical_direction;
			// When it reaches top or bottom limit, switch move direction
			if (go->verticalPosition > 260 || go->verticalPosition < 30) {
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


class BossAlien2 : public GameObject
{
public:

	int lives;
	bool isVulnerable;

	virtual ~BossAlien2() { SDL_Log("BossAlien2::~BossAlien"); }

	virtual void Init()
	{
		SDL_Log("BossAlien2::Init");
		GameObject::Init();
		lives = BOSS2_LIVES;
		isVulnerable = false;

	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

		if (m == HIT) {
			SDL_Log("BossAlien2::HIT!");
			Send(BOSS_HIT);
			RemoveLife();
		}
	}

	void RemoveLife()
	{
		if (horizontalPosition > 600) {}
		else
			lives--;

		if (lives < 0) {
			Send(BOSS_KILLED);
			enabled = false;
		}

		SDL_Log("Boss 2 - remaining lives %d", lives);

	}
};


