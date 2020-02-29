#pragma once


class Bg3_BehaviourComponent : public Component
{

public:
	virtual ~Bg3_BehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects)
	{
		Component::Create(engine, go, game_objects);
	}

	virtual void Init()
	{
		go->horizontalPosition = 0;
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


class Bg3 : public GameObject
{
public:

	virtual ~Bg3() { SDL_Log("Bg3::~Bg3"); }

	virtual void Create() {}

	virtual void Init()
	{
		SDL_Log("Bg3::Bg3");

		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

	}
};
