#pragma once
struct Level {
	enum class ALIEN_TYPE {
		ALIEN_S, 
		ALIEN_S_2, 
		ALIEN_G, 
		ALIEN_V
	};

	ALIEN_TYPE type;
	float pos_y;
	float delay;

	bool enabled = false;
	bool finished = false;
};

const std::vector<Level> levels{
	//Level{Level::ALIEN_TYPE::ALIEN_G, 200.f, 0.f}
	//Level{Level::ALIEN_TYPE::ALIEN_V, 200.f, 0.f}
	Level{Level::ALIEN_TYPE::ALIEN_S, 300.f, 0.f },
	Level{Level::ALIEN_TYPE::ALIEN_S_2, 100.f, 6.f},
	Level{Level::ALIEN_TYPE::ALIEN_S, 30.f, 12.f},
	Level{Level::ALIEN_TYPE::ALIEN_G, 300.f, 12.f},
	Level{Level::ALIEN_TYPE::ALIEN_V, 150.f, 12.f},
	Level{Level::ALIEN_TYPE::ALIEN_V, 400.f, 26.f},
	Level{Level::ALIEN_TYPE::ALIEN_S, 30.f, 28.f},
	Level{Level::ALIEN_TYPE::ALIEN_G, 100.f, 30.f}
};