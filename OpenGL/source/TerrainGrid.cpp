#include "TerrainGrid.h"

TerrainGrid::TerrainGrid(const int w, const int l, const float gridSize)
{
	float x0 = -gridSize * w/2;
	float z0 = -gridSize * l/2;

	for (int i = 0; i < w+1; i++) 
	{
		for (int j = 0; j < l+1; j++)
		{
			VectorPos vecPos = { x0 + i * gridSize , 0.0f, z0 + j * gridSize };

			m_Vertices.push_back(vecPos);
		}
	}

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < l; j++)
		{
			int topLeft = i * (l+1) + j;
			int topRight = i * (l+1) + j + 1;

			int botLeft = (i+1) * (l+1) + j;
			int botRight = (i+1) * (l+1) + j + 1;

			m_Indices.push_back(topLeft);
			m_Indices.push_back(topRight);
			m_Indices.push_back(botLeft);

			m_Indices.push_back(botLeft);
			m_Indices.push_back(topRight);
			m_Indices.push_back(botRight);
		}
	}
}
