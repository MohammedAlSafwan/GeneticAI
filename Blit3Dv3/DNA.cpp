#include "DNA.h"
#include "DieRoller.h"
#include <iostream> 
#include <unordered_set>

DNA::DNA()
{
}

DNA::DNA(int chessBoardSize, int numOfQueens)
{
	DNAfitness = 0.f;
	this->chessBoardSize = chessBoardSize;
	genes.resize(numOfQueens);
	for (int i = 0; i < genes.size(); i++) {
		// Pick from range of chars
		genes[i] = RollHighDLow(0, numOfQueens - 1);
	}
}

DNA::~DNA()
{
}

float DNA::fitness()
{
	int clashes = 0;
	//calculate row and column clashes
	//just subtract the unique length of array from total length of array
	//[1,1,1,2,2,2] - [1,2] => 4 clashes
	int row_col_clashes = genes.size() - this->clashes();
	clashes += row_col_clashes;

	//calculate diagonal clashes
	for (int i = 0; i < genes.size(); i++)
	{
		for (int j = 0; j < genes.size(); j++)
		{
			if (i != j) {
				int dx = abs(i - j);
				int dy = abs(genes[i] - genes[j]);
				if (dx == dy)
					clashes += 1;
			}
		}
	}
	DNAfitness = (28.f - clashes) / 28.f;
	return DNAfitness;
}

int DNA::clashes()
{
	//int clashesSoFar = 0;

	//// Pick all elements one by one 
	//for (int i = 1; i < genes.size(); i++) {
	//	int j = 0;
	//	for (j = 0; j < i; j++)
	//	{
	//		if (genes[i] == genes[j])
	//			break;
	//	}
	//	// If not printed earlier, then print it 
	//	if (i == j)
	//		clashesSoFar++;
	//}
	std::vector<int> vect2;
	for (int i = 0; i < genes.size(); i++)
		vect2.push_back(genes[i]);

	std::sort(vect2.begin(), vect2.end());
	return std::unique(vect2.begin(), vect2.end()) - vect2.begin();
	//return clashesSoFar;
}

// Crossover
DNA DNA::crossover(DNA partner) {
	// A new child
	DNA child = DNA(partner.chessBoardSize, partner.genes.size());

	// Pick a midpoint
	int midpoint = RollDN(partner.genes.size() - 1);

	// Half from one, half from the other
	for (int i = 0; i < partner.genes.size(); i++) {
		(i > midpoint) ?
			child.genes[i] = genes[i] :
			child.genes[i] = partner.genes[i];
	}
	return child;
}

// Based on a mutation probability, picks a new random gene
void DNA::mutate(float mutationRate) {
	int persentMutationRate = (mutationRate * 100);
	for (int i = 0; i < genes.size(); i++) {
		int randVal = RollHighDLow(0,100);
		if (randVal < persentMutationRate) {
			genes[i] = RollHighDLow(0, genes.size() - 1);
		}
	}
}