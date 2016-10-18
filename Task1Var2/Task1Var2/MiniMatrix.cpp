#include "stdafx.h"
#include "MiniMatrix.h"
#include "Matrixs.h"

using namespace std;

void Erase(Matrix &matrix, size_t i, size_t j)
{
	matrix.erase(matrix.begin() + i);
	for (size_t k = 0; k != matrix.size(); ++k)
	{
		matrix[k].erase(matrix[k].begin() + j);
	}
}

void Resize(Matrix &matrix, int number)
{
	matrix.resize(number);
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		matrix[i].resize(number);
	}
}

float GetDeterminantMatrix(Matrix matrix, float result, int count)
{
	Matrix oldMatrix(matrix);
	size_t size = matrix.size();
	for (size_t i = 0; i != size; ++i)
	{
		float number = matrix[0][i];
		Erase(matrix, 0, i);
		if (matrix.size() == 2)
		{
			result += (number * powf(-1.f, float(1 + count++)) * (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]));
		}
		else
		{
			result += (number * powf(-1.f, float(1 + count++)) * GetDeterminantMatrix(matrix));
		}
		matrix = oldMatrix;
	}
	return result;
}


void Output(Matrix const &matrix)
{
	for (auto el : matrix)
	{
		for (auto el2 : el)
		{
			std::cout << " " << el2;
		}
		std::cout << std::endl;
	}
}