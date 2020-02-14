#pragma once

// GameObject represents objects which moves are drawn
#include <vector>

enum Message { HIT, ALIEN_HIT, ALIEN_G_HIT, ALIEN_V_HIT, ALIEN_CHANGE_DIRECTION_AND_MOVE_DOWN, ALIEN_HIT_WALL,LIFE_PICKED, GAME_OVER, LEVEL_WIN, NO_MSG, QUIT };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	double horizontalPosition;
	double verticalPosition;
	bool enabled;

	virtual ~GameObject();

	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};