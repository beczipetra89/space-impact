#include "sprite.h"
#include <iostream>

#include "avancezlib.h"

Sprite::Sprite(SDL_Renderer* renderer, SDL_Texture* texture, int w, int h)
{
	if (renderer != NULL && texture != NULL) {
		Sprite::renderer = renderer;
		Sprite::texture = texture;
		width = w;
		height = h;
	}
	else {
		std::cout << "Error creating sprite" << std::endl;
	}
}

void Sprite::destroy()
{
}

void Sprite::draw(int x, int y)
{
	
	rect = SDL_Rect{x, y, width, height}; // rect.w, rect.h


	SDL_RenderCopy(Sprite::renderer, Sprite::texture, NULL, &rect);
	
	
}