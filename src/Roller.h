#ifndef ROLLER_H
#define ROLLER_H

#include <memory>
#include <vector>
#include "RollerTile.h"


class Roller
{
public:
	Roller();
	~Roller();

	void AddTile(std::shared_ptr<RollerTile> tile);
	void StartSpin();
	void StopSpin();
	void Render();
	void Update(float dt);
	int GetMiddleLineID();

private:
	std::vector<std::shared_ptr<RollerTile>> tiles;
	bool do_spinning;
	int align_tiles_px;
	int middle_line_tile_id;

	void move_tiles(int dy);
};

#endif //ROLLER_H