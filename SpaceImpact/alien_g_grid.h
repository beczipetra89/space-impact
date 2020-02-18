#pragma once

class AlienGGrid : public GameObject {

public:
	virtual ~AlienGGrid() { SDL_Log("AlienGGrid::~AlienGGrid"); }

	virtual void Init()
	{
		SDL_Log("AlienGGrid::Init");
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
	//float start_time; // the timestamp for when alien_g grid is initialized
	bool alien_g_level_clear = false;

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
	    alien_g_level_clear = false;
	}

	virtual void Update(float dt)
	{
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