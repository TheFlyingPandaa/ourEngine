#pragma once
#include <vector>
#include "../../ourEngine/interface/shape/Rectangle.h"

#define NUMBER_OF_TREES 500

class HeightMap
{
private:
	class DiamondSqaure
	{
	public:
		DiamondSqaure();
		~DiamondSqaure();

		std::vector<float> createDiamondSquare(int mapSize, int stepSize, float noise);

	private:
		int mapSize;
		std::vector<float> diamondSquare;

		float fRand();
		float getValue(int x, int z);
		void setValue(int x, int z, float value);
		void diamondStep(int x, int z, int stepSize, float noise);
		void squareStep(int x, int z, int stepSize, float noise);
		void diamondSquareAlgorithm(int stepSize, float noise);
		void smoothValues(int filtersize); //filtersize must be 2^n + 1
		void smoothValues(int filterSize, int x, int z);
	};
private:

	int m_mapSize;
	float m_offset;

	std::vector<float> m_heightValues;
	std::vector<VERTEX> m_vertices;
	std::vector<unsigned int> m_indices;

	RectangleShape rs;
	RectangleShape water;
	std::vector<RectangleShape> m_trees;
	

public:
	HeightMap(int mapWidth, int stepSize, float noise);
	HeightMap();
	void Draw();
	virtual ~HeightMap();
private:
	void _GenerateHeightValues(int stepSize, float noise);
	void _BuildHeightMap();
};