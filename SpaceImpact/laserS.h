#pragma once

class LaserSBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition -= BOMB_SPEED * dt; 

		if (go->horizontalPosition < 0 - 12) // When the laser reaches the side of the screen, it disappears.
			go->enabled = false;
	}
};


// lasers are shot by the alien towards the player
class LaserS : public GameObject
{
public:

	virtual void Init(double xPos, double yPos)
	{
		
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos+38;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			
		}

		if (m == BULLET_BULLET_HIT) {
			enabled = false;
		
		}
	}
};