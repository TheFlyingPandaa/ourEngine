#include "FroggeDSA.h"



DiamondSqaure::DiamondSqaure()
{
}


DiamondSqaure::~DiamondSqaure()
{
}

vector<float> DiamondSqaure::createDiamondSquare(int mapSize, int initStepSize, float noise)
{
	this->mapSize = mapSize;
	this->diamondSquare.resize(this->mapSize * this->mapSize);
	int stepSize = initStepSize;
	float noiseScale = noise;

	for (int z = 0; z < this->mapSize; z += stepSize)
		for (int x = 0; x < this->mapSize; x += stepSize)
			this->setValue(x, z, this->fRand());

	while (stepSize > 1)
	{
		this->diamondSquareAlgorithm(stepSize, noiseScale);

		stepSize = stepSize / 2;
		noiseScale = noiseScale / 2;
	}
	//add smoothing to the values and repeat X given times for further smoothing, 1 time is enough for now, but play with it for lulz
	smoothValues((int)pow(2, 2) + 1);
	return this->diamondSquare;
}

float DiamondSqaure::fRand()
{
	int min = -15;
	int max = 15;
	float randomNumber = (float)rand() / RAND_MAX;
	return (min + randomNumber * (max - (min)));
}

float DiamondSqaure::getValue(int x, int z)
{
	if (x < this->mapSize && x >= 0)
	{
		if (z < this->mapSize && z >= 0)
			return this->diamondSquare[x + (z * this->mapSize)];
		else
			return 0.0f;
	}
	else
		return 0.0f;
}

void DiamondSqaure::setValue(int x, int z, float value)
{
	this->diamondSquare[x + (z * this->mapSize)] = value;
}

void DiamondSqaure::diamondStep(int x, int z, int stepSize, float noise)
{
	int halfStep = stepSize / 2;

	float corner1 = this->getValue(x - halfStep, z - halfStep);
	float corner2 = this->getValue(x + halfStep, z - halfStep);
	float corner3 = this->getValue(x - halfStep, z + halfStep);
	float corner4 = this->getValue(x + halfStep, z + halfStep);

	float squareValue = ((corner1 + corner2 + corner3 + corner4) / 4) + noise;

	this->setValue(x, z, squareValue);
}

void DiamondSqaure::squareStep(int x, int z, int stepSize, float noise)
{
	if (x < this->mapSize && z < this->mapSize)
	{
		int halfStep = stepSize / 2;

		float corner1 = this->getValue(x - halfStep, z);
		float corner2 = this->getValue(x + halfStep, z);
		float corner3 = this->getValue(x, z - halfStep);
		float corner4 = this->getValue(x, z + halfStep);

		float diamondValue = ((corner1 + corner2 + corner3 + corner4) / 4) + noise;

		this->setValue(x, z, diamondValue);
	}
}

void DiamondSqaure::diamondSquareAlgorithm(int stepSize, float noise)
{
	int halfStep = stepSize / 2;

	for (int z = halfStep; z < this->mapSize; z += stepSize)
		for (int x = halfStep; x < this->mapSize; x += stepSize)
		{
			this->diamondStep(x, z, stepSize, this->fRand() * noise);
		}

	for (int z = 0; z < this->mapSize; z += stepSize)
		for (int x = 0; x < this->mapSize; x += stepSize)
		{
			this->squareStep(x + halfStep, z, stepSize, this->fRand() * noise);
			this->squareStep(x, z + halfStep, stepSize, this->fRand() * noise);
		}
}

void DiamondSqaure::smoothValues(int filtersize)
{
	int count = 0;
	float total = 0;

	//loop through the values
	for (int x = 0; x < mapSize - 1; x++)
	{
		for (int z = 0; z < mapSize - 1; z++)
		{
			count = 0;
			total = 0.f;
			for (int x0 = x - filtersize; x0 <= x + filtersize; x0++)
			{
				if (x0 < 0 || x0 > mapSize - 1) //check boundaries
					continue;
				for (int z0 = z - filtersize; z0 < z + filtersize; z0++)
				{
					if (z0 < 0 || z0 > mapSize - 1)
						continue;

					//add the contribution from the filter to the total for this point
					total += diamondSquare[z0 + (x0 * mapSize)];
					count++;
				} //z0
			} //x0
			if (count != 0 && total != 0)
				diamondSquare[z + (mapSize * x)] = total / (float)count;
		} //z
	} //x
}