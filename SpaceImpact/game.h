#pragma once
#include "sstream"
enum ALIEN_TYPE {
	ALIEN_S,
	ALIEN_G,
	ALIEN_V
};
const float LEVEL1_DELAY[] = {
	0.5f, // ALIEN_S init delays for 0.5 second
	0.f,  // For ALIEN_G and ALIEN_V no delays
	0.f
};

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;
	AvancezLib::KeyStatus keys = { false, false, false, false, false};
	bool cheat_mode = false;
	float init_time; // the time when Game object init() is called
	bool init_new_alien = false; // whether to init new alien
	bool init_new_alien_g = false; // whether to init new alien_g in alien_g_grid
	bool init_new_alien_v = false; 

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	ObjectPool<Bomb> bombs_pool;
	ObjectPool<AlienG> alien_g_pool;
	ObjectPool<AlienV> alien_v_pool;
	ObjectPool<AlienLaser> alienLaser_pool;
	ObjectPool<Mine> mines_pool;

	Player * player;
	Alien* alien;
	AlienG* alien_g;
	AlienGGrid* alien_g_grid;
	AlienV* alien_v;
	AlienVGrid* alien_v_grid;
	PickupLife* life_pickup;

	BossAlien* boss_alien;
	
	//AlienGrid* alien_grid;

	Sprite * life_sprite;
	bool game_over = false;

	unsigned int score = 0;

	// for drawing text
	char life_string[10];
	char score_string[10];

public:
	
	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");
		this->engine = engine;
		SDL_Log("%d", ALIEN_TYPE::ALIEN_G);

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
		player_bomb_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & bombs_pool);

		CollideComponent* player_laser_collide = new CollideComponent();
		player_laser_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & alienLaser_pool);

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
		alien_bullet_collide->Create(engine, alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);


		SingleObjectCollideComponent* alien_player_collide = new SingleObjectCollideComponent();
		alien_player_collide->Create(engine, alien, &game_objects, player);
		
		alien->Create();
		alien->AddComponent(alien_behaviour);
		alien->AddComponent(alien_render);
		alien->AddComponent(alien_bullet_collide);
		
		alien->AddComponent(alien_player_collide);
		
		alien->AddReceiver(this);
		game_objects.insert(alien);   

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
		float alien_g_x = 660.f, alien_g_y = 50.f, delay = 1.f;
		int alien_g_count = 1;
		for (auto alien_g = alien_g_pool.pool.begin(); alien_g != alien_g_pool.pool.end(); alien_g++)
		{
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *alien_g, &game_objects, "data/alien_g.png", 50, 38);
			(*alien_g)->Create();
			(*alien_g)->AddComponent(render);

			AlienGBehaviourComponent* behaviour = new AlienGBehaviourComponent();
			behaviour->Create(engine, *alien_g, &game_objects, delay);
			
			CollideComponent* alienG_rocket_collide = new CollideComponent();
			alienG_rocket_collide->Create(engine, *alien_g, &game_objects, (ObjectPool<GameObject>*)& rockets_pool);

			SingleObjectCollideComponent* alienG_player_collide = new SingleObjectCollideComponent();
			alienG_player_collide->Create(engine, * alien_g, & game_objects, player);
			
			(*alien_g)->AddComponent(behaviour);
			(*alien_g)->AddComponent(alienG_rocket_collide);
			(*alien_g)->AddComponent(alienG_player_collide);
			(*alien_g)->AddReceiver(this);

			(*alien_g)->horizontalPosition = alien_g_x;
			(*alien_g)->verticalPosition = alien_g_y;
			(*alien_g)->Init();


			alien_g_y = alien_g_y + 100;
			if (alien_g_count % 2 == 0) {
				alien_g_x = alien_g_x + 100; // space between the alien g columns
				alien_g_y = 50;
				delay = delay + 1.0f;
			}

			alien_g_count++;
		}

		////************** ALIEN V GRID ******************* 
		alien_v_grid = new AlienVGrid();
		AlienVGridBehaviourComponent* alien_v_grid_behaviour = new AlienVGridBehaviourComponent();
		alien_v_grid_behaviour->Create(engine, alien_v_grid, &game_objects, &alien_v_pool, &alienLaser_pool);

		alien_v_grid->Create();
		alien_v_grid->AddComponent(alien_v_grid_behaviour);
		game_objects.insert(alien_v_grid);
		alien_v_grid->AddReceiver(this); // alien_v_grid can send message to game

		//**************CREATE ALIEN V POOL***************
		alien_v_pool.Create(7); // create alien v pool of 7 aliens
		std::vector<AlienV::Coordinate> alien_v_coordinates = MakeVShapeAlienPositions({ 660.f, AlienVRandomHeigh() }, 7);
		int alien_v_count = 0;		
		for (auto alien_v = alien_v_pool.pool.begin(); alien_v != alien_v_pool.pool.end(); alien_v++)
		{
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *alien_v, &game_objects, "data/alien_v.png", 33, 33);
		
			(*alien_v)->Create();	
			(*alien_v)->AddComponent(render);

			AlienVBehaviourComponent* behaviour = new AlienVBehaviourComponent();
			behaviour->Create(engine, *alien_v, &game_objects, &alienLaser_pool);
		
			CollideComponent* alienV_rocket_collide = new CollideComponent();
			alienV_rocket_collide->Create(engine, *alien_v, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

			SingleObjectCollideComponent* alienV_player_collide = new SingleObjectCollideComponent();
			alienV_player_collide->Create(engine, *alien_v, &game_objects, player);

			(*alien_v)->AddComponent(behaviour);
			(*alien_v)->AddComponent(alienV_rocket_collide);
			(*alien_v)->AddComponent(alienV_player_collide);
			(*alien_v)->AddReceiver(this);

			(*alien_v)->horizontalPosition = alien_v_coordinates.at(alien_v_count).x;
			(*alien_v)->verticalPosition = alien_v_coordinates.at(alien_v_count).y;
			(*alien_v)->Init();

			alien_v_count++;
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
		boss_render->Create(engine, boss_alien, &game_objects, "data/boss_b.png", 110, 135);
		CollideComponent* boss_bullet_collide = new CollideComponent();
		boss_bullet_collide->Create(engine, boss_alien, &game_objects, (ObjectPool<GameObject>*)& rockets_pool);

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
		player->Init();
		alien->Init(AlienRandomHeigh());
		alien_g_grid->Init();
		alien_v_grid->Init(0.f + randomDelay()); 
		//alien_v_grid->Init(0.f);

		/*	alien->Init();
		//alien_g->Init();
		alien_g_grid->Init();  
		alien_v_grid->Init();
		life_pickup->Init();
		*/
		//boss_alien->Init();

		// Set background to lila
		AvancezLib::RGBColor LILA = { 99, 0, 191 };
		engine->SetBackgroundColor(LILA);

		enabled = true;
		init_time = engine->getElapsedTime();
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

		if (init_new_alien_g && engine->getElapsedTime() < init_time + 60 * 5.f) // if game runs for less than 5 minutes then we can start new aliens
		{
			init_new_alien_g = false;

			float alien_g_x = 660.f, alien_g_y = 50.f, delay = 1.0f;
			int alien_g_count = 1;
			for (auto alien_g = alien_g_pool.pool.begin(); alien_g != alien_g_pool.pool.end(); alien_g++)
			{
				(*alien_g)->horizontalPosition = alien_g_x;
				(*alien_g)->verticalPosition = alien_g_y;
				(*alien_g)->Init();
				AlienGBehaviourComponent* component = (*alien_g)->GetComponent<AlienGBehaviourComponent*>();
				component->SetDelay(delay);

				alien_g_y = alien_g_y + 100;
				if (alien_g_count % 2 == 0) {
					alien_g_x = alien_g_x + 100; // space between the alien g columns
					alien_g_y = 50;
					delay = delay + 1.0f;
				}

				alien_g_count++;
			}
			alien_g_grid->Init();
		}

		if (init_new_alien_v)
		{
			init_new_alien_v = false;

			std::vector<AlienV::Coordinate> alienv_pos = MakeVShapeAlienPositions({660.f, AlienVRandomHeigh()}, 7);
			int alien_v_count = 0;
			for (auto alien_v = alien_v_pool.pool.begin(); alien_v != alien_v_pool.pool.end(); alien_v++)
			{
				AlienV::Coordinate pos = alienv_pos.at(alien_v_count);
				(*alien_v)->horizontalPosition = pos.x;
				(*alien_v)->verticalPosition = pos.y;
				(*alien_v)->Init();
				alien_v_count++;
			}
			alien_v_grid->Init(randomDelay());
		}

		if (init_new_alien)
		{
			alien->Init(AlienRandomHeigh()); // random vertical position
			alien->GetComponent<AlienBehaviourComponent*>()->setInitDelay(randomDelay());
			init_new_alien = false;
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
		// memory leak here, create too many sprite objects but not destroying them
		//for (int i = 0; i <= player->lives; i++) {
		//	Sprite* life_sprite = engine->createSprite("data/heart.gif", 15, 12); 
		//	life_sprite->draw(20 * i, 20); 
		//}
		sprintf(life_string, "Life: %d", player->lives);
		engine->drawText(0, 16, life_string);

		AvancezLib::RGBColor LILA = { 99, 0, 191 };
		engine->SetBackgroundColor(LILA);

		//Score indicator
		sprintf(score_string, "%07d", score);
		engine->drawText(500, 16, score_string);

		
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

		if (m == LEVEL_WIN)  //if boss alien dies
		{
			SDL_Log("GAME::LEVEL_WIN");

			// New level, prepare aliens and new background
		}

		if (m == ALIEN_G_LEVEL_CLEAR) {
			SDL_Log("GAME::ALIEN_G_LEVEL_CLEAR!");
			if (!init_new_alien_g)
				init_new_alien_g = true;
			//boss_alien->Init();
		}

		if (m == ALL_ALIENS_V_CLEAR) {
			SDL_Log("GAME::ALL_ALIENS_V_CLEAR!");
			if (!init_new_alien_v)
				init_new_alien_v = true;
		}
		
		if(m == LIFE_PICKED){
		// If the +UP collected, add one life to the player
			//player->AddLife();
		}

		if (m == ALIEN_HIT || m == ALIEN_G_HIT || m == ALIEN_V_HIT)
		{
			SDL_Log("GAME::ALIEN_HIT!");
			score += POINTS_PER_ALIEN;
		}

		if (m == BULLET_BULLET_HIT) {
			SDL_Log("GAME::BULLET_BULLET_HIT!");
		}

		if (m == ALIEN_HIT || m == ALIEN_LEVEL_CLEAR)
		{
			if (!init_new_alien)
				init_new_alien = true;
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
		alien_v_pool.Destroy();
		alienLaser_pool.Destroy();

		

		mines_pool.Destroy();

		//life_pickup.Destroy();

		//aliens_pool.Destroy();

		delete player;
/*		delete alien;
	//	delete alien_grid; 
		delete alien_g;  
		delete alien_v;
		delete life_pickup;
		
		*/

		delete boss_alien;

		// Mark game class as disabled
		enabled = false;
	}

	float AlienRandomHeigh() {
		return 30.f + rand() % 420; // generate random height between 30 to 450
	}
	float AlienVRandomHeigh() {
		return 120.f + rand() % 240; // generate random height between 120 and 360
	}
	// Generate position x and y value to draw V shape aliens
	// alien_num should be an odd number: 3, 5, 7
	std::vector<AlienV::Coordinate> MakeVShapeAlienPositions(AlienV::Coordinate first_alien_xy, int alien_num) {
		std::vector<AlienV::Coordinate> alien_v_coordinates;

		alien_v_coordinates.push_back(first_alien_xy);

		for (auto i = 1; i < alien_num; i++) {
			int col_num = (i+1) / 2;
			printf("col_num %d\n", col_num);
			float y = first_alien_xy.y;
			if (i % 2 == 0)
				y = y + 30 * col_num;
			else
				y = y - 30 * col_num;

			alien_v_coordinates.push_back({ (first_alien_xy.x + col_num * 30.f), y});
		}

		return alien_v_coordinates;
	}
};