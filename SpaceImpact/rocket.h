#pragma once


class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{

		go->horizontalPosition += ROCKET_SPEED * dt; // rocket_speed * time. When testing we can add "* 5"
		
		if (go->horizontalPosition > 600) // When the rocket reaches the edge of the screen, it disappears.
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

		horizontalPosition = xPos+40;  //considering player sprite and bullet sprite size
		verticalPosition = yPos+20;

		//horizontalPosition = 0+32;
		//verticalPosition = 250 - 52; 
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
	}
};