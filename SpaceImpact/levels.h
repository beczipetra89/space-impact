#pragma once
struct Sequence {
	enum class ALIEN_TYPE {
		ALIEN_S, 
		ALIEN_S_2, 
		ALIEN_G, 
		ALIEN_V,
		BOSS
	};

	ALIEN_TYPE type;
	float pos_y;
	float delay;

	bool enabled = false;
	bool finished = false;
};

const std::vector<Sequence> level1_spawns{
	//Level{Level::ALIEN_TYPE::ALIEN_G, 200.f, 0.f}
	//Level{Level::ALIEN_TYPE::ALIEN_V, 200.f, 0.f}
	//Sequence{Sequence::ALIEN_TYPE::ALIEN_S, 300.f, 0.f },
	//Sequence{Sequence::ALIEN_TYPE::ALIEN_S_2, 100.f, 6.f},
	//Sequence{Sequence::ALIEN_TYPE::ALIEN_S, 60.f, 9.f},
	//Sequence{Sequence::ALIEN_TYPE::ALIEN_G, 300.f, 12.f},
	Sequence{Sequence::ALIEN_TYPE::BOSS, -1.f, 0.f} // postion Y is ignored for boss
	/*
	Level{Level::ALIEN_TYPE::ALIEN_V, 240.f, 20.f},
	Level{Level::ALIEN_TYPE::ALIEN_S, 60.f, 28.f},
	Level{Level::ALIEN_TYPE::ALIEN_S_2, 400.f, 30.f},
	Level{Level::ALIEN_TYPE::ALIEN_G, 240, 33.f},
	
	Level{Level::ALIEN_TYPE::ALIEN_S, 80, 36.f},
	Level{Level::ALIEN_TYPE::ALIEN_S_2, 130.f, 38.f},
	Level{Level::ALIEN_TYPE::ALIEN_G, 230, 40.f},

	Level{Level::ALIEN_TYPE::ALIEN_V, 240.f, 44.f},
	Level{Level::ALIEN_TYPE::ALIEN_S, 450, 46.f},
	Level{Level::ALIEN_TYPE::ALIEN_S_2, 300, 48.f},
	Level{Level::ALIEN_TYPE::ALIEN_G, 300.f, 50.f}
	*/
};

const std::vector<Sequence> level2_spawns{
	//Level{Level::ALIEN_TYPE::ALIEN_G, 200.f, 0.f}
	//Level{Level::ALIEN_TYPE::ALIEN_V, 200.f, 0.f}
	Sequence{Sequence::ALIEN_TYPE::ALIEN_S, 300.f, 0.f },
	Sequence{Sequence::ALIEN_TYPE::ALIEN_S_2, 100.f, 6.f},
	//Sequence{Sequence::ALIEN_TYPE::ALIEN_S, 60.f, 9.f},
	Sequence{Sequence::ALIEN_TYPE::ALIEN_G, 300.f, 12.f},
	Sequence{Sequence::ALIEN_TYPE::BOSS, -1.f, 15.f}
};