#ifndef ROLLERTILE_H 
#define ROLLERTILE_H 

#include <SDL.h>

#define NONE_ID		(-1)

class RollerTile
{
public:
	RollerTile(int id, SDL_Rect destination, SDL_Renderer *renderer, SDL_Texture *texture);
	~RollerTile();

	void SetDestination(SDL_Rect dest);
	SDL_Rect GetDestination();
	int GetId();
	void Render();
private:
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	SDL_Rect destination;
	int id;
};

#endif //ROLLERTILE_H 
