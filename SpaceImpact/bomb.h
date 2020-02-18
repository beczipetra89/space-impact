#pragma once

class BombBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition -= BOMB_SPEED * dt; // bomb_speed * time

		if (go->horizontalPosition < 0 - 32) // When the bomb reaches the side of the screen, it disappears.
			go->enabled = false;
	}
};


// bombs are shot by the alien towards the player
class Bomb : public GameObject
{
public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Bomb::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos+18;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Bomb::Hit");
		}

		if (m == BULLET_BULLET_HIT) {
			enabled = false;
			SDL_Log("Bomb::BULLET_BULLET_HIT");
		}
	}
};