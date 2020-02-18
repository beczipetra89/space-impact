#pragma once
#include "game_object.h"
class BossAlienManager : public GameObject
{
public:
	bool spawn_boss = false; // if set to true, we will spawn alien boss

	virtual void Receive(Message m)
	{
		if (!enabled) {

			return;
		}
		if (m == ALIENS_ALL_CLEAR) {
			// Show alien boss when all other aliens disappear on the screen
			SDL_Log("BossAlienManager::ALIENS_ALL_CLEAR!");
			spawn_boss = true;
		}
	}

};

class BossAlienManagerBehaviourComponent : public Component
{
public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects)
	{
		Component::Create(engine, go, game_objects);
	}
};
