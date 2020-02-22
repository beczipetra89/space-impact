#pragma once
struct Level {
	enum ALIEN_TYPE {ALIEN_S, ALIEN_G};
	ALIEN_TYPE type;
	float pos_y;
	float time;
};

const std::vector<Level> levels{
	Level{Level::ALIEN_S, 300.f, 0.f }
};