#include "RollerTile.h"



RollerTile::RollerTile(int id, SDL_Rect destination, SDL_Renderer * renderer, SDL_Texture * texture):
	renderer(nullptr),
	texture(nullptr),
	destination({0,0,0,0}),
	id(NONE_ID)
{
	this->destination = destination;
	this->renderer = renderer;
	this->texture = texture;
	this->id = id;
}

RollerTile::~RollerTile()
{
}

void RollerTile::SetDestination(SDL_Rect dest)
{
	this->destination = dest;
}

SDL_Rect RollerTile::GetDestination()
{
	return destination;
}

int RollerTile::GetId()
{
	return id;
}

void RollerTile::Render()
{
	SDL_RenderCopy(renderer, texture, nullptr, &destination);
}
