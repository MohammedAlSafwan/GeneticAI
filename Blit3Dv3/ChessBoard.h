#pragma once
#include "Tile.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class ChessBoard
{

public:

	int mazWidth = 0;							//Map width
	int mazHeight = 0;							//Map height

	std::vector<std::vector<Tile *>> map;		//Map tiles

	ChessBoard();
	~ChessBoard();
	void LoadBoard(std::string fileName);
	void SaveBoard(std::string fileName);

};
