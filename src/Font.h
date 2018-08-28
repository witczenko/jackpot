#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <SDL_ttf.h>

class Font
{
public:
	Font(SDL_Renderer *renderer, const char* path, int size);
	~Font();

	void Say(const char* text, SDL_Rect dest, SDL_Color color);
	bool IsInitilized();
private:
	bool initialized;
	SDL_Renderer *renderer;
	TTF_Font *font;
};

#endif //FONT_H