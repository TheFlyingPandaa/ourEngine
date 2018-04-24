#pragma once
#include <random>
#include <chrono>

class RandomNumberGenerator
{
private:
	
public:
	RandomNumberGenerator();
	~RandomNumberGenerator();

	int GenerateRandomNumber(int lowerBound, int upperBound);
};