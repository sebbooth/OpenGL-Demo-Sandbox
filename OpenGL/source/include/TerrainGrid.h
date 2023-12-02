#pragma once

#include "Renderer.h"

struct VectorPos {
	float x, y, z;
};

class TerrainGrid
{
public:
	TerrainGrid(const int w, const int l, const float gridSize);

	std::vector<VectorPos> m_Vertices;
	std::vector<unsigned int> m_Indices;
};

