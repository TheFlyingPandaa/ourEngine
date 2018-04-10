#pragma once
#include <random>
#include <chrono>

class RandomNumberGenerator
{
private:
	
public:
	RandomNumberGenerator();
	~RandomNumberGenerator();

	int generateRandomNumber(int lowerBound, int upperBound);
};