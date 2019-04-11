#pragma once
#include <queue>
#include<math.h>
#define TILE_WIDTH 76
#define TILE_HEIGHT 76

//traversable
enum class TileEnum {
	BLACK,
	WHITE,
	SIZE
};

class Tile
{

public:
	int x = -1;
	int y = -1;
	TileEnum typeID = TileEnum::WHITE; //envirment cost

	Tile();
	~Tile();



};

