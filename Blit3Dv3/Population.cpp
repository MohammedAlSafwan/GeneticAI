#include "Population.h"
#include <math.h>
#include "DieRoller.h"
#include <iostream>

Population::Population()
{
}

Population::Population(int chessBoardSize, int numOfQueens, float mutationRate, int numOfPopulation)
{
	//target = p;
	this->mutationRate = mutationRate;
	this->population.resize(numOfPopulation);
	for (int i = 0; i < this->population.size(); i++) {
		this->population[i] = DNA(chessBoardSize, numOfQueens);
	}
	this->calcFitness();
	this->matingPool.resize(numOfPopulation);
	this->finished = false;
	this->generations = 0;

	this->perfectScore = 1;
}

Population::~Population()
{
}

// Fill our fitness array with a value for every member of the population
void Population::calcFitness() {
	for (int i = 0; i < population.size(); i++) {
		population[i].fitness();
	}
}

// Generate a mating pool
void Population::naturalSelection() {
	// Clear the ArrayList
	matingPool.clear();

	float maxFitness = 0;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].fitness() > maxFitness) {
			maxFitness = population[i].fitness();
		}
	}

	// Based on fitness, each member will get added to the mating pool a certain number of times
	// a higher fitness = more entries to mating pool = more likely to be picked as a parent
	// a lower fitness = fewer entries to mating pool = less likely to be picked as a parent
	for (int i = 0; i < population.size(); i++) {

		float fitness = map(population[i].fitness(), 0, maxFitness, 0, 1);
		int n = int(fitness * 100);  // Arbitrary multiplier, we can also use monte carlo method
		for (int j = 0; j < n; j++) {              // and pick two random numbers
			matingPool.push_back(population[i]);
		}
	}
}

// Create a new generation
void Population::generate() {
	// Refill the population with children from the mating pool
	for (int i = 0; i < population.size(); i++) {
		int a = RollDN(matingPool.size() - 1);
		int b = RollDN(matingPool.size() - 1);
		DNA partnerA = matingPool[a];
		DNA partnerB = matingPool[b];
		DNA child = partnerA.crossover(partnerB);
		child.mutate(mutationRate);
		population[i] = child;
	}
	generations++;
}

// Compute the current "most fit" member of the population
DNA Population::getBest() {
	float worldrecord = 0.0;
	int index = 0;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].fitness() > worldrecord) {
			index = i;
			worldrecord = population[i].fitness();
			if (worldrecord == perfectScore)
			{
				this->finished = true;
				ultimateDNA = population[i];
			}
			if (worldrecord > ultimateWorldBest)
			{
				ultimateWorldBest = worldrecord;
				system("pause");
			}
		}
	}

	return population[index];
}

int Population::getGenerations() {
	return generations;
}

// Compute average fitness for the population
float Population::getAverageFitness() {
	float total = 0;
	for (int i = 0; i < population.size(); i++) {
		total += population[i].fitness();
	}
	return total / (population.size());
}

std::vector<DNA> Population::allGenese() {
	return population;
}

void Population::update()
{
	if (!finished) {
		// Generate mating pool
		naturalSelection();
		//Create next generation
		generate();
		// Calculate fitness
		calcFitness();
	}
}

void Population::draw()
{
	printf("====================================\n");
	DNA theBestGene = getBest();
	for (int i = 0; i < theBestGene.genes.size(); i++) {
		for (int j = 0; j < theBestGene.genes.size(); j++)
			(theBestGene.genes[i] == j) ?
			printf("0\t") :
			printf("1\t");
		printf("\n");
	}
	std::cout << "DNAFitness = " << theBestGene.DNAfitness << std::endl;
}

double Population::round(double d)
{
	return floor(d + 0.5);
}

float Population::map(float input, float input_start, float input_end, float output_start, float output_end)
{
	double slope = 1.0 * (output_end - output_start) / (input_end - input_start);
	return output_start + round(slope * (input - input_start));
}

