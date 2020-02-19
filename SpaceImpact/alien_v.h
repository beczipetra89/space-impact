#pragma once

// AN ALIEN FOR THE V-SHAPE GROUP 

#include <cstdlib>
#include <ctime>

class AlienVBehaviourComponent : public Component
{
	ObjectPool<AlienLaser>* alienLaser_pool;

	float time_laser_shot;  // Time from last time laser was shot by alien V


	float direction = -1.f; // if direction = 1, alien moves up, if direction = -1, alien moves down
	float time_moved;	// time from the last time alien_g moved
	float next_movement;
	bool vertical_movement_pause = false;
	float initialYPos;

public:
	virtual ~AlienVBehaviourComponent() {}

	// replaced "GameObject with AlienV
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienLaser>* alienLaser_pool)
	{
		Component::Create(engine, go, game_objects);
		this->alienLaser_pool = alienLaser_pool;
		
	}

	virtual void Init()
	{

		initialYPos = go->verticalPosition;
		
	}

	virtual void Update(float dt)
	{

		Move(dt);
		if (go->horizontalPosition < -40) // When alian G flew oout of window to the left, it disappears.
			go->enabled = false;

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

	struct Coordinate { float  x; float y; }; // Coordinates to use when drawing V shape

	virtual ~AlienV() { SDL_Log("AlienV::~AlienV"); }

	virtual void Init()
	{
		SDL_Log("AlienV::Init x:%f, y:%f", horizontalPosition, verticalPosition);
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