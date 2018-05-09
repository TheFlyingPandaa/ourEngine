#include "HeightMapGenerator.h"
#include <iostream>
#include <fstream>

float HeightMap::DiamondSqaure::fRand()
{
	int min = -10;
	int max = 5;
	float randomNumber = (float)rand() / RAND_MAX;
	return (min + randomNumber * (max - (min)));
}

float HeightMap::DiamondSqaure::getValue(int x, int z)
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

void HeightMap::DiamondSqaure::setValue(int x, int z, float value)
{
	this->diamondSquare[x + (z * this->mapSize)] = value;
}

void HeightMap::DiamondSqaure::diamondStep(int x, int z, int stepSize, float noise)
{
	int halfStep = stepSize / 2;

	float corner1 = this->getValue(x - halfStep, z - halfStep);
	float corner2 = this->getValue(x + halfStep, z - halfStep);
	float corner3 = this->getValue(x - halfStep, z + halfStep);
	float corner4 = this->getValue(x + halfStep, z + halfStep);

	float squareValue = ((corner1 + corner2 + corner3 + corner4) / 4) + noise;

	this->setValue(x, z, squareValue);
}

void HeightMap::DiamondSqaure::squareStep(int x, int z, int stepSize, float noise)
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

void HeightMap::DiamondSqaure::diamondSquareAlgorithm(int stepSize, float noise)
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

void HeightMap::DiamondSqaure::smoothValues(int filterSize)
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
			for (int x0 = x - filterSize; x0 <= x + filterSize; x0++)
			{
				if (x0 < 0 || x0 > mapSize - 1) //check boundaries
					continue;
				for (int z0 = z - filterSize; z0 < z + filterSize; z0++)
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

HeightMap::DiamondSqaure::DiamondSqaure()
{
}

HeightMap::DiamondSqaure::~DiamondSqaure()
{
}

std::vector<float> HeightMap::DiamondSqaure::createDiamondSquare(int mapSize, int sSize, float noise)
{
	this->mapSize = mapSize;
	this->diamondSquare.resize(this->mapSize * this->mapSize);
	int stepSize = sSize;
	float noiseScale = noise;

	/*for (int z = 0; z < this->mapSize; z += stepSize)
		for (int x = 0; x < this->mapSize; x += stepSize)
			setValue(x, z, 10);*/
	int mountainHeight = 25;
	for (int z = 0; z < this->mapSize; z += stepSize)
		this->setValue(0, z, mountainHeight);
	for (int z = 0; z < this->mapSize; z += stepSize)
		this->setValue(mapSize - 1, z, mountainHeight);

	for (int z = 0; z < this->mapSize; z += stepSize)
		this->setValue(z,0, mountainHeight);
	for (int z = 0; z < this->mapSize; z += stepSize)
		this->setValue(z, mapSize - 1, mountainHeight);

	//this->setValue(mapSize - 1, 0, 30);
	//this->setValue(mapSize - 1, mapSize - 1, 30);
	//this->setValue(0, mapSize - 1, 30);
	while (stepSize > 1)
	{
		this->diamondSquareAlgorithm(stepSize, noiseScale);

		stepSize = stepSize / 2;
		noiseScale = noiseScale / 2;
	}
	//add smoothing to the values and repeat X given times for further smoothing, 1 time is enough for now, but play with it for lulz
	for (int z = (mapSize / 2) - 12; z < (mapSize / 2) + 12; z++)
		for (int x = (mapSize / 2) - 12; x < (mapSize / 2) + 12; x++)
			this->setValue(x, z, 5);
	smoothValues((int)pow(2, 2) + 1);
	return this->diamondSquare;
}

HeightMap::HeightMap(int mapWidth, int stepSize, float noise)
	: m_mapSize(mapWidth)
{
	_GenerateHeightValues(stepSize, noise);
	m_offset = 1.0f;
	_BuildHeightMap();
}

HeightMap::~HeightMap()
{

}

void HeightMap::_GenerateHeightValues(int stepSize, float noise)
{
	DiamondSqaure ds;
	m_heightValues = ds.createDiamondSquare(m_mapSize, stepSize, noise);
}

std::string HeightMap::toString() const
{
	return std::string();
}

void HeightMap::_BuildHeightMap()
{
	VERTEX v;
	using namespace DirectX;

	for (int z = 0; z < m_mapSize; z++)
	{
		for (int x = 0; x < m_mapSize; x++)
		{
			int index = x + (m_mapSize * z);
			XMFLOAT3 vertex = XMFLOAT3((float)x * m_offset, m_heightValues[index], (float)z * m_offset);

			v.x = vertex.x; 
			v.y = vertex.y; 
			v.z = vertex.z;

			m_vertices.push_back(v);
		}
	}


	float texUIndex = 0.f;
	float texVIndex = 0.f;
	//calculate the UV coords, normal, tangent and binormals
	for (int z = 0; z < m_mapSize - 1; z++)
	{
		for (int x = 0; x < m_mapSize - 1; x++) {

			int v1, v2, v3, v4, v5, v6;

			v1 = (z + 1)*m_mapSize + x;
			v2 = (z*m_mapSize + x + 1);
			v3 = z * m_mapSize + x;

			v4 = z * m_mapSize + x + 1;
			v5 = (z + 1)*m_mapSize + x;
			v6 = (z + 1)*m_mapSize + x + 1;

			//set the UV coordinats
			XMFLOAT2 v1Tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);
			m_vertices[v1].u = v1Tex.x; 
			m_vertices[v1].v = v1Tex.y;

			XMFLOAT2 v2Tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);
			m_vertices[v2].u = v2Tex.x; 
			m_vertices[v2].v = v2Tex.y;

			XMFLOAT2 v3Tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);
			m_vertices[v3].u = v3Tex.x; 
			m_vertices[v3].v = v3Tex.y;

			XMFLOAT2 v4Tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);
			m_vertices[v4].u = v4Tex.x;
			m_vertices[v4].v = v4Tex.y;

			XMFLOAT2 v5Tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);
			m_vertices[v5].u = v5Tex.x; 
			m_vertices[v5].v = v5Tex.y;

			XMFLOAT2 v6Tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);
			m_vertices[v6].u = v6Tex.x; 
			m_vertices[v6].v = v6Tex.y;


			XMFLOAT3 vtx1, vtx2, vtx3;
			//XMFLOAT2 uv1, uv2, uv3;
			XMFLOAT3 normal;

			vtx1 = XMFLOAT3(m_vertices[v1].x, m_vertices[v1].y, m_vertices[v1].z);
			vtx2 = XMFLOAT3(m_vertices[v2].x, m_vertices[v2].y, m_vertices[v2].z);
			vtx3 = XMFLOAT3(m_vertices[v3].x, m_vertices[v3].y, m_vertices[v3].z);

			//uv1 = m_vertices[v1].tex;
			//uv2 = m_vertices[v2].tex;
			//uv3 = m_vertices[v3].tex;

			XMVECTOR edge1 = XMLoadFloat3(&vtx2) - XMLoadFloat3(&vtx1);
			XMVECTOR edge2 = XMLoadFloat3(&vtx3) - XMLoadFloat3(&vtx1);

			XMVECTOR n = XMVector3Normalize(XMVector3Cross(edge1, edge2));

			XMStoreFloat3(&normal, n);

			m_vertices[v1].nx = normal.x; m_vertices[v1].ny = normal.y; m_vertices[v1].nz = normal.z;
			m_vertices[v2].nx = normal.x; m_vertices[v2].ny = normal.y; m_vertices[v2].nz = normal.z;
			m_vertices[v3].nx = normal.x; m_vertices[v3].ny = normal.y; m_vertices[v3].nz = normal.z;


			vtx1 = XMFLOAT3(m_vertices[v4].x, m_vertices[v4].y, m_vertices[v4].z);
			vtx2 = XMFLOAT3(m_vertices[v5].x, m_vertices[v5].y, m_vertices[v5].z);
			vtx3 = XMFLOAT3(m_vertices[v6].x, m_vertices[v6].y, m_vertices[v6].z);

			//uv1 = m_vertices[v1].tex;
			//uv2 = m_vertices[v2].tex;
			//uv3 = m_vertices[v3].tex;

			edge1 = XMLoadFloat3(&vtx2) - XMLoadFloat3(&vtx1);
			edge2 = XMLoadFloat3(&vtx3) - XMLoadFloat3(&vtx1);

			n = XMVector3Normalize(XMVector3Cross(edge1, edge2));

			XMStoreFloat3(&normal, n);

			m_vertices[v4].nx = normal.x; m_vertices[v4].ny = normal.y; m_vertices[v4].nz = normal.z;
			m_vertices[v5].nx = normal.x; m_vertices[v5].ny = normal.y; m_vertices[v5].nz = normal.z;
			m_vertices[v6].nx = normal.x; m_vertices[v6].ny = normal.y; m_vertices[v6].nz = normal.z;


			m_indices.push_back(v1);
			m_indices.push_back(v2);
			m_indices.push_back(v3);

			m_indices.push_back(v4);
			m_indices.push_back(v5);
			m_indices.push_back(v6);

			texUIndex++;
		}
		texUIndex = 0.f;
		texVIndex++;
	}


	std::ofstream stream;
	stream.open("trolls_inn/TerrainLol.txt");
	stream << "mtllib floor.mtl\n";

	for (int i = 0; i < m_vertices.size(); i++)
		stream << "v " << m_vertices[i].x << " " << m_vertices[i].y << " " << m_vertices[i].z  << "\n";

	for (int i = 0; i < m_vertices.size(); i++)
		stream << "vt " << m_vertices[i].u << " " << m_vertices[i].v << "\n";

	for (int i = 0; i < m_vertices.size(); i++)
		stream << "vn " << m_vertices[i].nx << " " << m_vertices[i].ny << " " << m_vertices[i].nz << "\n";

	stream << "usemtl wood\n";
	for (int i = 0; i < m_indices.size(); i += 6)
	{
		stream << "f ";
		stream << m_indices[i] + 1 << "/" << m_indices[i] + 1 << "/" << m_indices[i] + 1<< " ";
		stream << m_indices[i + 1] + 1 << "/" << m_indices[i + 1] + 1 << "/" << m_indices[i + 1] + 1 << " ";
		stream << m_indices[i + 2] + 1 << "/" << m_indices[i + 2] + 1 << "/" << m_indices[i + 2] + 1 << "\n";

		stream << "f ";
		stream << m_indices[i + 4] + 1 << "/" << m_indices[i + 4] + 1 << "/" << m_indices[i + 4] + 1 << " ";
		stream << m_indices[i + 5] + 1 << "/" << m_indices[i + 5] + 1 << "/" << m_indices[i + 5] + 1 << " ";
		stream << m_indices[i + 3] + 1 << "/" << m_indices[i + 3] + 1 << "/" << m_indices[i + 3] + 1 << "\n";

	}

	stream.close();


}
