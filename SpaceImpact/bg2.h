#pragma once


class Bg2_BehaviourComponent : public Component
{

public:
	virtual ~Bg2_BehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects)
	{
		Component::Create(engine, go, game_objects);
	}

	virtual void Init()
	{
		go->horizontalPosition = 840;
		go->verticalPosition = 0;
	}

	virtual void Update(float dt)
	{

		Move(dt * BACKGROUND_SPEED);
		if (go->horizontalPosition < -840)
		{
			go->horizontalPosition = 840;
		}

	}

		void Move(float move)
	{
		go->horizontalPosition -= move;
	}
};


class Bg2 : public GameObject
{
public:

	virtual ~Bg2() {  }

	virtual void Create() {}

	virtual void Init()
	{
		
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

	}
};
