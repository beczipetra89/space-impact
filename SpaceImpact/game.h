#pragma once
#include "sstream"
#include "levels.h"

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;
	AvancezLib::KeyStatus keys = { false, false, false, false, false};
	bool cheat_mode = false;
	float init_time; // the time when Game object init() is called
	float relative_time;
	bool init_new_alien_g = false; // whether to init new alien_g in alien_g_grid
	bool init_new_alien_v = false; 
	bool boss_spawned = false;
	int seq_count = 0; // Current game level in levels
	std::vector<Sequence> current_level_sequence;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	ObjectPool<Bomb> bombs_pool;
	ObjectPool<AlienG> alien_g_pool;
	ObjectPool<AlienV> alien_v_pool;
	ObjectPool<AlienLaser> alienLaser_pool;
	ObjectPool<Mine> mines_pool;

	Player * player;
	Alien* alien;
	Alien2* alien2;

	AlienG* alien_g;
	AlienGGrid* alien_g_grid;
	AlienV* alien_v;
	AlienVGrid* alien_v_grid;
	PickupLife* life_pickup;

	BossAlien* boss_alien;

	Sprite * life_sprite;

	bool level_finished = false;
	bool level_win = false;
	bool game_over = false;
	bool game_won = false;
	int current_level = -1;
	unsigned int score = 0;

	// for drawing text
	char life_string[10];
	char score_string[10];
	char debug_string[30];

public:
	
	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");
		this->engine = engine;

		//*************CREATE ROCKETS POOL**************
		rockets_pool.Create(100);
		for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
		{
			RocketBehaviourComponent* behaviour = new RocketBehaviourComponent();
			behaviour->Create(engine, *rocket, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *rocket, &game_objects, "data/bullet.png", 7, 5);

			BulletCollideComponent* rocket_laser_S_collision = new BulletCollideComponent();
			rocket_laser_S_collision->Create(engine, *rocket, &game_objects, (ObjectPool<GameObject>*) & bombs_pool);

			BulletCollideComponent* rocket_laser_V_collision = new BulletCollideComponent();
			rocket_laser_V_collision->Create(engine, *rocket, &game_objects, (ObjectPool<GameObject>*) & alienLaser_pool);

			BulletCollideComponent* rocket_mine_collision = new BulletCollideComponent();
			rocket_mine_collision->Create(engine, *rocket, &game_objects, (ObjectPool<GameObject>*) & mines_pool);


			(*rocket)->Create();
			(*rocket)->AddComponent(behaviour);
			(*rocket)->AddComponent(render);

			(*rocket)->AddComponent(rocket_laser_S_collision);
			(*rocket)->AddComponent(rocket_laser_V_collision);
			(*rocket)->AddComponent(rocket_mine_collision);
			(*rocket)->AddReceiver(this);
		}

		//**************CREATE LASER S POOL***************
		bombs_pool.Create(2);
		for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent* behaviour = new BombBehaviourComponent();
			behaviour->Create(engine, *bomb, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *bomb, &game_objects, "data/laser.png", 9, 7);
			
			BulletCollideComponent* laserS_rocket_collision = new BulletCollideComponent();
			laserS_rocket_collision->Create(engine, *bomb, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

			(*bomb)->Create();
			(*bomb)->AddComponent(behaviour);
			(*bomb)->AddComponent(render);

			(*bomb)->AddComponent(laserS_rocket_collision);
			(*bomb)->AddReceiver(this);

		}

		//**************CREATE ALIEN V LASER POOL***************
		alienLaser_pool.Create(7);
		for (auto laser = alienLaser_pool.pool.begin(); laser != alienLaser_pool.pool.end(); laser++)
		{
			AlienLaserBehaviourComponent* behaviour = new AlienLaserBehaviourComponent();
			behaviour->Create(engine, *laser, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *laser, &game_objects, "data/laser.png", 9, 7);

			BulletCollideComponent* laserV_rocket_collision = new BulletCollideComponent();
			laserV_rocket_collision->Create(engine, *laser, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

			(*laser)->AddComponent(laserV_rocket_collision);
			(*laser)->AddReceiver(this);

			(*laser)->Create();
			(*laser)->AddComponent(behaviour);
			(*laser)->AddComponent(render);
		}

		//**************CREATE MINES POOL***************
		mines_pool.Create(30);
		for (auto mine = mines_pool.pool.begin(); mine != mines_pool.pool.end(); mine++)
		{
			MineBehaviourComponent* behaviour = new MineBehaviourComponent();
			behaviour->Create(engine, *mine, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *mine, &game_objects, "data/cross.png", 18, 17);

			BulletCollideComponent* mine_rocket_collision = new BulletCollideComponent();
			mine_rocket_collision->Create(engine, *mine, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

			(*mine)->AddComponent(mine_rocket_collision);
			(*mine)->AddReceiver(this);

			(*mine)->Create();
			(*mine)->AddComponent(behaviour);
			(*mine)->AddComponent(render);
		}



		//***************** PLAYER *******************
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
		player_behaviour->InitKeys(&keys);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(engine, player, &game_objects, "data/player.png", 43, 43);
		
		CollideComponent * player_bomb_collide = new CollideComponent();			//Collide with enemy bullet
		player_bomb_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & bombs_pool, { 0, 0, 43, 43 });

		CollideComponent* player_laser_collide = new CollideComponent();
		player_laser_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*)& alienLaser_pool, { 0, 0, 43, 43 });

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_bomb_collide);
		player->AddComponent(player_laser_collide);
		player->AddReceiver(this);
		game_objects.insert(player);

		 
		// **********************  ALIEN S   ************************

		alien = new Alien();
		AlienBehaviourComponent* alien_behaviour = new AlienBehaviourComponent();
		alien_behaviour->Create(engine, alien, &game_objects, &bombs_pool);
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, alien, &game_objects, "data/alien_s.png", 53, 44);
		CollideComponent* alien_bullet_collide = new CollideComponent();
		alien_bullet_collide->Create(engine, alien, &game_objects, (ObjectPool<GameObject>*)& rockets_pool, { 0, 0, 53, 44 });
		SingleObjectCollideComponent* alien_player_collide = new SingleObjectCollideComponent();
		alien_player_collide->Create(engine, alien, &game_objects, player);
		alien->Create();
		alien->AddComponent(alien_behaviour);
		alien->AddComponent(alien_render);
		alien->AddComponent(alien_bullet_collide);
		alien->AddComponent(alien_player_collide);
		alien->AddReceiver(this);
		game_objects.insert(alien);

		alien2 = new Alien2();
		AlienBehaviourComponent* alien2_behaviour = new AlienBehaviourComponent();
		alien2_behaviour->Create(engine, alien2, &game_objects, &bombs_pool);
		RenderComponent* alien2_render = new RenderComponent();
		alien2_render->Create(engine, alien2, &game_objects, "data/alien_s.png", 53, 44);
		CollideComponent* alien2_bullet_collide = new CollideComponent();
		alien2_bullet_collide->Create(engine, alien2, &game_objects, (ObjectPool<GameObject>*)& rockets_pool, { 0, 0, 53, 44 });
		SingleObjectCollideComponent* alien2_player_collide = new SingleObjectCollideComponent();
		alien2_player_collide->Create(engine, alien2, &game_objects, player);
		alien2->Create();
		alien2->AddComponent(alien2_behaviour);
		alien2->AddComponent(alien2_render);
		alien2->AddComponent(alien2_bullet_collide);
		alien2->AddComponent(alien2_player_collide);
		alien2->AddReceiver(this);
		game_objects.insert(alien2);

		////************** ALIEN G GRID *******************   
		alien_g_grid = new AlienGGrid();
		AlienGGridBehaviourComponent* alien_grid_behaviour = new AlienGGridBehaviourComponent();
		alien_grid_behaviour->Create(engine, alien_g_grid, &game_objects, &alien_g_pool);

		alien_g_grid->Create();
		alien_g_grid->AddComponent(alien_grid_behaviour);
		alien_g_grid->AddReceiver(this); // alien_g_grid can send message to game
		game_objects.insert(alien_g_grid);

		//**************CREATE ALIEN G POOL***************
		alien_g_pool.Create(6); // create alien g pool of 6 aliens
		for (auto alien_g = alien_g_pool.pool.begin(); alien_g != alien_g_pool.pool.end(); alien_g++)
		{
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *alien_g, &game_objects, "data/alien_g.png", 50, 38);
			AlienGBehaviourComponent* behaviour = new AlienGBehaviourComponent();
			behaviour->Create(engine, *alien_g, &game_objects);
			CollideComponent* alienG_rocket_collide = new CollideComponent();
			alienG_rocket_collide->Create(engine, *alien_g, &game_objects, (ObjectPool<GameObject>*) & rockets_pool, { 0, 0, 50, 38 });
			SingleObjectCollideComponent* alienG_player_collide = new SingleObjectCollideComponent();
			alienG_player_collide->Create(engine, * alien_g, & game_objects, player);
			
			(*alien_g)->Create();
			(*alien_g)->AddComponent(render); 
			(*alien_g)->AddComponent(behaviour);
			(*alien_g)->AddComponent(alienG_rocket_collide);
			(*alien_g)->AddComponent(alienG_player_collide);
			(*alien_g)->AddReceiver(this);
		}

		////************** ALIEN V GRID ******************* 
		alien_v_grid = new AlienVGrid();
		AlienVGridBehaviourComponent* alien_v_grid_behaviour = new AlienVGridBehaviourComponent();
		alien_v_grid_behaviour->Create(engine, alien_v_grid, &game_objects, &alien_v_pool, &alienLaser_pool);

		alien_v_grid->Create();
		alien_v_grid->AddComponent(alien_v_grid_behaviour);
		alien_v_grid->AddReceiver(this); // alien_v_grid can send message to game
		game_objects.insert(alien_v_grid);

		//**************CREATE ALIEN V POOL***************
		alien_v_pool.Create(7); // create alien v pool of 7 aliens	
		for (auto alien_v = alien_v_pool.pool.begin(); alien_v != alien_v_pool.pool.end(); alien_v++)
		{
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *alien_v, &game_objects, "data/alien_v.png", 33, 33);		
			AlienVBehaviourComponent* behaviour = new AlienVBehaviourComponent();
			behaviour->Create(engine, *alien_v, &game_objects, &alienLaser_pool);
			CollideComponent* alienV_rocket_collide = new CollideComponent();
			alienV_rocket_collide->Create(engine, *alien_v, &game_objects, (ObjectPool<GameObject>*) & rockets_pool, { 0, 0, 33, 33 });
			SingleObjectCollideComponent* alienV_player_collide = new SingleObjectCollideComponent();
			alienV_player_collide->Create(engine, *alien_v, &game_objects, player);

			(*alien_v)->Create();
			(*alien_v)->AddComponent(render);
			(*alien_v)->AddComponent(behaviour);
			(*alien_v)->AddComponent(alienV_rocket_collide);
			(*alien_v)->AddComponent(alienV_player_collide);
			(*alien_v)->AddReceiver(this);
		}

		// LIFE PICKUP
		life_pickup = new PickupLife();
		PickupLifeBehaviourComponent* life_pickup_behaviour = new PickupLifeBehaviourComponent();
		life_pickup_behaviour->Create(engine, life_pickup, &game_objects);
		
		RenderComponent* life_pickup_render = new RenderComponent();
		life_pickup_render->Create(engine, life_pickup, &game_objects, "data/life.png", 33, 33);

		LifePickupCollisionComponent* life_pickup_collide = new LifePickupCollisionComponent();
		life_pickup_collide->Create(engine, life_pickup, &game_objects, player);

		life_pickup->Create();
		life_pickup->AddComponent(life_pickup_behaviour);
		life_pickup->AddComponent(life_pickup_render);
		life_pickup->AddComponent(life_pickup_collide);
		life_pickup->AddReceiver(this);
		game_objects.insert(life_pickup);



		// *******************BOSS ALIEN ************************

		boss_alien = new BossAlien();
		BossAlienBehaviourComponent* boss_behaviour = new BossAlienBehaviourComponent();
		boss_behaviour->Create(engine, boss_alien, &game_objects, &mines_pool);
		RenderComponent* boss_render = new RenderComponent();
		boss_render->Create(engine, boss_alien, &game_objects, "data/boss_blue.png", 130, 212);
		CollideComponent* boss_bullet_collide = new CollideComponent();
		boss_bullet_collide->Create(engine, boss_alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool, { 0,0,130,212 });
		BossObjectCollideComponent* boss_player_collide = new BossObjectCollideComponent();
		boss_player_collide->Create(engine, boss_alien, &game_objects, player);

	//	SingleObjectCollideComponent* alien_player_collide = new SingleObjectCollideComponent();
	//	alien_player_collide->Create(engine, alien, &game_objects, player);

		boss_alien->Create();
		boss_alien->AddComponent(boss_behaviour);
		boss_alien->AddComponent(boss_render);
		boss_alien->AddComponent(boss_bullet_collide);
		boss_alien->AddComponent(boss_player_collide);

	//	alien->AddComponent(alien_player_collide);
		boss_alien->AddReceiver(this);
		game_objects.insert(boss_alien);

	}

	float randomDelay()
	{
		//generate random float between 0.0 and 10.0
		return (rand() % 10);
	}

	virtual void Init()
	{
		init_time = engine->getElapsedTime();
		current_level = 1;
		current_level_sequence = level1_spawns;

		// Set background to lila
		AvancezLib::RGBColor LILA = { 99, 0, 191 };
		engine->SetBackgroundColor(LILA);
		
		player->Init();
		enabled = true;
	}

	void InitNewLevel() {
		level_finished = false;
		level_win = false;
		current_level = 2;
		// Set background to soemthing
		AvancezLib::RGBColor c = { 191, 0, 99 };
		engine->SetBackgroundColor(c);

		// Load spawn sequences for level 2 and 
		// reset current sequence counter to 0
		current_level_sequence = level2_spawns;
		seq_count = 0;
		init_time = engine->getElapsedTime();

		engine->PlaySFX("data/audio/win_level.wav", 0, -1);
	}

	virtual void Update(float dt)
	{
		engine->getKeyStatus(keys);
		if (keys.esc) {
			enabled = false;
			//return;
			Destroy();
			engine->quit();
		}
		
		if (keys.cheat) {
			SDL_Log("Cheat mode enabled");
			cheat_mode = true;
			ROCKET_SPEED = ROCKET_SPEED * 1.15f;
			FIRE_TIME_INTERVAL = FIRE_TIME_INTERVAL / 10;
		}

		if (level_finished && level_win) {
			InitNewLevel();
		}

		// Current_level counter should not exceed level total size
		// Only spawn new enermies if level_finished is false
		if (seq_count < current_level_sequence.size() && !level_finished)
		{
			// delay + init_time to convert relative delay time to absolute time
			if (engine->getElapsedTime() > current_level_sequence.at(seq_count).delay + init_time) {
				const Sequence seq = current_level_sequence.at(seq_count);
				Spawn(seq.type, seq.pos_y);
				seq_count++;
			}
		}


		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		{
			// If game_over is true, set dt to 0 so that all objects will not move.
			if (game_over || game_won)
				(*go)->Update(0);
			else
				(*go)->Update(dt);
		}
	} 

	virtual void Draw()
	{
		//... Draw user interface elements here

		//Draw current lives indicator
		// memory leak here, create too many sprite objects but not destroying them
		//for (int i = 0; i <= player->lives; i++) {
		//	Sprite* life_sprite = engine->createSprite("data/heart.gif", 15, 12); 
		//	life_sprite->draw(20 * i, 20); 
		//}
		sprintf(life_string, "Life: %d", player->lives);
		engine->drawText(0, 16, life_string);

		relative_time = engine->getElapsedTime() - init_time;
		sprintf(debug_string, "T: %.2f R-T: %.2f Level: %d Seq: %d", engine->getElapsedTime(), relative_time, current_level, seq_count);
		engine->drawText(230, 16, debug_string);

		//Score indicator
		sprintf(score_string, "%07d", score);
		engine->drawText(500, 16, score_string);
		
		if (game_over) {
			engine->drawText(250, 0, "***GAME OVER***");
		}

		if (game_won) {
			engine->drawText(250, 0, "***YOU WON***");
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
			engine->PlaySFX("data/audio/game_over.wav", 0, -1);
		}

		if (m == BOSS_KILLED)  // call this boss killed
		//if boss alien dies
		// New level, prepare aliens and new background
		{
			SDL_Log("GAME::BOSS_KILLED");   
			engine->PlaySFX("data/audio/boss_die.wav", 0, -1);
			if (current_level < 2) {
				level_win = true;
				level_finished = true;
			}
			else game_won = true;
		}

		if (m == ALIEN_G_LEVEL_CLEAR) {
			SDL_Log("GAME::ALIEN_G_LEVEL_CLEAR!");
			if (!init_new_alien_g)
				init_new_alien_g = true;
		}

		if (m == ALL_ALIENS_V_CLEAR) {
			SDL_Log("GAME::ALL_ALIENS_V_CLEAR!");
			if (!init_new_alien_v)
				init_new_alien_v = true;
		}
		

		if (m == HIT) {
			engine->PlaySFX("data/audio/player_hit.wav", 0, -1);
		}

		if (m == BOSS_HIT) {
			engine->PlaySFX("data/audio/player_hit.wav", 0, -1);
		}


		if(m == LIFE_PICKED){
		// If the +UP collected, add one life to the player
			//player->AddLife();
			engine->PlaySFX("data/audio/pickup_sound.wav", 0, -1);
		}

		if (m == ALIEN_HIT || m == ALIEN_G_HIT || m == ALIEN_V_HIT)
		{
			SDL_Log("GAME::ALIEN_HIT!");
			score += POINTS_PER_ALIEN;
			engine->PlaySFX("data/audio/alien_v_die.wav", 0, -1);
		}

		if (m == BULLET_BULLET_HIT) {
			SDL_Log("GAME::BULLET_BULLET_HIT!");
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
		rockets_pool.Destroy();
		life_sprite->destroy();
		bombs_pool.Destroy();
		alien_g_pool.Destroy();
		alien_v_pool.Destroy();
		alienLaser_pool.Destroy();
		mines_pool.Destroy();

		//life_pickup.Destroy();

		//aliens_pool.Destroy();

		delete player;
		delete alien;
		delete alien2;
		delete alien_g_grid; 
		/*
		delete alien_v;
		delete life_pickup;
		
		*/

		delete boss_alien;

		// Mark game class as disabled
		enabled = false;
	}

private:
	void Spawn(Sequence::ALIEN_TYPE type, float pos_y) {

		switch (type)
		{
		case Sequence::ALIEN_TYPE::ALIEN_S:
			SDL_Log("Spwaning ALIEN_S");
			alien->Init(pos_y);
			break;

		case Sequence::ALIEN_TYPE::ALIEN_S_2:
			SDL_Log("Spwaning ALIEN_S_2");
			alien2->Init(pos_y);
			break;

		case Sequence::ALIEN_TYPE::ALIEN_G:
		{
			SDL_Log("Spwaning ALIEN_G");
			std::vector<Vector2D> xys = MakeGShapeAlienPositions(pos_y, 6);
			int index = 0;
			float delay = 0.f;
			for (auto it = alien_g_pool.pool.begin(); it != alien_g_pool.pool.end(); it++)
			{
				Vector2D xy = xys.at(index);
				(*it)->horizontalPosition = xy.x;
				(*it)->verticalPosition = xy.y;
				(*it)->GetComponent<AlienGBehaviourComponent*>()->SetDelay(delay);
				(*it)->Init();
				index++;
				// Increase delay every column
				if (index % 2 == 0)
					delay += 1.2f;
			}

			alien_g_grid->Init();
			break;
		}

		case Sequence::ALIEN_TYPE::ALIEN_V:
		{
			SDL_Log("Spwaning ALIEN_V");
			std::vector<Vector2D> xys = MakeVShapeAlienPositions(pos_y, 7);
			int index = 0;
			for (auto it = alien_v_pool.pool.begin(); it != alien_v_pool.pool.end(); it++)
			{
				Vector2D xy = xys.at(index);
				(*it)->horizontalPosition = xy.x;
				(*it)->verticalPosition = xy.y;
				(*it)->Init();
				index++;
			}
			alien_v_grid->Init(0.f);
			break;
		}

		case Sequence::ALIEN_TYPE::BOSS:
			SDL_Log("Spawning BOSS");
			boss_alien->Init();
			//engine->PlaySFX("data/audio/boss_appear.wav", 0, -1);
		}

		//case Sequence::ALIEN_TYPE::BOSS:
		return;
	}

	// TODO remove this
	float AlienRandomHeight() {
		return 30.f + rand() % 420; // generate random height between 30 to 450
	}

	float AlienGRandomHeight() {
		return 50.f + rand() % 350; // generate random height between 50 and 400
	}

	float AlienVRandomHeight() {
		return 120.f + rand() % 240; // generate random height between 120 and 360
	}

	// Generate position x and y value to draw V shape aliens
	// alien_num should be an odd number: 3, 5, 7
	std::vector<Vector2D> MakeVShapeAlienPositions(float first_alien_y, int alien_num) {
		std::vector<Vector2D> pos;
		float init_x = 640, init_y = first_alien_y;

		pos.push_back(Vector2D(init_x, init_y));

		for (auto i = 1; i < alien_num; i++) {
			int col_num = (i+1) / 2;
			float y;

			if (i % 2 == 0)
				y = init_y + 30 * col_num;
			else
				y = init_y - 30 * col_num;

			pos.push_back(Vector2D((init_x + col_num * 30.f), y));
		}

		return pos;
	}

	// Generate position x and y value to draw alien G shapes
	// alien_num should be even numbers: 2, 4, 6...
	std::vector<Vector2D> MakeGShapeAlienPositions(float first_alien_y, int alien_num) {
		std::vector<Vector2D> pos;
		float init_x = 640, init_y = first_alien_y;

		for (auto i = 0; i < alien_num; i++) {
			Vector2D alien_pos;

			int col_num = i / 2;
			printf("col_num %d\n", col_num);

			alien_pos.x = init_x + col_num * 100.f;

			if (i % 2 == 0)
				alien_pos.y = init_y;
			else
				alien_pos.y = init_y + 100.f;

			pos.push_back(alien_pos);
		}

		return pos;
	}
};