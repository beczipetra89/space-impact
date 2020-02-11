#pragma once
#include "sstream"

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;
	AvancezLib::KeyStatus keys = { false, false, false, false, false};
	bool cheat_mode = false;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	ObjectPool<Bomb> bombs_pool;
	
	
	Player * player;
	Alien* alien;

	
	
	
	
	//AlienGrid* alien_grid;

	Sprite * life_sprite;
	bool game_over = false;

	unsigned int score = 0;

public:
	
	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");
		this->engine = engine;

		//*************CREATE ROCKETS POOL**************
		rockets_pool.Create(30);
		for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
		{
			RocketBehaviourComponent* behaviour = new RocketBehaviourComponent();
			behaviour->Create(engine, *rocket, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *rocket, &game_objects, "data/bullet1.png");
			(*rocket)->Create();
			(*rocket)->AddComponent(behaviour);
			(*rocket)->AddComponent(render);
		}

		//**************CREATE BOMBS POOL***************
		bombs_pool.Create(30);
		for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent* behaviour = new BombBehaviourComponent();
			behaviour->Create(engine, *bomb, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *bomb, &game_objects, "data/bullet2.png");
			(*bomb)->Create();
			(*bomb)->AddComponent(behaviour);
			(*bomb)->AddComponent(render);
		}


		//***************** PLAYER *******************
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
		player_behaviour->InitKeys(&keys);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(engine, player, &game_objects, "data/player.png");
		CollideComponent * player_bomb_collide = new CollideComponent();
		player_bomb_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & bombs_pool);
		CollideComponent* player_alien_collide = new CollideComponent();
		player_alien_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & alien);

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_bomb_collide);
		player->AddComponent(player_alien_collide);
		player->AddReceiver(this);
		game_objects.insert(player);


		// *******************ENEMYS SINGLE ************************
		

		alien = new Alien();
		AlienBehaviourComponent* alien_behaviour = new AlienBehaviourComponent();
		alien_behaviour->Create(engine, alien, &game_objects, &bombs_pool);
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, alien, &game_objects, "data/enemySingle.png");
		CollideComponent* alien_bullet_collide = new CollideComponent();
		alien_bullet_collide->Create(engine, alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);
		
		alien->Create();
		alien->AddComponent(alien_behaviour);
		alien->AddComponent(alien_render);
		player->AddComponent(alien_bullet_collide);
		alien->AddReceiver(this);
		game_objects.insert(alien);
	
}

	virtual void Init()
	{
		player->Init();
		alien->Init();
		

		// Set background to white
		AvancezLib::RGBColor LILA = { 99, 0, 191 };
		engine->SetBackgroundColor(LILA);

		enabled = true;
	}

	virtual void Update(float dt)
	{
		//AvancezLib::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}

		if (keys.cheat) {
			SDL_Log("Cheat mode enabled");
			cheat_mode = true;
			ROCKET_SPEED = ROCKET_SPEED * 1.15f;
			FIRE_TIME_INTERVAL = FIRE_TIME_INTERVAL / 10;
		}

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		{
			// If game_over is true, set dt to 0 so that all objects will not move.
			if (game_over)
				(*go)->Update(0);
			else
				(*go)->Update(dt);
		}
	}

	virtual void Draw()
	{
		//... Draw user interface elements here

		

		//Draw current lives indicator
		for (int i = 0; i <= player->lives; i++) {
			Sprite* life_sprite = engine->createSprite("data/player.bmp");  
			life_sprite->draw(34*i, 16); // 34*i, 16
		}

		//Score indicator
		char score_string[10];
		sprintf(score_string, "%07d", score); // pad score with zeros on the left
		engine->drawText(250, 16, score_string);
		
		//Bonus indicator
		// Make a string using sprintf with some format
		char bonus_string [20];
		sprintf(bonus_string, "Bonus: %.1f X", game_speed);
		engine->drawText(500, 16, bonus_string);

		//40-el tobb
		if (game_over) {
			engine->drawText(250, 0, "***GAME OVER***");
		}

		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
		{
			SDL_Log("GAME::GAME_OVER!");
			game_over = true;
		}

		if (m == LEVEL_WIN)
		{
			SDL_Log("GAME::LEVEL_WIN");
/*
			// New level, prepare aliens
			game_speed *= 1.2f;
			int alien_x = 0, alien_y = 32, alien_count = 1;
			for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
			{
				(*alien)->Init(alien_x, alien_y);
			//	//********* "DRAWING" THE GRID***********
			// Change the drawing position X to 0 and Y to Y + 4 at every 11 aliens
				if (alien_count > 1 && alien_count % 11 == 0) {
					alien_x = 0;
					alien_y += 32 + 2; // leave 4 pixel between alien lines
				}
				else alien_x += 32 + 4; // leave 32+4 pixel between alien on the same row
				alien_count++;
			}
			alien_grid->Init();

			*/
		}
		
		if (m == ALIEN_HIT) 
		{
			SDL_Log("GAME::ALIEN_HIT!");
			score += POINTS_PER_ALIEN * game_speed;
		}
	}

	virtual void Destroy()
	{
		// Skip cleaning up of game class if this method has already run once (game class is disabled)
		if (!enabled)
			return;

		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		// Empty the game_objects set
		game_objects.clear();

		life_sprite->destroy();
	
		rockets_pool.Destroy();
		bombs_pool.Destroy();
	//	aliens_pool.Destroy();

		delete player;
		delete alien;
	//	delete alien_grid;
		

		// Mark game class as disabled
		enabled = false;
	}
};