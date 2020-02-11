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
	}

	virtual void Update(float dt)
	{
		for (auto it = alien_g_pool->pool.begin(); it != alien_g_pool->pool.end(); it++) {
			(*it)->Update(dt);
		}
	}
};