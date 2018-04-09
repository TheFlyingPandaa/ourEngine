#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator()
{
}

RandomNumberGenerator::~RandomNumberGenerator()
{
}

int RandomNumberGenerator::generateRandomNumber(int lowerBound, int upperBound)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	int lower_bound = lowerBound;
	int upper_bound = upperBound;
	std::uniform_int_distribution<int> unif(lower_bound, upper_bound);
	std::default_random_engine randomEngine(seed);
	int a_random_int = unif(randomEngine);

	return a_random_int;
}