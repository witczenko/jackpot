#include <algorithm>
#include <iterator>
#include <map>
#include "Roller.h"
#include "Settings.h"

Roller::Roller():
	do_spinning(false),
	align_tiles_px(0),
	middle_line_tile_id(NONE_ID)
{
}


Roller::~Roller()
{
}

void Roller::AddTile(std::shared_ptr<RollerTile> tile)
{
	tiles.push_back(tile);
}

void Roller::StartSpin()
{
	do_spinning = true;
}

void Roller::StopSpin()
{
	do_spinning = false;
}

void Roller::Render()
{
	for (auto tile : tiles) {
		tile->Render();
	}
}

void Roller::Update(float dt)
{
	//dt = 0.016;
	const int pixel_per_sec = 2000;
	int dy = static_cast<int>(pixel_per_sec * dt);

	if (do_spinning) {
		move_tiles(dy);
	}
	else if (align_tiles_px > 0){
		move_tiles(align_tiles_px);
		align_tiles_px = 0;
	}

}

int Roller::GetMiddleLineID()
{
	return middle_line_tile_id;
}

void Roller::move_tiles(int dy)
{
	std::vector<std::pair<int, int>> dist_table; 
	
	for (auto tile : tiles) {
		SDL_Rect tile_dest = tile->GetDestination();

		tile_dest.y = tile_dest.y > static_cast<int>WINDOW_HEIGHT ?
			tile_dest.y - (static_cast<int>(tiles.size())*tile_dest.h) + dy:
			tile_dest.y + dy;
		tile->SetDestination(tile_dest);

		if (int dist = (WINDOW_HEIGHT / 2) - (tile_dest.y + tile_dest.h/2);  dist >= 0) {
			dist_table.push_back(std::pair<int, int>(tile->GetId(), dist));
		}

	}

	auto min_element = std::min_element(std::begin(dist_table), std::end(dist_table),
		[](decltype(dist_table)::value_type& l, decltype(dist_table)::value_type& r) -> bool { return l.second < r.second; });

	if (min_element != std::end(dist_table)) {
		align_tiles_px = min_element->second;
		middle_line_tile_id = min_element->first;
	}

	dist_table.clear();
}
