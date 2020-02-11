#pragma once
class AlienBehaviourComponent : public Component
{
	int direction = 1; // if direction = 1, alien moves right, if direction = -1, alien moves left
	ObjectPool<Bomb>* bombs_pool;

public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		go->horizontalPosition = 0;
		go->verticalPosition = 0;
	}

	virtual void Update(float dt)
	{
		Move(dt * ALIEN_SPEED * direction);
	}

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		go->horizontalPosition += move;

		if (go->horizontalPosition > (640 - 32)) {
			go->horizontalPosition = 640 - 32;
			go->verticalPosition = go->verticalPosition + 32;
			direction = -1; // Alien reaches right wall, now move left
		}

		if (go->horizontalPosition < 0) {
			go->horizontalPosition = 0;
			go->verticalPosition = go->verticalPosition + 32;
			direction = 1; // Alien reaches left wall, now move right
		}

		// If alien reaches the bottom of the screen, stop moving down
		if (go->verticalPosition > 480 - 32)
			go->verticalPosition = 480 - 32;
	}
};


class Alien : public GameObject
{
public:

	int lives;

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init()
	{
		SDL_Log("Alien::Init");
		GameObject::Init();
		
	}

	virtual void Receive(Message m)
	{
		
			if (!enabled) {
				return;
			}
			if (m == HIT){
				SDL_Log("Alilen::HIT!");
				enabled = false;
		}
	}
};