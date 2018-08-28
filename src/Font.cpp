#include "Font.h"

Font::Font(SDL_Renderer * renderer, const char * path, int size):
	initialized(false),
	font(nullptr),
	renderer(nullptr)
{
	this->renderer = renderer;
    this->font = TTF_OpenFont(path, size);

	if (font != nullptr) {
		initialized = true;
	}
}

Font::~Font()
{
}

void Font::Say(const char * text, SDL_Rect dest, SDL_Color color)
{
	if (initialized) {
		SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_RenderCopy(renderer, texture, NULL, &dest);

		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
}

bool Font::IsInitilized()
{
	return initialized;
}

