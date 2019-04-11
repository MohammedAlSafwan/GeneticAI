#pragma once
#include <stdlib.h>
#include <vector>

class DNA //Chromosome
{
public:
	int chessBoardSize = 0;
	std::vector<int> genes;
	float DNAfitness;

	DNA();
	DNA(int chessBoardSize, int numOfQueens);
	~DNA();

	float fitness();
	DNA crossover(DNA partner);
	void mutate(float mutationRate);

	int clashes();
};

