#pragma once

class AlienVGrid : public GameObject {

public:
	float start_delay; // a time delay, after which the game object in pool can perform update()

	virtual ~AlienVGrid() { SDL_Log("AlienVGrid::~AlienVGrid"); }

	virtual void Init(float start_delay)
	{
		SDL_Log("AlienVGrid::Init initial delay %f", start_delay);
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

class AlienVGridBehaviourComponent : public Component {
	// needs access to the object pools: aliens, bombs
	ObjectPool<AlienV>* alien_v_pool;
	ObjectPool<AlienLaser>* alienLaser_pool;

	float time_laser_shot;  // Time from last time laser was shot by alien V
	float init_delay; // Time when the aliens can start moving and shooting
	bool alien_v_clear = false;
public:
	virtual ~AlienVGridBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, AlienVGrid* go, std::set<GameObject*>* game_objects, ObjectPool<AlienV>* alien_v_pool, ObjectPool<AlienLaser>* alienLaser_pool)
	{
		Component::Create(engine, go, game_objects);
		this->alien_v_pool = alien_v_pool;
		this->alienLaser_pool = alienLaser_pool;   
	}

	virtual void Init() {
		SDL_Log("AlienVGridBehaviourComponent::Init");
		Component::Init();
		AlienVGrid* grid = dynamic_cast<AlienVGrid*>(go); //ugly hack from lab5 to make us access start_delay property from AlienVGrid
	    init_delay = engine->getElapsedTime() + grid->start_delay;
		SDL_Log("Initial delay %f, AlienV should show up at %f", grid->start_delay, init_delay);
		time_laser_shot = -10000.f;
		alien_v_clear = false;
	}

	virtual void Update(float dt)
	{
		// If we haven't passed the delay time, skip update
		if (engine->getElapsedTime() < init_delay)
		{
			return;
		}

		// if all alien_v clear skip update so that we don't spam ALL_ALIENS_V_CLEAR message
		if (alien_v_clear) return;

		bool all_alien_v_disabled = true;
		for (auto it = alien_v_pool->pool.begin(); it != alien_v_pool->pool.end(); it++) {
			if ((*it)->enabled)
			{
				all_alien_v_disabled = false;
				(*it)->Update(dt);
			}
		}

		if (all_alien_v_disabled) {
			SDL_Log("AlienVGridBehaviourComponent::Update Send message ALL_ALIENS_V_CLEAR ");
			go->Send(ALL_ALIENS_V_CLEAR);
			alien_v_clear = true;
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
					engine->PlaySFX("data/audio/laser_sound.wav", 0, -1);
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