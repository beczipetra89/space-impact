#pragma once

// PICKUP: +UP Life

class PickupLifeBehaviourComponent : public Component
{

public:
	virtual ~PickupLifeBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects)
	{
		Component::Create(engine, go, game_objects);
		
	}

	virtual void Init()
	{
		go->horizontalPosition = 600;

	
	}

	virtual void Update(float dt)
	{

		Move(dt * PICKUP_SPEED);
		if (go->horizontalPosition < -40) // When floats out of the window to the left, it disappears.
			go->enabled = false;


	}

	// move the alien  to left
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		go->horizontalPosition -= move;

	}

};


class PickupLife : public GameObject
{
public:

	virtual ~PickupLife() { SDL_Log("PickupLife::~PickupLife"); }

	virtual void Init()
	{
		SDL_Log("PickupLife::Init");
		GameObject::Init();

	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}
		if (m == LIFE_PICKED ) { // life can be picked when player has full life, but will not add life
			
			SDL_Log("LifePickup::HIT!");
			Send(LIFE_PICKED); // Send a message so that game can update the score
			enabled = false;
		}
		

	}
};