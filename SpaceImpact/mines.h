#pragma once

class MineBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition -= MINE_SPEED * dt; // bomb_speed * time

		if (go->horizontalPosition < 0 - 32) // When the mine reaches the side of the screen, it disappears.
			go->enabled = false;
	}
};


// bombs are shot by the alien towards the player
class Mine : public GameObject
{
public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Mine::Init");
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
			SDL_Log("Mine::Hit");
		}

		if (m == BULLET_BULLET_HIT) {
			enabled = false;
			SDL_Log("Mine::BULLET_BULLET_HIT");
		}
	}
};