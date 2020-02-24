#pragma once


class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{

		go->horizontalPosition += ROCKET_SPEED * dt; // rocket_speed * time. When testing we can add "* 5"
		
		if (go->horizontalPosition > 820) // When the rocket reaches the edge of the screen, it disappears.
			go->enabled = false;

	
	}
};


// rockets are shot by the player towards the aliens
class Rocket : public GameObject
{

public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		horizontalPosition = xPos+80;  
		verticalPosition = yPos+38;
	}


	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Rocket::Hit");
		}

		if (m == BULLET_BULLET_HIT) {
			enabled = false;
			SDL_Log("Rocket::BULLET_BULLET_HIT");
		}
	}
};