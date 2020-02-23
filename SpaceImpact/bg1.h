#pragma once


class Bg1_BehaviourComponent : public Component
{
	
public:
	virtual ~Bg1_BehaviourComponent() {}

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
		if (go->horizontalPosition < -640) 
		{
			go->horizontalPosition = 640;
		}

	}

	// move the alien to left
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		go->horizontalPosition -= move;
	}
};


class Bg1 : public GameObject
{
public:

	virtual ~Bg1() { SDL_Log("Bg1::~Bg1"); }

	virtual void Init()
	{
		SDL_Log("Bg1::Bg1");
		
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled) {
			return;
		}

	}
};