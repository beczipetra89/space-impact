#pragma once

class PlayerBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Rocket> * rockets_pool;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Rocket> * rockets_pool)
	{
		Component::Create(engine, go, game_objects);
		this->rockets_pool = rockets_pool;
	}

	virtual void Init()	
	{
		go->horizontalPosition = 0 + 32;  // 320
		go->verticalPosition = 250 - 32; //480- 32

		time_fire_pressed = -10000.f;
	}

	void InitKeys(AvancezLib::KeyStatus * keys) {
		PlayerBehaviourComponent::keys = keys;
	}

	virtual void Update(float dt)
	{
		//AvancezLib::KeyStatus keys;
		engine->getKeyStatus(*keys);

		if (keys->up) {
			Move(-dt * PLAYER_SPEED);
			go->verticalPosition--;
		}

		if (keys->down) {
			Move(dt * PLAYER_SPEED);
			go->verticalPosition++;
		}

		if (keys->right) {
			Move(dt * PLAYER_SPEED);
			go->horizontalPosition++;
		}

		if (keys->left) {
			Move(-dt * PLAYER_SPEED);
			go->horizontalPosition--;
		}

		if (keys->fire)
		{
			if (CanFire())
			{
				// fetches a rocket from the pool and use it in game_objects
				Rocket* rocket = rockets_pool->FirstAvailable();
				if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
				{
					rocket->Init(go->horizontalPosition, go->verticalPosition);
					game_objects->insert(rocket);
				}
			}
		} 

	}


	// move the player up or down, left and right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		//go->verticalPosition += move;

		//Window boundaries to wrap player inside the window
		if (go->verticalPosition > (480 - 32))
			go->verticalPosition = 480 - 32;

		if (go->verticalPosition < 0)
			go->verticalPosition = 0;
	

		if (go->horizontalPosition > (640 - 32))
			go->horizontalPosition = 640 - 32;

		if (go->horizontalPosition < 0)
			go->horizontalPosition = 0;

	}

	// return true if enough time has passed from the previous rocket
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
			return false;

		time_fire_pressed = engine->getElapsedTime();

		SDL_Log("fire!");
		return true;
	}
private:
	AvancezLib::KeyStatus* keys;
	
};


// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 

	virtual ~Player()	{SDL_Log("Player::~Player");}

	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		lives = NUM_LIVES;
	}

	virtual void Receive(Message m) 
	{
		if (m == HIT)
		{ 
			SDL_Log("Player::Hit!");
			RemoveLife();
		
		
			if (lives < 0)
				Send(GAME_OVER);
		}

		if (m == LIFE_PICKED) {
			AddLife();
		}
	}

	void AddLife() {
	if(lives<2)
		lives++;
	SDL_Log("Total lives: %d", lives);
		
	}


	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
		
	}
};
