#include "sprite.h"
#include <iostream>

Sprite::Sprite(SDL_Renderer* renderer, SDL_Texture* texture)
{
	if (renderer != NULL && texture != NULL) {
		Sprite::renderer = renderer;
		Sprite::texture = texture;
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
	rect = SDL_Rect{ x, y, 32, 32 };
	SDL_RenderCopy(Sprite::renderer, Sprite::texture, NULL, &rect);
}