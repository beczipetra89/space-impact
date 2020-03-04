#pragma once

#ifndef _AVANCEZLIB_H
#define _AVANCEZLIB_H
#include "SDL.h"
#include "SDL_ttf.h"
#include "Sprite.h"
#include "SDL_mixer.h"
#include <map>
#include <string>


class AvancezLib
{

public:
	// Destroys the avancez library instance
	void destroy();

	// Destroys the avancez library instance and exits
	void quit();

	// Creates the main window. Returns true on success.
	bool init(int width, int height);

	// Clears the screen and draws all sprites and texts which have been drawn
	// since the last update call.
	// If update returns false, the application should terminate.
	void processInput();

	void swapBuffers();

	void clearWindow();

	// Create a sprite given a string.
	// All sprites are 32*32 pixels.
	Sprite* createSprite(const char* name, int w, int h);

	// Draws the given text.
	void drawText(int x, int y, const char* msg, int fontSize);

	// Return the total time spent in the game, in seconds.
	float getElapsedTime();

	struct KeyStatus
	{
		bool switch_weapon;	// "s" pressed
		bool fire; // space
		bool up; // uparrow
		bool down; // down arrow
		bool left; // left arrow
		bool right; // right arrow
		bool esc; // escape button
		bool cheat; // cheat mode
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);

	struct RGBColor {
		int r, g, b;
	};

	void SetBackgroundColor(RGBColor& color);

	//For rendering collider box
	SDL_Renderer* renderer;

	std::map<std::string, Mix_Music*>mMusic;	//This map will hold all the music files that we will be loading
	std::map<std::string, Mix_Chunk*>mSFX;		//This map will hold all the sound effects that we will be loading

	Mix_Music* GetMusic(std::string filename);
	Mix_Chunk* GetSFX(std::string filename);
	void PlayMusic(std::string filename, int loops = -1); // Music will play only once
	void PauseMusic();
	void ResumeMusic();

	void PlaySFX(std::string filename, int loops = 0, int channel = 0);

private:
	SDL_Window* window;
	//For rendering collider box
	//SDL_Renderer* renderer;

	TTF_Font* font;
};
#endif // _AVANCEZLIB_H