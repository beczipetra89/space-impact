#pragma once

class AlienLaserBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition -= ALIENLASER_SPEED * dt; 

		if (go->horizontalPosition < 0 ) // When the laser reaches the screen, it disappears.
			go->enabled = false;
	}
};


// lasers are shot by the alien towards the player
class AlienLaser : public GameObject
{
public:

	virtual void Create() {}

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("AlienLaser::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos+30;
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

		if (m == BULLET_BULLET_HIT) {
			enabled = false;
			SDL_Log("AlienLaser::BULLET_BULLET_HIT");
		}
	}
};