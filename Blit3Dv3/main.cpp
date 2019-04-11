/*
	Simple example of an Arkinoid level editor
*/

#include "Blit3D.h"
#include "Camera.h"
#include "DieRoller.h"
#include "Population.h"
#include <chrono>
Blit3D *blit3D = NULL;

//memory leak detection
#define CRTDBG_MAP_ALLOC

#include <crtdbg.h>

#include "ChessBoard.h"

#define MAX_DRAW_X	100
#define MAX_DRAW_Y	100

#define TILE_WIDTH 76
#define TILE_HEIGHT 76

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define PAN_SCREEN_BY 1.0f
#define PAN_TILES_BY 1

Camera2D *camera; //pans the view
ChessBoard *map; //bricks for a level

void initSprits();
void initFont();
void initChessBoard();
void initPopulation();
void initCamera();
void solveProblem();
void drawFont();
void drawMap();

//GLOBAL DATA
std::vector<Sprite *> spriteList;
TileEnum editBrick = TileEnum::WHITE; //brick we are currently editing with

int offX = 0;
int offY = 0;

float timeStep = 5.f / 60.f; //one 60th of a second
float elapsedTime = 0; //used for calculating time passed

int showTile = 0;

AngelcodeFont *font = NULL;//caviar57font
std::string infoTxt = "";

glm::vec2 cursor;

int numOfQueen;
int popmax;
float mutationRate;
Population* population = NULL;

void Init()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void Init() 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   initilize the sprits, the font, the map, and camera.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	initSprits();
	initFont();
	initChessBoard();
	initPopulation();
	initCamera();
}

void initPopulation()
{
	Seed();
	numOfQueen = 8;
	popmax = 150;
	mutationRate = 0.01;

	// Create a populationation with a target phrase, mutation rate, and populationation max
	population = new Population(map->mazWidth, numOfQueen, mutationRate, popmax);
	printf("total generations:     %d\n", population->getGenerations());
	std::cout << "average fitness:       " << population->getAverageFitness() << std::endl;
	printf("Total populaton:	   %d\n", population->population.size());
	printf("mutation rate:         %f %\n", population->mutationRate * 100);
}

void initSprits()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	initSprits() 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   initilize the sprits.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	spriteList.push_back(blit3D->MakeSprite(0, 0, TILE_WIDTH, TILE_HEIGHT, "Media\\BLACK.png"));
	spriteList.push_back(blit3D->MakeSprite(0, 0, TILE_WIDTH, TILE_HEIGHT, "Media\\WHITE.png"));
	spriteList.push_back(blit3D->MakeSprite(0, 0, 64, 64, "Media\\QUEEN.png"));

}

void initFont()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void initFont() 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   initilize the font.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//load an Angelcode binary32 font file
	font = blit3D->MakeAngelcodeFontFromBinary32("Media\\CaviarDreams57.bin");
}

void initChessBoard()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void initMaze() 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   initilize the map.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	map = new ChessBoard();
	map->LoadBoard("map.txt");
}

void initCamera()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void initCamera() 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   initilize the camera.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//make a camera
	camera = new Camera2D();
	camera->minX = (blit3D->screenWidth / 2) - (TILE_WIDTH / 2);
	camera->minY = (blit3D->screenHeight / 2) - (TILE_HEIGHT / 2);
	camera->maxX = map->mazWidth * TILE_WIDTH + (blit3D->screenWidth / 2);
	camera->maxY = map->mazHeight * TILE_HEIGHT + (blit3D->screenHeight / 2);


}

void solveProblem()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void solveMap() 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Try to solve the puzzle and print output to the console and the screen.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	printf("Start Solving!\n");
	infoTxt = "Start Solving...\n";

	//Start counting the time it takes to solve the puzzle
	auto start = std::chrono::high_resolution_clock::now();

	//Finish counting
	auto finish = std::chrono::high_resolution_clock::now();
	//Print the time needed to find the game
	printf("Path: %s\n", infoTxt.c_str());
	printf("Time to finish ");
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() << " ns\n";
	//printf("%d steps\n", map->solutionPath.size());
	//printf("%f steps a day", (map->counter / 10000.f));
}

void DeInit(void)
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void DeInit(void) 
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Try to solve the puzzle and print output to the console and the screen.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//Delete camera
	if (camera != NULL) delete camera;

	//Delete map
	if (map != NULL) map->~ChessBoard();

	// DO NOT delete the font!
	//If we need to delete a font at runtime, call DeleteFont().

	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}

void Update(double seconds)
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void Update(double seconds)
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Update the camera and the map. The rover if there is a solution.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	elapsedTime += (float)seconds;

	//if passed or equal to the time step
	if (elapsedTime >= timeStep)
	{
		//make sure that the elapsed time is less than time step
		while (elapsedTime >= timeStep)
		{
			elapsedTime -= timeStep;
		}

	}
	if (!population->finished)
		population->update();

	//Update the camera
	camera->Update((float)seconds);
}

void Draw(void)
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void Draw(void)
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Draw the camera, map, fond, and square under cursor.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//clear screen before drawing
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//clear colour: r,g,b,a 	
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//MUST draw camera first!
	camera->Draw();

	//Draw the map.
	drawMap();
	/*	11011111
		11111011
		11111110
		01111111
		11110111
		11111101
		10111111
		11101111*/

	if (population->finished)
	{
		printf("====================================\n");
		printf("Best solution\n");
		DNA theBestGene = population->getBest();
		for (int i = 0; i < theBestGene.genes.size(); i++) {
			for (int j = 0; j < theBestGene.genes.size(); j++)
				(theBestGene.genes[i] == j) ?
				printf("0\t") :
				printf("1\t");
			printf("\n");
		}
		std::cout << "DNAFitness = " << theBestGene.DNAfitness << std::endl;
		std::cout << "Generations = " << population->generations << std::endl;
	}
	else
	{
		population->draw();
	}
	//undraw the camera so the font doesn't move out of the screen
	camera->UnDraw();

	//Draw font
	drawFont();

}

void drawMap()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void drawMap()
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Draw the map.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

				//draw a the tiles
	for (int x = 0; x < map->mazWidth; x++)
		for (int y = 0; y < map->mazHeight; y++)
			spriteList[(int)(map->map[y][x]->typeID)]->Blit(x * TILE_WIDTH, y * TILE_HEIGHT);


}

void drawFont()
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void drawFont()
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Draw font.
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	font->BlitText(50.f, 1040.f, infoTxt);
}

void DoInput(int key, int scancode, int action, int mods)
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	void DoInput(int key, int scancode, int action, int mods)
	//
	// Method parameters	:	key      - key that was pressed
	//							scancode - the scanned code out of the device
	//							action   - the action on the key
	//							mods     - the moder 
	//
	// Method return		:	void
	//
	// Synopsis				:   Do the user input from the keyboard
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		//save!
		map->SaveBoard("map.txt");
	}


	if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		solveProblem();
	}


	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Pan(1, 0);
		(offX + PAN_TILES_BY >= map->mazWidth) ?
			offX = 0 :
			offX += PAN_TILES_BY;
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Pan(-1, 0);
		(offX - PAN_TILES_BY < 0) ?
			offX = 0 :
			offX -= PAN_TILES_BY;
	}

	if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE)
	{
		camera->Pan(0, 0);
	}

	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->Pan(0, 1);
		(offY + PAN_TILES_BY >= map->mazHeight) ?
			offY = 0 :
			offY += PAN_TILES_BY;
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS))
	{
		camera->Pan(0, -1);
		(offY - PAN_TILES_BY < 0) ?
			offY = 0 :
			offY -= PAN_TILES_BY;
	}

	if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_UP) && action == GLFW_RELEASE)
	{
		camera->Pan(0, 0);
	}
}

int main(int argc, char *argv[])
{
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Method				:	int main(int argc, char *argv[])
	//
	// Method parameters	:	
	//
	// Method return		:	void
	//
	// Synopsis				:   Build and run Blit3D
	//							
	//
	// Modifications		:
	//								Date			 Developer				   Notes
	//								----			 ---------			 	   -----
	//							Oct 5, 2018		  Mohammed Al-Safwan		Initial setup
	//
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, SCREEN_WIDTH, SCREEN_HEIGHT);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::MULTITHREADED);
	if (blit3D) delete blit3D;
}