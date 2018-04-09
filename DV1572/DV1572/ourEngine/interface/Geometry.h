#pragma once
#include "shape/Mesh.h"


namespace Geometry
{
	Mesh* createRectangle()
	{
		std::vector<VERTEX> vertices;

		VERTEX v = {

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		vertices.push_back(v);

		v = {

			0.0f, 0.5f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		vertices.push_back(v);

		v = {
			0.5f, 0.5f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		vertices.push_back(v);

		v = {
			0.5f, 0.5f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		vertices.push_back(v);

		v = {
			0.5f, 0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		vertices.push_back(v);

		v = {

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};

		Mesh* m;
		m = new Mesh();
		m->LoadModel(vertices);

		return m;
	}
}