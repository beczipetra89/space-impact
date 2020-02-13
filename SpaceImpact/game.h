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
	ObjectPool<AlienG> alien_g_pool;
	ObjectPool<AlienV> alien_v_pool;
	ObjectPool<AlienLaser> alienLaser_pool;

	
	
	Player * player;
	Alien* alien;
	AlienG* alien_g;
	AlienGGrid* alien_g_grid;

	AlienV* alien_v;
	AlienVGrid* alien_v_grid;
	
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
			render->Create(engine, *rocket, &game_objects, "data/bullet1.png", 32, 32);
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
			render->Create(engine, *bomb, &game_objects, "data/bullet2.png", 32, 32);
			(*bomb)->Create();
			(*bomb)->AddComponent(behaviour);
			(*bomb)->AddComponent(render);
		}

		//**************CREATE ALIEN LASER POOL***************
		alienLaser_pool.Create(30);
		for (auto laser = alienLaser_pool.pool.begin(); laser != alienLaser_pool.pool.end(); laser++)
		{
			AlienLaserBehaviourComponent* behaviour = new AlienLaserBehaviourComponent();
			behaviour->Create(engine, *laser, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *laser, &game_objects, "data/bullet2.png", 32, 32);
			(*laser)->Create();
			(*laser)->AddComponent(behaviour);
			(*laser)->AddComponent(render);
		}



		//***************** PLAYER *******************
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
		player_behaviour->InitKeys(&keys);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(engine, player, &game_objects, "data/player.png", 35, 35);
		
		CollideComponent * player_bomb_collide = new CollideComponent();			//Collide with enemy bullet
		player_bomb_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & bombs_pool);

		CollideComponent* player_laser_collide = new CollideComponent();
		player_laser_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & alienLaser_pool);


		//CollideComponent* player_alien_collide = new CollideComponent();
		//player_alien_collide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & alien);

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_bomb_collide);
		player->AddComponent(player_laser_collide);

		//player->AddComponent(player_alien_collide);
		player->AddReceiver(this);
		game_objects.insert(player);


		
		// *******************ENEMYS SINGLE ************************
		/*
		alien = new Alien();
		AlienBehaviourComponent* alien_behaviour = new AlienBehaviourComponent();
		alien_behaviour->Create(engine, alien, &game_objects, &bombs_pool);
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, alien, &game_objects, "data/enemySingle.png", 64, 64);
		CollideComponent* alien_bullet_collide = new CollideComponent();
		alien_bullet_collide->Create(engine, alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);
		
		alien->Create();
		alien->AddComponent(alien_behaviour);
		alien->AddComponent(alien_render);
		alien->AddComponent(alien_bullet_collide);
		alien->AddReceiver(this);
		game_objects.insert(alien);   

		*/
		

		/*
		// **********************ALIEN G****************************  TO BE DELETED----
		alien_g = new AlienG();
		AlienGBehaviourComponent* alien_g_behaviour = new AlienGBehaviourComponent();
		alien_g_behaviour->Create(engine, alien_g, &game_objects);
		RenderComponent* alien_g_render = new RenderComponent();
		alien_g_render->Create(engine, alien_g, &game_objects, "data/enemyGroup.png", 64, 64);
		alien_g->Create();
		alien_g->AddComponent(alien_g_behaviour);
		alien_g->AddComponent(alien_g_render);
		alien_g->AddReceiver(this);
		game_objects.insert(alien_g);
		*/





		/*

		////************** ALIEN G GRID ******************* 
		alien_g_grid = new AlienGGrid();
		AlienGGridBehaviourComponent* alien_grid_behaviour = new AlienGGridBehaviourComponent();
		alien_grid_behaviour->Create(engine, alien_g_grid, &game_objects, &alien_g_pool);

		alien_g_grid->Create();
		alien_g_grid->AddComponent(alien_grid_behaviour);
		
		
		
			game_objects.insert(alien_g_grid);

		//**************CREATE ALIEN G POOL***************
		alien_g_pool.Create(6); // create alien g pool of 6 aliens
		float alien_g_x = 600.f, alien_g_y = 50.f, delay = 1.f;
		int alien_g_count = 1;
		for (auto alien_g = alien_g_pool.pool.begin(); alien_g != alien_g_pool.pool.end(); alien_g++)
		{
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *alien_g, &game_objects, "data/enemyGroup.png", 64, 64);
			(*alien_g)->Create();
			(*alien_g)->AddComponent(render);

			AlienGBehaviourComponent* behaviour = new AlienGBehaviourComponent();
			behaviour->Create(engine, *alien_g, &game_objects, delay);
			(*alien_g)->AddComponent(behaviour);

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


		*/


		////************** ALIEN V GRID ******************* 
		alien_v_grid = new AlienVGrid();
		AlienVGridBehaviourComponent* alien_v_grid_behaviour = new AlienVGridBehaviourComponent();
		alien_v_grid_behaviour->Create(engine, alien_v_grid, &game_objects, &alien_v_pool, &alienLaser_pool);

		alien_v_grid->Create();
		alien_v_grid->AddComponent(alien_v_grid_behaviour);
		game_objects.insert(alien_v_grid);

		//**************CREATE ALIEN V POOL***************
		alien_v_pool.Create(7); // create alien v pool of 8 aliens
		std::vector<AlienV::Coordinate> alien_v_coordinates;
		unsigned int time_ui = unsigned int(time(NULL));
		srand(time_ui);
		float initialVY = float(rand() % 250 + 1); // generate base line Y coordinate value
		
		// coordinates, x & y, screen size w = 600, h = 480, need to spawn out of screen so 640 for the first object
		//alien_v_coordinates.push_back({ 760, 120 });
		//alien_v_coordinates.push_back({ 720, 160});
		//alien_v_coordinates.push_back({ 680, 200 });	// x + 40, y - 40
		//alien_v_coordinates.push_back({ 640, 240 });  // <---first alien
		//alien_v_coordinates.push_back({ 680, 280 });	// x + 40, y + 40
		//alien_v_coordinates.push_back({ 720, 320 });
		//alien_v_coordinates.push_back({ 760, 360 });

		// Random height + 40 (spacing)
		alien_v_coordinates.push_back({ 720, initialVY + 30 });
		alien_v_coordinates.push_back({ 690, initialVY + 60 });
		alien_v_coordinates.push_back({ 660, initialVY + 90 });	// x + 30, y - 30
		alien_v_coordinates.push_back({ 630, initialVY + 120 });  // <---first alien
		alien_v_coordinates.push_back({ 660, initialVY + 150 });	// x + 30, y + 30
		alien_v_coordinates.push_back({ 690, initialVY + 180 });
		alien_v_coordinates.push_back({ 720, initialVY + 210 });


		float alien_v_x = 300.f, alien_v_y = 200.f;
		int alien_v_count = 1;
		
		for (auto alien_v = alien_v_pool.pool.begin(); alien_v != alien_v_pool.pool.end(); alien_v++)
		{
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *alien_v, &game_objects, "data/enemyVGroup.png", 32, 32);
		
			(*alien_v)->Create();	
			(*alien_v)->AddComponent(render);

			AlienVBehaviourComponent* behaviour = new AlienVBehaviourComponent();
			behaviour->Create(engine, *alien_v, &game_objects, &alienLaser_pool);
		
			CollideComponent* alienV_rocket_collide = new CollideComponent();
			alienV_rocket_collide->Create(engine, *alien_v, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

			(*alien_v)->AddComponent(behaviour);
			(*alien_v)->AddComponent(alienV_rocket_collide);
			(*alien_v)->AddReceiver(this);

			(*alien_v)->horizontalPosition = alien_v_coordinates.at(alien_v_count-1).x;
			(*alien_v)->verticalPosition = alien_v_coordinates.at(alien_v_count - 1).y;
			(*alien_v)->Init();

			alien_v_count++;
		}

	}

	virtual void Init()
	{
		player->Init();
//		alien->Init();
		//alien_g->Init();
	//	alien_g_grid->Init();  ---ez kell
		alien_v_grid->Init();
		

		// Set background to lila
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
			Sprite* life_sprite = engine->createSprite("data/player.bmp", 32, 32);  
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
		
		if (m == ALIEN_HIT || m == ALIEN_G_HIT || m == ALIEN_V_HIT)
		{
			SDL_Log("GAME::ALIEN_HIT!");
			score += POINTS_PER_ALIEN;
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
		alien_v_pool.Destroy();
		alienLaser_pool.Destroy();

		//aliens_pool.Destroy();

		delete player;
	//	delete alien;
	//	delete alien_grid; 
	//	delete alien_g;  ---ez kell
		delete alien_v;

		// Mark game class as disabled
		enabled = false;
	}
};