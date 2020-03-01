#pragma once
struct Sequence {
	enum class OBJECT_TYPE {
		ALIEN_S, 
		ALIEN_S_2, 
		ALIEN_G, 
		ALIEN_V,
		LIFE_PICKUP,
		BOSS,
		BOSS2
	};

	OBJECT_TYPE type;
	float pos_y;
	float delay;

	bool enabled = false;
	bool finished = false;
};

const std::vector<Sequence> level1_spawns{
	
	// Level 1 spawn aliens for 02:43, then spawn the Boss 1
	
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 200.f, 0.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 300.f, 6.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 100.f, 12.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 150.f, 18.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 150.f, 28.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 500.f, 34.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 250.f, 38.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 350.f, 44.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 400.f, 48.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 300.f, 54.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 440.f, 58.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 350.f, 62.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 150.f, 68.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 450.f, 74.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 250.f, 78.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 300.f, 84.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 200.f, 88.f},
		Sequence{Sequence::OBJECT_TYPE::LIFE_PICKUP, 250.f, 90.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 300.f, 92.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 280.f, 98.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 250.f, 105.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 120.f, 111.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 350.f, 115.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 150.f, 121.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 450.f, 127.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 250.f, 131.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 370.f, 137.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 500.f, 140.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 300.f, 140.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 200.f, 144.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 250.f, 150.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 480.f, 156.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 300.f, 159.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 300.f, 162.f},
	Sequence{Sequence::OBJECT_TYPE::BOSS, -1.f, 169.f},		// postion Y is ignored for boss
		Sequence{Sequence::OBJECT_TYPE::LIFE_PICKUP, 250.f, 173.f}

};

const std::vector<Sequence> level2_spawns{
	//Level 2: spawn aliens for 2:29 then spawn Boss 2
	
		Sequence{Sequence::OBJECT_TYPE::LIFE_PICKUP, 120.f, 0.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 200.f, 1.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 300.f, 6.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 100.f, 12.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 150.f, 18.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 150.f, 28.f },
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 500.f, 34.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 250.f, 38.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 350.f, 44.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 400.f, 48.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 300.f, 54.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 440.f, 58.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 350.f, 62.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 150.f, 68.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 450.f, 74.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 250.f, 78.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 300.f, 84.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 200.f, 88.f},
		Sequence{Sequence::OBJECT_TYPE::LIFE_PICKUP, 250.f, 90.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 300.f, 92.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 380.f, 98.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_V, 250.f, 105.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 120.f, 111.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S_2, 350.f, 115.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_G, 150.f, 121.f},
	Sequence{Sequence::OBJECT_TYPE::ALIEN_S, 450.f, 127.f},  
	Sequence{Sequence::OBJECT_TYPE::BOSS2, -1.f, 133.f},		// postion Y is ignored for boss
		Sequence{Sequence::OBJECT_TYPE::LIFE_PICKUP, 250.f, 139.f}
		
};