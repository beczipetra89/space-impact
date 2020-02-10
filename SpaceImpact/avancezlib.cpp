#include "avancezlib.h"
#include "SDL_image.h"
#include <iostream>

using namespace std;

void AvancezLib::destroy()
{
	SDL_DestroyWindow(window);

	SDL_Quit();
	TTF_Quit();
}

void AvancezLib::quit()
{
	AvancezLib::destroy();
}

bool AvancezLib::init(int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	window = SDL_CreateWindow("Space Impact", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		cout << "There was error initializing the window" << endl << SDL_GetError() << endl;
		return false;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		std::cout << "Error creating renderer" << std::endl;
		return false;
	}

	return true;
}

void AvancezLib::processInput()
{
}

void AvancezLib::swapBuffers()
{
	SDL_RenderPresent(renderer);
}

void AvancezLib::clearWindow()
{
	SDL_RenderClear(renderer);
}

Sprite* AvancezLib::createSprite(const char* name)
{
	SDL_Surface* surface = IMG_Load(name);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	Sprite* sprite = new Sprite(renderer, texture);

	return sprite;
}


void AvancezLib::drawText(int x, int y, const char* msg)
{
	// Create text texture with font, following tutorial http://gigi.nullneuron.net/gigilabs/displaying-text-in-sdl2-with-sdl_ttf/
	
	TTF_Font* font = TTF_OpenFont("data/space_invaders.ttf", 12);
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface* fontSurface = TTF_RenderText_Solid(font, msg, color);
	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);

	// Get text texture dimensions
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(fontTexture, NULL, NULL, &texW, &texH);
	// Set position also
	SDL_Rect dstRect = { x, y, texW, texH };
	SDL_RenderCopy(renderer, fontTexture, NULL, &dstRect);

	TTF_CloseFont(font);
	SDL_FreeSurface(fontSurface);
	SDL_DestroyTexture(fontTexture);
}

float AvancezLib::getElapsedTime()
{
	return SDL_GetTicks()/1000.0f;  //returns how many seconds it has been since we first initialized SDL
}

void AvancezLib::getKeyStatus(KeyStatus& keys)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:		
			keys.esc = true;
			break;
			
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
				keys.esc = true;
				break;

			case SDLK_UP:
				keys.up = true;
				break;
			
			case SDLK_DOWN:
				keys.down = true;
				break;

			case SDLK_LEFT:
				keys.left = true;
				break;

			case SDLK_RIGHT:
				keys.right = true;
				break;

			case SDLK_SPACE:
				keys.fire = true;
				break;

			case SDLK_p:
				keys.cheat = true;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				keys.left = false;
				break;

			case SDLK_UP:
				keys.up = false;
				break;

			case SDLK_DOWN:
				keys.down = false;
				break;

			case SDLK_RIGHT:
				keys.right = false;
				break;

			case SDLK_SPACE:
				keys.fire = false;
				break;

			case SDLK_p:
				keys.cheat = false;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}

void AvancezLib::SetBackgroundColor(RGBColor& color)
{
	// Select the color for drawing.
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
}