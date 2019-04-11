#include "DieRoller.h"

//std::mt19937 is the Mersenne Twister with a popular choice of parameters
std::mt19937 rng; //keep one global instance of your random device (per thread)

void Seed()
{
	int seedVal;
	std::random_device rd; //not as good/fast as std::mt19937, but a good unpredictable sequence starter.
	seedVal = rd();
	rng.seed(seedVal); //seed the RNG: do this ONLY ONCE at the start of your program
}

int RollDN(int sides)
{
	std::uniform_int_distribution<int> int_dist(0, sides); // range [0,sides]

	return int_dist(rng);
}

float RollDFloat(int sides)
{
	std::uniform_real_distribution<float> float_dist(0, sides); // range [0,sides]
	
	return float_dist(rng);
}

int RollHighDLow(int low, int high)
{
	std::uniform_int_distribution<> dis(low, high);
	return dis(rng);
}