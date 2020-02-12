#pragma once

class AlienVGrid : public GameObject {

public:
	virtual ~AlienVGrid() { SDL_Log("AlienVGrid::~AlienVGrid"); }

	virtual void Init()
	{
		SDL_Log("AlienVGrid::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

	}
};

class AlienVGridBehaviourComponent : public Component {
	// needs access to the object pools: aliens, bombs
	ObjectPool<AlienV>* alien_v_pool;

public:
	virtual ~AlienVGridBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienV>* alien_v_pool)
	{
		Component::Create(engine, go, game_objects);
		this->alien_v_pool = alien_v_pool;
	}

	virtual void Init() {
		SDL_Log("AlienVGridBehaviourComponent::Init");
		Component::Init();
	}

	virtual void Update(float dt)
	{
		for (auto it = alien_v_pool->pool.begin(); it != alien_v_pool->pool.end(); it++) {
			(*it)->Update(dt);
		}
	}
};