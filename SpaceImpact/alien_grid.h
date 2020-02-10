#pragma once

class AlienGrid : public GameObject {

public:
	virtual ~AlienGrid() { SDL_Log("AlienGrid::~AlienGrid"); }
	bool stop_update; // if game over then we don't need to update Alien

	virtual void Init()
	{
		SDL_Log("AlienGrid::Init");
		GameObject::Init();
		alien_grid_direction = 1;
		stop_update = false;
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

		// Get message from one alien that it hits the wall, 
		// we then send messages to all the aliens in the grid to change the direction and move downwards
		if (m == ALIEN_HIT_WALL)
		{
			SDL_Log("Alilen::ALIEN_HIT_WALL");
			Send(ALIEN_CHANGE_DIRECTION_AND_MOVE_DOWN);
		}

		// If we get a GAME_OVER message, stop updating all allens in the grid by setting stop_update flag to true
		if (m == GAME_OVER)
		{
			SDL_Log("Alilen::GAME_OVER!");
			stop_update = true;
		}
	}

	int alien_grid_direction; // if direction = 1, all aliens move right, if direction = -1, aliens move left
};

class AlienGridBehaviourComponent : public Component {	 // needs access to the object pools: aliens, bombs
	ObjectPool<Alien> * aliens_pool;
	ObjectPool<Bomb> * bombs_pool;

	float time_bomb_dropped;	// time from the last time the bomb was dropped
	bool all_aliens_killed; // if all aliens are killed
public:
	virtual ~AlienGridBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Alien> * aliens_pool, ObjectPool<Bomb> * bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->aliens_pool = aliens_pool;
		this->bombs_pool = bombs_pool;
	}

	virtual void Init() {
		SDL_Log("AlienGridBehaviourComponent::Init");
		Component::Init();

		time_bomb_dropped = -10000.f;
		all_aliens_killed = false;
	}

	virtual void Update(float dt)
	{
		// We will skip the rest of the update functions if all aliens are killed
		if (all_aliens_killed) {
			return;
		}

		// If alien_grid stop_update flag set to true, then we stop updating all aliens and drop bombs
		// Use "(AlienGrid *) go" to cast variable "go" from GameObject type to AlienGrid type
		// get aliengrid stop_update flag
		if (((AlienGrid*)go)->stop_update) {
			return;
		}

		// Use "(AlienGrid *) go" to cast variable "go" from GameObject type to AlienGrid type
		// get aliengrid direction
		int alien_grid_direction = ((AlienGrid *) go)->alien_grid_direction;

		for (auto it = aliens_pool->pool.begin(); it != aliens_pool->pool.end(); it++) {
			(*it)->Update(dt);
		}

		if (AreAllAliensKilled())
		{
			// Set all_aliens_killed flag to true so that we don't have to run Update() anymore
			all_aliens_killed = true;
			go->Send(LEVEL_WIN);
			// Skip firing logic below
			return;
		}

		if (CanFire())
		{
			// fetches a rocket from the pool and use it in game_objects
			Bomb* bomb = bombs_pool->FirstAvailable();
			if (bomb != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				Alien* alien = aliens_pool->SelectRandom();
				bomb->Init(alien->horizontalPosition, alien->verticalPosition);
				game_objects->insert(bomb);
			}
		}
	}

	bool AreAllAliensKilled()
	{
		bool enabledAlienExists = false;
		// try to find the first alien that is enabled
		for (auto it = aliens_pool->pool.begin(); it != aliens_pool->pool.end(); it++) {
			if ((*it)->enabled == true)
			{
				enabledAlienExists = true;
				// it's enough we find one alien that is alive, don't need to continue
				break;
			}
		}

		// If there is at least one alien is enabled, then it means not all aliens are killed
		return !enabledAlienExists;
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
