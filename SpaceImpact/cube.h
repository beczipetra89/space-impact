#pragma once

class CubeBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition -= MINE_SPEED * dt; 

		if (go->horizontalPosition < 0 - 32) // When the cube reaches the side of the screen, it disappears.
			go->enabled = false;
	}
};


// cubes are shot by the alien towards the player
class Cube : public GameObject
{
public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Cube::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos - 40;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Mine::Hit");
		}

		if (m == BULLET_BULLET_HIT) {
			enabled = false;
			SDL_Log("Cube::BULLET_BULLET_HIT");
		}
	}
};
