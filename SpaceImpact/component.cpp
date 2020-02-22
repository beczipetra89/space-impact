#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "sprite.h"

void Component::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name, int w, int h)
{
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name, w, h);
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;

	if (sprite)
		sprite->draw(int(go->horizontalPosition), int(go->verticalPosition));
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}


void CollideComponent::Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects, SDL_Rect coll_rect)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
	this->coll_rect = coll_rect;
}


void CollideComponent::Update(float dt)  
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition) &&  
				(go0->horizontalPosition < go->horizontalPosition + coll_rect.w) &&
				(go0->verticalPosition   > go->verticalPosition) &&
				(go0->verticalPosition   < go->verticalPosition + coll_rect.h))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}

void BulletCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}


void BulletCollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject* go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 5) &&
				(go0->horizontalPosition < go->horizontalPosition + 5) &&
				(go0->verticalPosition > go->verticalPosition - 7) &&
				(go0->verticalPosition < go->verticalPosition + 7))
			{
				go->Receive(BULLET_BULLET_HIT);
				go0->Receive(BULLET_BULLET_HIT);
			}
		}
	}
}


void SingleObjectCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject* coll_object)
{
	Component::Create(engine, go, game_objects);
	this->coll_object = coll_object;
}

void SingleObjectCollideComponent::Update(float dt)
{
	GameObject* go0 = this->coll_object;

	if (go != NULL && go0 !=NULL ) {
		if (go->enabled && go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 35) &&
				(go0->horizontalPosition < go->horizontalPosition + 35) &&
				(go0->verticalPosition > go->verticalPosition - 35) &&
				(go0->verticalPosition < go->verticalPosition + 35))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}

void LifePickupCollisionComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject* coll_object)
{
	Component::Create(engine, go, game_objects);
	this->coll_object = coll_object;
}

void LifePickupCollisionComponent::Update(float dt)
{
	GameObject* go0 = this->coll_object;

	if (go != NULL && go0 != NULL) {
		if (go->enabled && go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 10) &&
				(go0->horizontalPosition < go->horizontalPosition + 10) &&
				(go0->verticalPosition > go->verticalPosition - 10) &&
				(go0->verticalPosition < go->verticalPosition + 10))
			{
				go->Receive(LIFE_PICKED);
				go0->Receive(LIFE_PICKED);
			}
		}
	}
}


void BossObjectCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject* coll_object)
{
	Component::Create(engine, go, game_objects);
	this->coll_object = coll_object;
}

void BossObjectCollideComponent::Update(float dt)
{
	// For rendering collider
	//SDL_Rect rect = {go->horizontalPosition-110, go->verticalPosition - 135, 220, 270};
	//SDL_SetRenderDrawColor(engine->renderer, 255, 0, 0, 128);
	//SDL_RenderFillRect(engine->renderer, &rect);

	GameObject* go0 = this->coll_object;

	if (go != NULL && go0 != NULL) {
		if (go->enabled && go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 50) &&
				(go0->horizontalPosition < go->horizontalPosition + 50) &&
				(go0->verticalPosition > go->verticalPosition - 50) &&
				(go0->verticalPosition < go->verticalPosition + 50))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}