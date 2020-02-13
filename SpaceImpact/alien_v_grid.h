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
	ObjectPool<AlienLaser>* alienLaser_pool;

	float time_laser_shot;  // Time from last time laser was shot by alien V

public:
	virtual ~AlienVGridBehaviourComponent() {}


	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienV>* alien_v_pool, ObjectPool<AlienLaser>* alienLaser_pool)
	{
		Component::Create(engine, go, game_objects);
		this->alien_v_pool = alien_v_pool;
		this->alienLaser_pool = alienLaser_pool;   
	}

	virtual void Init() {
		SDL_Log("AlienVGridBehaviourComponent::Init");
		Component::Init();

		time_laser_shot = -10000.f;
	}

	virtual void Update(float dt)
	{
		for (auto it = alien_v_pool->pool.begin(); it != alien_v_pool->pool.end(); it++) {
			(*it)->Update(dt);
		}
			


		if (CanFire())
		{
			// fetches a rocket from the pool and use it in game_objects
			AlienLaser* laser = alienLaser_pool->FirstAvailable();
			if (laser != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				AlienV* alien_v = alien_v_pool->SelectRandom();
				if (alien_v != NULL)
				{
					laser->Init(alien_v->horizontalPosition, alien_v->verticalPosition);
					game_objects->insert(laser);
				}
			}
		}
	}


	bool CanFire()
	{
		// shoot just if enough time passed by

		
		if ((engine->getElapsedTime() - time_laser_shot) < (ALIENLASER_TIME_INTERVAL / game_speed))
			return false;

		time_laser_shot = engine->getElapsedTime();

		SDL_Log("laser!");
		return true;
	}
};