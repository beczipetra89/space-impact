#pragma once

// GameObject represents objects which moves are drawn
#include <vector>

enum Message {
	HIT,
	ALIEN_HIT,
	ALIEN2_HIT,
	ALIEN_G_HIT, ALIEN_V_HIT,
	ALIEN_CHANGE_DIRECTION_AND_MOVE_DOWN, ALIEN_HIT_WALL,
	ALIEN_G_LEVEL_CLEAR,
	ALL_ALIENS_V_CLEAR,
	ALIENS_ALL_CLEAR,
	PLAYER_HIT,
	BOSS_HIT, BOSS_KILLED, LIFE_PICKED, BULLET_BULLET_HIT, GAME_OVER, LEVEL_WIN, NO_MSG, QUIT };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	double horizontalPosition;
	double verticalPosition;
	int width, height;
	bool enabled;

	virtual ~GameObject();

	virtual void Create(int w, int h);

	virtual void AddComponent(Component * component);

	template<typename T>
	T GetComponent() {
		for (Component* c : components) {
			T t = dynamic_cast<T>(c);  //ugly but it works...
			if (t != nullptr) {
				return t;
			}
		}

		return nullptr;
	}

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};