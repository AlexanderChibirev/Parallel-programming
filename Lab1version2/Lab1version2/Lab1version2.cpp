// Lab1version2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

const int MATRIX_SIZE = 31;
const double EPS = 1E-9;

using namespace std;

int GetRang(std::vector<vector<float>> &matrix)
{
	int rank = MATRIX_SIZE;
	vector<bool> line_used(MATRIX_SIZE);
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		int j;
		for (j = 0; j < MATRIX_SIZE; ++j)
			if (!line_used[j] && abs(matrix[j][i]) > EPS)
				break;
		if (j == MATRIX_SIZE)
			--rank;
		else 
		{
			line_used[j] = true;
			for (int p = i + 1; p< MATRIX_SIZE; ++p)
				matrix[j][p] /= matrix[j][i];
			for (int k = 0; k< MATRIX_SIZE; ++k)
				if (k != j && abs(matrix[k][i]) > EPS)
					for (int p = i + 1; p< MATRIX_SIZE; ++p)
						matrix[k][p] -= matrix[j][p] * matrix[k][i];
		}
	}
	return rank;
}


vector<vector<float>> GenerateMatrix()
{
	vector<vector<float>> matrix(MATRIX_SIZE);

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j< MATRIX_SIZE; j++)
		{
			matrix[i].push_back(rand() % 1009);
			cout << matrix[i][j] << '\t';
		}
		cout << '\n';
	}
	return matrix;
}

int main()
{
	cout << GetRang(GenerateMatrix());
	return 0;
}

