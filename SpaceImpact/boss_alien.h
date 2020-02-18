#pragma once

// BOSS ENEMY, MOVES BACK AND FORTH AND SHOOTS MULTIPLE BULLETS

class BossAlienBehaviourComponent : public Component
{

	int direction = 1; // if direction = 1, alien moves right, if direction = -1, alien moves left
	ObjectPool<Mine>* mines_pool;

	float time_moved;	// time from the last time boss alien moved
	//int direction = 1;

	float time_mine_dropped;	// time from the last time the bomb was dropped


public:
	virtual ~BossAlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Mine>* mines_pool)
	{
		Component::Create(engine, go, game_objects);
		this->mines_pool = mines_pool;
	}

	virtual void Init()
	{
		time_mine_dropped = -10000.f;

		go->verticalPosition = 210;
		go->horizontalPosition = 640;
	}

	virtual void Update(float dt)
	{

		Move(dt * BOSS_SPEED);
		if (go->horizontalPosition < 500) {}
		// go->enabled = false; 

		if (CanFire())
		{
			// fetches a mine from the pool and use it in game_objects
			Mine* mine = mines_pool->FirstAvailable();
			if (mine != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				
				mines_pool->SelectRandom();
				mine->Init(go->horizontalPosition, go->verticalPosition);
				game_objects->insert(mine);
			}
		}

	}

	// move the boss alien to left
	
	void Move(float move)
	{

		// wMoved in screen (xpos = 500)
		go->horizontalPosition -= move;

		if (go->horizontalPosition < 500) {
		
		}




		
		

		

	}





	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_mine_dropped) < (MINE_TIME_INTERVAL / game_speed))
			return false;

		time_mine_dropped = engine->getElapsedTime();

		SDL_Log("bomb!");
		return true;
	}

};


class BossAlien : public GameObject
{
public:

	int lives;

	virtual ~BossAlien() { SDL_Log("BossAlien::~BossAlien"); }

	virtual void Init()
	{
		SDL_Log("BossAlien::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {

			return;
		}
		if (m == HIT) {
			// Disabe alien when it's hit
			SDL_Log("AlBossAlienien::HIT!");
			Send(BOSS_HIT); // Send a message so that game can update the score
			enabled = false;
		}
	}
};