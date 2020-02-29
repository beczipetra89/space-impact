#pragma once
//class LifeIconGridBehaviourComponent : public Component {
//	// needs access to the object pools: lives
//	ObjectPool<LifeIcon>* lives_pool;
//	int* lives;
//
//public:
//	virtual ~LifeIconGridBehaviourComponent() {}
//	
//	virtual void Create(AvancezLib* engine, LifeIconGrid* go, std::set<GameObject*>* game_objects, ObjectPool<LifeIcon>* lives_pool)
//	{
//		Component::Create(engine, go, game_objects);
//		this->lives_pool = lives_pool;
//	}
//
//	virtual void Init() {
//		SDL_Log("LifeIconGridBehaviourComponent::Init");
//		Component::Init();
//	}
//
//	virtual void Update(float dt) {
//		if ((*lives) < 0) { return; }
//
//		for (auto i = NUM_LIVES; i > (*lives); i--) {
//			lives_pool->pool.at(i)->enabled = false;
//		}
//	}
//};
//
//class LifeIconGrid : public GameObject
//{
//public:
//
//	virtual ~LifeIconGrid() {  }
//
//	virtual void Create() {}
//
//	virtual void Update(float dt)
//	{}
//};
