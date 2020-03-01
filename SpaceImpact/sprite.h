#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

class Sprite
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int width;
	int height;

public:
	// Create Sprite with specified width and height
	Sprite(SDL_Renderer* renderer, SDL_Texture* texture, int w, int h);

	// Destroys the sprite instance
	void destroy();
	
	SDL_Rect rect; // Sprite rect for drawing
	// Draw the sprite at the given position.
	// Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
	// (All sprites are 32*32 pixels, clipping is not supported)
	void draw(int x, int y);
	void partialDraw(SDL_Rect* src_rect, SDL_Rect* dst_rect);
};
