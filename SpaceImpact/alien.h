#pragma once

class Alien : public GameObject
{
public:
	int direction = 1;  // if direction = 1, all aliens move right, if direction = -1, aliens move left
	
	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init(int xPos, int yPos)
	{
		SDL_Log("Alien::Init with X: %d, Y: %d", xPos, yPos);
		horizontalPosition = xPos;
		verticalPosition = yPos;
		direction = 1;

		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

		if (m == HIT)
		{
			// Disabe alien when it's hit
			SDL_Log("Alien::HIT!");
			Send(ALIEN_HIT); // Send a message so that game can update the score
			enabled = false;
		}

		if (m == ALIEN_CHANGE_DIRECTION_AND_MOVE_DOWN)
		{
			// Move alien Y position downwards and change X moving direction
			verticalPosition = verticalPosition + 32; // Move downwards from the current vertical position
			direction = direction * - 1;
		}

	}
};

class AlienBehaviourComponent : public Component
{
	float time_bomb_dropped;	// time from the last time the bomb was dropped
	ObjectPool<Bomb>* bombs_pool;
	bool stop_updating = false;   // if set to true, alien will not do anything

public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, Alien* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Update(float dt)
	{
		if (stop_updating)
			return;

		// Use "(Alien *) go" to cast variable "go" from GameObject type to Alien type
		int direction = ((Alien*)go)->direction;
		Move(dt * ALIEN_SPEED * direction);
	}

	// move the alien left or right
	// param move depends on the time, so the alien moves always at the same speed on any computer
	void Move(float move)
	{
		go->horizontalPosition += move;
		
		if (go->horizontalPosition > (640 - 32)) {
			go->horizontalPosition = 640 - 32;
			// Send a message to alien_grid that we need to change direction
			go->Send(ALIEN_HIT_WALL);
		}

		if (go->horizontalPosition < 0) {
			go->horizontalPosition = 0;
			// Send a message to alien_grid that we need to change direction
			go->Send(ALIEN_HIT_WALL);
		}

		// If alien reaches the bottom of the screen, send game over messages to all components
		if (go->verticalPosition > 480 - 32) {
			// set stop_updating to true so that we will not send GAME_OVER message over and over again
			stop_updating = true;
			go->Send(GAME_OVER);
		}

	}
};