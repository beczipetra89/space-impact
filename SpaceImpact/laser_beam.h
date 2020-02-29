#pragma once


class LaserBeamBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{

		go->horizontalPosition += LASERBEAM_SPEED * dt; 

		if (go->horizontalPosition > 820) // When the laser beam reaches the edge of the screen, it disappears.
			go->enabled = false;


	}
};


// A laser beam shot by the player towards the aliens
class LaserBeam : public GameObject
{

public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("LaserBeam::Init");
		GameObject::Init();

		horizontalPosition = xPos + 80;
		verticalPosition = yPos;
	}


	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			
		}

	//	if (m == BULLET_BULLET_HIT) {
		//	
		//}
	}
};
