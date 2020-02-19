#pragma once

class AlienGGrid : public GameObject {

public:
	float start_delay; // a time delay, after which the game object in pool can perform update()

	virtual ~AlienGGrid() { SDL_Log("AlienGGrid::~AlienGGrid"); }

	virtual void Init(float start_delay)
	{
		SDL_Log("AlienGGrid::Init");
		this->start_delay = start_delay;
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

	}
};

class AlienGGridBehaviourComponent : public Component {	 
	// needs access to the object pools: aliens, bombs
	ObjectPool<AlienG>* alien_g_pool;
	bool alien_g_level_clear = false;
	float init_delay; // Time when the aliens can start moving and shooting

public:
	virtual ~AlienGGridBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienG>* alien_g_pool)
	{
		Component::Create(engine, go, game_objects);
		this->alien_g_pool = alien_g_pool;
	}

	virtual void Init() {
		SDL_Log("AlienGGridBehaviourComponent::Init");
		Component::Init();
		AlienGGrid* grid = dynamic_cast<AlienGGrid*>(go); //ugly hack from lab5 to make us access start_delay property from AlienVGrid
		init_delay = engine->getElapsedTime() + grid->start_delay;
		SDL_Log("Initial delay %f, AlienV should show up at %f", grid->start_delay, init_delay);
	    alien_g_level_clear = false;
	}

	virtual void Update(float dt)
	{
		// If we haven't passed the delay time, skip update
		if (engine->getElapsedTime() < init_delay)
		{
			return;
		}

		if (alien_g_level_clear) {
			return;
		}
		else {
			bool all_alien_g_disabled = true;
			for (auto it = alien_g_pool->pool.begin(); it != alien_g_pool->pool.end(); it++) {
				if ((*it)->enabled)
				{
					all_alien_g_disabled = false;
					(*it)->Update(dt);
				}
			}

			if (all_alien_g_disabled)
			{
				SDL_Log("AlienGGridBehaviourComponent::Update Send message ALL_ALIENS_G_GONE ");
				go->Send(ALIEN_G_LEVEL_CLEAR);
				alien_g_level_clear = true;
			}
		}
	}
};