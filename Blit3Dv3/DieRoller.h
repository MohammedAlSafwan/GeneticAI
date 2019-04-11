#pragma once
#include <random> //need for random number generation

extern std::mt19937 rng; //keep one global instance of your random device (per thread)

void Seed();

int RollDN(int sides);

float RollDFloat(int sides);

int RollHighDLow(int low, int high);