#pragma once
#include <cstdlib>
#include <ctime>

class AlienGBehaviourComponent : public Component
{

	float direction = -1.f; // if direction = 1, alien moves up, if direction = -1, alien moves down
	float time_moved;	// time from the last time alien_g moved
	float vertical_delay; // time when alien can start moving vertically
	float initialYPos;

public:
	virtual ~AlienGBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, float delay)
	{
		Component::Create(engine, go, game_objects);
		SDL_Log("AlienGBehaviourComponent::Create Y: %f", go->verticalPosition);
		initialYPos = go->verticalPosition;
		vertical_delay = engine->getElapsedTime() + delay;
	}

	void SetDelay(float delay)
	{
		vertical_delay = engine->getElapsedTime() + delay;
	}

	virtual void Init()
	{
		SDL_Log("AlienGBehaviourComponent::Init::next_move: %f", vertical_delay);
		initialYPos = go->verticalPosition;
		SDL_Log("AlienGBehaviourComponent::Init::initialYPos %f elapsedTime: %f, next_move: %f", go->verticalPosition, engine->getElapsedTime(), vertical_delay);
		time_moved = -1000.0f;
	}

	virtual void Update(float dt)
	{

		Move(dt);

		if (go->horizontalPosition < -40) // When alian G flew out of window to the left, it disappears.
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

		if (engine->getElapsedTime()>vertical_delay)
		{
			go->verticalPosition += dt * ALIEN_G_VERTICAL_SPEED * direction;
		//	//SDL_Log("New YPos: %f", go->verticalPosition);
		}
	}
};

class AlienG : public GameObject
{
public:
	virtual ~AlienG() { SDL_Log("AlienG::~AlienG"); }

	virtual void Init()
	{
		SDL_Log("AlienG::Init x:%f y:%f", horizontalPosition, verticalPosition);
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