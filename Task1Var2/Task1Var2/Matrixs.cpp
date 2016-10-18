#include "stdafx.h"
#include "Matrixs.h"
#include "MiniMatrix.h"

SMatrixs::SMatrixs(std::vector<std::vector<float>> matrix)
{
	int size = matrix.size();
	basicMatrix.resize(size);
	for (size_t i = 0; i != size; ++i)
	{
		basicMatrix[i].resize(size);
		for (size_t j = 0; j != size; ++j)
		{
			basicMatrix[i][j] = matrix[i][j];
		}
	}
	minorsMatrix.resize(size);
	for (size_t i = 0; i != size; ++i)
	{
		minorsMatrix[i].resize(size);
	}
	copyBasicMatrix = basicMatrix;
	base = basicMatrix;
}

SMatrixs::SMatrixs()
{
	int size = 6;
	basicMatrix.resize(size);
	for (size_t i = 0; i != size; ++i)
	{
		basicMatrix[i].resize(size);
		for (size_t j = 0; j != size; ++j)
		{
			basicMatrix[i][j] = rand() % 9 + 1;
		}
	}
	minorsMatrix.resize(size);
	for (size_t i = 0; i != size; ++i)
	{
		minorsMatrix[i].resize(size);
	}
	copyBasicMatrix = basicMatrix;
}