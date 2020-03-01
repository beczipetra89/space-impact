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

void SingleObjectCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject* coll_object, SDL_Rect coll_rect)
{
	Component::Create(engine, go, game_objects);
	this->coll_object = coll_object;
	this->coll_rect = coll_rect;
}

void SingleObjectCollideComponent::Update(float dt)
{
	GameObject* go0 = this->coll_object;

	if (go != NULL && go0 != NULL) {
		if (go->enabled && go0->enabled)
		{

			if (go0->horizontalPosition + go0->width <= go->horizontalPosition)
				return;
			if (go0->horizontalPosition >= go->horizontalPosition + coll_rect.w)
				return;
			if (go0->verticalPosition + go0->height <= go->verticalPosition)
				return;
			if (go0->verticalPosition >= go->verticalPosition + coll_rect.h)
				return;

			go->Receive(HIT);
			go0->Receive(HIT);
		}
	}
}

void LifePickupCollisionComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, GameObject* coll_object, SDL_Rect coll_rect)
{
	Component::Create(engine, go, game_objects);
	this->coll_object = coll_object;
	this->coll_rect = coll_rect;
}

void LifePickupCollisionComponent::Update(float dt)
{
	GameObject* go0 = this->coll_object;

	if (go != NULL && go0 != NULL) {
		if (go->enabled && go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition) &&
				(go0->horizontalPosition < go->horizontalPosition + coll_rect.w) &&
				(go0->verticalPosition > go->verticalPosition) &&
				(go0->verticalPosition < go->verticalPosition + coll_rect.h))
			{
				SDL_Log("Life picked up");
				go->enabled = false;
				go->Send(LIFE_PICKED);
			}
		}
	}
}

void LaserBeamCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}

void LaserBeamCollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject* go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if (go0->horizontalPosition > go->horizontalPosition)
				// go0 is the laserbeam, it will not be disabled by collision, so only disable the other game_object
				go->Receive(HIT);
		}
	}
}

/* w and h is the total width and height of the image 
animation_speed, the higher the faster, default to 1 */
void AnimRenderComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, const char* sprite_name, int w, int h, int total_frame, int animation_speed = 1)
{
	Component::Create(engine, go, game_objects);
	sprite = engine->createSprite(sprite_name, w, h);
	frame_w = w / total_frame;
	frame_h = h;
	this->total_frame = total_frame;
	frame_time = 1.0 / (total_frame * animation_speed);
	init_time = engine->getElapsedTime();
}

void AnimRenderComponent::Update(float dt)
{
	SDL_Rect src_rect;
	SDL_Rect dst_rect;

	if (!go->enabled)
		return;

	if (sprite != NULL) {
		float t =  engine->getElapsedTime() - init_time;
		int frame_index = int(t / frame_time);
		frame_index = frame_index % total_frame; // use modulous to make sure the index number is always smaller than boundary
		// see http://gigi.nullneuron.net/gigilabs/animations-with-sprite-sheets-in-sdl2/
		src_rect = { frame_w * frame_index, 0, frame_w, frame_h };
		dst_rect = { int(go->horizontalPosition), int(go->verticalPosition), frame_w, frame_h };
		sprite->partialDraw(&src_rect, &dst_rect);
	}
}

void AnimRenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}
