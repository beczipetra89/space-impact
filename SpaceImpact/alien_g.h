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

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, float delay = 0.f)
	{
		Component::Create(engine, go, game_objects);
		initialYPos = go->verticalPosition;
		vertical_delay = engine->getElapsedTime() + delay;
		SDL_Log("AlienGBehaviourComponent::Create Y: %f, vertical_delay: %f", go->verticalPosition, vertical_delay);
	}

	void SetDelay(float delay)
	{
		vertical_delay = engine->getElapsedTime() + delay;
		SDL_Log("AlienGBehaviourComponent::SetDelay Y: %f, vertical_delay: %f", go->verticalPosition, vertical_delay);
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

		if (go->horizontalPosition < -80) // When alian G flew out of window to the left, it disappears.
			go->enabled = false;
	}

	// move the alien  to left
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float dt)
	{
		// When alien moved to top or bottom boundry, switch the vertical direction
		if (((go->verticalPosition - initialYPos) > 20 || (initialYPos-go->verticalPosition) > 20))
			direction = direction * -1.0f;

		go->horizontalPosition -= dt * ALIEN_G_SPEED;

		if (engine->getElapsedTime()>vertical_delay)
			go->verticalPosition += dt * ALIEN_G_VERTICAL_SPEED * direction;
	}
};

class AlienG : public GameObject
{
public:
	virtual ~AlienG() { SDL_Log("AlienG::~AlienG"); }

	virtual void Init() {
		SDL_Log("AlienG::Init x:%f y:%f", horizontalPosition, verticalPosition);
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;
		
		if (m == HIT) {
			// Disabe alien_g when it's hit
			SDL_Log("AlienG::HIT!");
			Send(ALIEN_G_HIT); // Send a message so that game can update the score
			enabled = false;
		}
	}
};