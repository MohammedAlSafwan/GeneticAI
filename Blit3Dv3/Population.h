#pragma once
#include "DNA.h"
class Population
{
public:

	float ultimateWorldBest = 0.f;
	DNA ultimateDNA;
	float mutationRate;				// Mutation rate
	std::vector<DNA> population;    // Array to hold the current population
	std::vector<DNA> matingPool;    // ArrayList which we will use for our "mating pool"
	//std::string target;             // Target phrase
	int generations;				// Number of generations
	bool finished;					// Are we finished evolving?
	int perfectScore;

	Population();
	Population(int chessBoardSize, int numOfQueens, float mutationRate, int numOfPopulation);
	~Population();
	void calcFitness();
	void naturalSelection();
	void generate();

	DNA getBest();
	int getGenerations();
	float getAverageFitness();
	std::vector<DNA> allGenese();

	double round(double d);
	float map(float input, float input_start, float input_end, float output_start, float output_end);

	void update();
	void draw();
};

