#pragma once
#include "ChessBoard.h"
#include <assert.h>
ChessBoard::ChessBoard()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	ChessBoard::ChessBoard()
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   ChessBoard constructor
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
}

ChessBoard::~ChessBoard()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	ChessBoard::~ChessBoard()
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   ChessBoard deconstructor
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	for (auto row : map)
	{
		for (auto col : row)
		{
			col->~Tile();
		}
	}

	//clear the tiles
	map.clear();
}

void ChessBoard::LoadBoard(std::string fileName)
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void ChessBoard::LoadBoard(std::string fileName)
	//
	// Method parameters	:	fileName - the file name to be loaded.
	//
	// Method return		:	void
	//
	// Synopsis				:   Load a Board from a given file
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	for (auto row : map)
	{
		for (auto col : row)
		{
			col->~Tile();
		}
	}
	map.clear();
	//open file
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{
		//read in # of bricks
		myfile >> mazWidth;
		myfile >> mazHeight;

		//init cols
		for (int y = 0; y < mazHeight; y++)
		{
			//init rows
			std::vector<Tile*> newRow;
			for (int x = 0; x < mazWidth; x++)
			{
				//make a brick
				Tile *B = new Tile();
				B->typeID = TileEnum::WHITE;
				B->x = x;
				B->y = y;
				newRow.push_back(B);
			}
			map.push_back(newRow);

			/*tileList[y].reserve(mazWidth);*/
		}

		int type = 0;
		int posX = -1;
		int posY = 0;
		//std::string line;
		while (!myfile.eof())
		{
			posX++;

			if (posX == mazWidth)
				posY++;

			posX = posX % mazWidth;

			if (posY == mazHeight)
				break;
			myfile >> type;
			map[posY][posX]->typeID = (TileEnum)type;
		}
		//while (!myfile.eof())
		//{
		//	//make a tile
		//	myfile >> t;

		//	//width
		//	myfile >> posX;

		//	//height
		//	myfile >> posY;


		//	Board[posY][posX]->typeID = (TileEnum)t;

		//	//if rover
		//	if ((TileEnum)t == TileEnum::ROVER)
		//	{
		//		//make rover
		//		rover.x = posX;
		//		rover.y = posY;
		//	}

		//	//if exit
		//	if ((TileEnum)t == TileEnum::END)
		//	{
		//		//make exit
		//		exit.x = posX;
		//		exit.y = posY;
		//	}
		//}
		//close when done
		myfile.close();
	}
}

void ChessBoard::SaveBoard(std::string fileName)
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void ChessBoard::SaveBoard(std::string fileName)
	//
	// Method parameters	:	fileName - the file name to be saved.
	//
	// Method return		:	void
	//
	// Synopsis				:   Save a Board from a given file
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//open file
	std::ofstream myfile;
	myfile.open(fileName, std::ios::trunc);

	if (myfile.is_open())
	{
		//write out # of bricks
		myfile << map.size() << std::endl;
		myfile << map[0].size() << std::endl;
		//write out each brick
		for (int y = 0; y < map.size(); y++)
		{
			for (int x = 0; x < map[0].size(); x++)
			{
				Tile *saveTile = map[y][x];
				myfile << (int)saveTile->typeID << " " << x << " " << y << std::endl;

			}
		}

		myfile.close();
	}
}

