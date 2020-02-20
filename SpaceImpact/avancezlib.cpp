#include "avancezlib.h"
#include "SDL_image.h"
#include <iostream>

using namespace std;

void AvancezLib::destroy()
{
	SDL_DestroyWindow(window);

	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
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
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error creating renderer");
		return false;
	}

	// Initialize audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		printf("Mixer Initialization Error: %s\n", Mix_GetError());
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

Sprite* AvancezLib::createSprite(const char* name, int w, int h)
{
	SDL_Surface* surface = IMG_Load(name);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	Sprite* sprite = new Sprite(renderer, texture, w, h);
	return sprite;
}


void AvancezLib::drawText(int x, int y, const char* msg)
{
	// Create text texture with font, following tutorial http://gigi.nullneuron.net/gigilabs/displaying-text-in-sdl2-with-sdl_ttf/
	
	TTF_Font* font = TTF_OpenFont("data/space_invaders.ttf", 12);
	SDL_Color color = { 0, 255, 213 };
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
				PlaySFX("data/audio/player_shoot.wav", 0, -1);
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


// AudioManager will be using these 
Mix_Music* AvancezLib::GetMusic(std::string filename) {
	
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("data/audio/" + filename);

	//check if we already loaded the file or not
	if (mMusic[fullPath] == nullptr) {
		mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());

		if (mMusic[fullPath] == NULL)
			printf("Music Loading Error: File -%s Error -%s", filename.c_str(), Mix_GetError());		
	}

	return mMusic[fullPath];
	
}

Mix_Chunk* AvancezLib::GetSFX(std::string filename) {
	//std::string filepath = SDL_GetBasePath();
	//fullPath.append("data/audio/" + filename);
	//SDL_Log("Loading SFX sound file from %s", filepath.c_str());

	if(mSFX[filename] == nullptr){
		mSFX[filename] = Mix_LoadWAV(filename.c_str());

		if (mSFX[filename] == NULL)
			printf("SFX Loading Error: File -%s Error-%s\n",filename.c_str(), Mix_GetError());
	}
	return mSFX[filename];
}

void AvancezLib::PlayMusic(std::string filename, int loops)
{
	Mix_PlayMusic(GetMusic(filename), loops);
}

void AvancezLib::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
		Mix_PauseMusic();
}

void AvancezLib::ResumeMusic()
{
	if (Mix_PausedMusic() != 0)
		Mix_ResumeMusic();
}

void AvancezLib::PlaySFX(std::string filename, int loops, int channel)
{
	Mix_PlayChannel(channel, GetSFX(filename), loops);
}