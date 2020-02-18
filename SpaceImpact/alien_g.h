#pragma once
#include <cstdlib>
#include <ctime>

class AlienGBehaviourComponent : public Component
{

	float direction = -1.f; // if direction = 1, alien moves up, if direction = -1, alien moves down
	float time_moved;	// time from the last time alien_g moved
	float next_movement;
	bool vertical_movement_pause = false;
	float initialYPos;

public:
	virtual ~AlienGBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, float delay)
	{
		Component::Create(engine, go, game_objects);
	
		next_movement = engine->getElapsedTime() + delay;
	}

	void SetDelay(float delay)
	{
		next_movement = engine->getElapsedTime() + delay;
	}

	virtual void Init()
	{
		SDL_Log("AlienGBehaviourComponent::Init");
		SDL_Log("AlienGBehaviourComponent::Init::next_move: %f", next_movement);
		//go->horizontalPosition = 600;

		//int min = 64;
		//int max = 412;

		//srand((unsigned)time(0));
		//int r = (int)rand() / (int)RAND_MAX;
		//go->verticalPosition = min + r * (max - min);
		initialYPos = go->verticalPosition;
		SDL_Log("AlienGBehaviourComponent::Init::initialYPos %f elapsedTime: %f, next_move: %f", go->verticalPosition, engine->getElapsedTime(), next_movement);
		time_moved = -1000.0f;
		// Init alien on random height

		//go->verticalPosition = (rand() % 439) + 1;

	//	go->verticalPosition = 64 + (std::rand() % (64 - 416 + 1)); 

	//	go->verticalPosition = RandomHeight(64, 416);
	}

	virtual void Update(float dt)
	{

		Move(dt);

		if (go->horizontalPosition < 0) // When alian G flew out of window to the left, it disappears.
		{
			go->enabled = false;
		}
		/*	xpos = xpos - 1 * horizontialSpeed;
		if (ypos < initialYpos - 20) {
			// reached upper limit, fly downwards instead
			verticalDirection = 1;
		}
		else if (ypos > initialYpos + 20) {
			verticalDirection = -1;
		}
		ypos = ypos + verticalSpeed * verticalDirection;  */

	}

	// move the alien  to left
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float dt)
	{
		//// When alien moved to top or bottom boundry, switch the vertical direction
		if (((go->verticalPosition - initialYPos) > 20 || (initialYPos-go->verticalPosition) > 20))
		{
			//next_movement = engine->getElapsedTime() + 2.f;
			direction = direction * -1.0f;
			//vertical_movement_pause = false;
		}
		//if ((initialYPos - go->verticalPosition) < 0.f)
		//	SDL_Log("iYP-go: %f", (initialYPos - go->verticalPosition));

		go->horizontalPosition -= dt * ALIEN_G_SPEED;

		if (engine->getElapsedTime()>next_movement)
		{
			go->verticalPosition += dt * ALIEN_G_VERTICAL_SPEED * direction;
		//	//SDL_Log("New YPos: %f", go->verticalPosition);
		}
		//else
		//{
		//	vertical_movement_pause = true;
		//	//SDL_Log("Wait until %f, current time %f, vertial_pause %d", next_movement, engine->getElapsedTime(), vertical_movement_pause);
		//}
		//SDL_Log("New Ypos: %f, InitalY: %f", go->verticalPosition, initialYPos);
	}

	int RandomHeight(int min, int max)
	{
		srand((unsigned)time(0));
		int r = (int)rand() / (int)RAND_MAX;
		return min + r * (max - min);
	}

	// return true if enough time has passed from the last vertical movement
	bool canMove()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_moved) < (ALIEN_G_VERTICAL_MOVE_TIME_INTERNAL / game_speed))
			return false;

		if ((engine->getElapsedTime() - time_moved) > 500)
			time_moved = engine->getElapsedTime();
		SDL_Log("Move! New time_moved: %f elaspsed time: %f", time_moved, engine->getElapsedTime());
		return true;
	}
};

class AlienG : public GameObject
{
public:
	virtual ~AlienG() { SDL_Log("AlienG::~AlienG"); }

	virtual void Init()
	{
		SDL_Log("AlienG::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}
		if (m == HIT) {
			// Disabe alien_g when it's hit
			SDL_Log("AlienG::HIT!");
			Send(ALIEN_G_HIT); // Send a message so that game can update the score
			enabled = false;
		}
	}
};