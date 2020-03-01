#pragma once

#include <set>
#include "object_pool.h"

class GameObject;
class AvancezLib;
class Sprite;

class Component
{
protected:
	AvancezLib * engine;	// used to access the engine
	GameObject * go;		// the game object this component is part of
	std::set<GameObject*> * game_objects;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects);

	virtual void Init() {}
	virtual void Update(float dt) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
	Sprite* sprite;

public:

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name, int w, int h);
	virtual void Update(float dt);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
};


class AnimRenderComponent : public Component
{
	Sprite* sprite;
	int total_frame, frame_w, frame_h;
	float frame_time, init_time;
public:

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, const char* sprite_name, int w, int h, int total_frame, int animation_speed);
	virtual void Update(float dt);
	virtual void Destroy();
};


class CollideComponent : public Component
{
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects
	SDL_Rect coll_rect;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects, SDL_Rect coll_rect);
	virtual void Update(float dt);
};


// For testing collision between single objects
class SingleObjectCollideComponent : public Component
{
	GameObject * coll_object; // collision will be tested with a single object
	SDL_Rect coll_rect;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject * coll_object, SDL_Rect coll_rect);
	virtual void Update(float dt);
};

// For laser beam collision check
class LaserBeamCollideComponent : public Component
{
	ObjectPool<GameObject>* coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects);
	virtual void Update(float dt);
};

//  Collision to PICKUPS
class LifePickupCollisionComponent : public Component
{
	GameObject* coll_object; // collision will be tested with pickups
	SDL_Rect coll_rect;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject* coll_object, SDL_Rect coll_rect);
	virtual void Update(float dt);
};
