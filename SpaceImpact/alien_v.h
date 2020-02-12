#pragma once

// AN ALIEN FOR THE V-SHAPE GROUP 

#include <cstdlib>
#include <ctime>

class AlienVBehaviourComponent : public Component
{

	float direction = -1.f; // if direction = 1, alien moves up, if direction = -1, alien moves down
	float time_moved;	// time from the last time alien_g moved
	float next_movement;
	bool vertical_movement_pause = false;
	float initialYPos;

public:
	virtual ~AlienVBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects) //deleted float delay
	{
		Component::Create(engine, go, game_objects);
		//next_movement = engine->getElapsedTime() + delay;
	}

	virtual void Init()
	{

		initialYPos = go->verticalPosition;
		
	}

	virtual void Update(float dt)
	{

		Move(dt);

	}

	// move the alien  to left
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float dt)
	{
		
		go->horizontalPosition -= dt * ALIEN_V_SPEED;

	}

	
};

class AlienV : public GameObject
{
public:
	virtual ~AlienV() { SDL_Log("AlienV::~AlienV"); }

	virtual void Init()
	{
		SDL_Log("AlienV::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}
		if (m == HIT) {
			// Disabe alien_g when it's hit
			SDL_Log("AlienV::HIT!");
			Send(ALIEN_V_HIT); // Send a message so that game can update the score
			enabled = false;
		}
	}
};