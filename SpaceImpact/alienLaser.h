#pragma once

class AlienLaserBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition -= ALIENLASER_SPEED * dt; // laser_speed * time

		if (go->horizontalPosition < 0 - 32) // When the bomb reaches the screen, it disappears.
			go->enabled = false;
	}
};


// bombs are shot by the alien towards the player
class AlienLaser : public GameObject
{
public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("AlienLaser::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("AlienLaser::Hit");
		}
	}
};