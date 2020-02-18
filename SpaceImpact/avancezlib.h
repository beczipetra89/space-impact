#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Sprite.h"

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
	void drawText(int x, int y, const char* msg);

	// Return the total time spent in the game, in seconds.
	float getElapsedTime();

	struct KeyStatus
	{
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
	SDL_Renderer* renderer;

private:
	SDL_Window* window;
	//For rendering collider box
	//SDL_Renderer* renderer;

	TTF_Font* font;
};
