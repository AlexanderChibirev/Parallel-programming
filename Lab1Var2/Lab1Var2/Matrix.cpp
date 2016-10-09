#include "stdafx.h"
#include "Matrix.h"

const int n = 5;

using namespace std;

std::unique_ptr<SMatrixs> matrix = std::make_unique<SMatrixs>();

namespace
{

	void Erase(vector<vector<int>> &matrix, int i, int j)
	{
		matrix.erase(matrix.begin() + i);
		for (size_t k = 0; k != matrix.size(); ++k)
		{
			matrix[k].erase(matrix[k].begin() + j);
		}
	}

	void Resize(vector<vector<int>> &matrix, int number)
	{
		matrix.resize(number);
		for (size_t i = 0; i != matrix.size(); ++i)
		{
			matrix[i].resize(number);
		}
	}

	int GetDeterminantMatrix(Matrix matrix, int result = 0, int count = 1)
	{
		vector<vector<int>> oldMatrix(matrix);
		size_t size = matrix.size();
		for (size_t i = 0; i != size; ++i)
		{
			int number = matrix[0][i];
			Erase(matrix, 0, i);
			if (matrix.size() == 2)
			{
				result += number * (int)pow(-1, 1 + count++) * (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
			}
			else
			{
				result += number * (int)pow(-1, 1 + count++) * GetDeterminantMatrix(matrix);
			}
			matrix = oldMatrix;
		}
		return result;
	}
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

DWORD WINAPI CMatrixHelperParallel::CalculateMatrixCofactors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			matrix->minorsMatrix[i][j] *= pow(-1, i + j + 2);
		}
	}
	return 0;
}

DWORD WINAPI CMatrixHelperParallel::CalculateTransposedMatrix(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			matrix->basicMatrix[j][i] = matrix->minorsMatrix[i][j];
		}
	}
	return 0;
}

/*int CMatrixHelperParallel::CalculateDeterminantMatrix(Matrix matrix, int result, int count)
{
vector<vector<int>> oldMatrix(matrix);
size_t size = oldMatrix.size();
for (size_t i = 0; i != size; ++i)
{
int number = oldMatrix[0][i];
Erase(oldMatrix, 0, i);
if (oldMatrix.size() == 2)
{
result += number * (int)pow(-1, 1 + count++) * (oldMatrix[0][0] * oldMatrix[1][1] - oldMatrix[0][1] * oldMatrix[1][0]);
}
else
{
result += number * (int)pow(-1, 1 + count++) * CalculateDeterminantMatrix(oldMatrix);
}
oldMatrix = matrix;
}
return result;
}*/

DWORD WINAPI CMatrixHelperParallel::CalculateMatrixMinors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	Matrix copyMatrix = matrix->basicMatrix;
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			int result = 0;
			Erase(copyMatrix, i, j);
			int res = GetDeterminantMatrix(copyMatrix);
			matrix->minorsMatrix[i][j] = res;
			copyMatrix = matrix->basicMatrix;
		}
	}
	return 0;
}


CMatrixHelperParallel::CMatrixHelperParallel(int numberThreads)
	: m_numberThreads(numberThreads)
{}

Matrix CMatrixHelperParallel::GetTransposedMatrix()
{
	Output(matrix->basicMatrix);
	CalculateMatrixMinors();

	Output(matrix->minorsMatrix);
	CalculateMatrixCofactors();
	Output(matrix->minorsMatrix);
	CalculateTransposedMatrix();
	Output(matrix->basicMatrix);

	return matrix->basicMatrix;
}

void CMatrixHelperParallel::CalculateMatrixCofactors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;
	int width = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	int y = width;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << width * id << std::endl;
		std::cout << "width * (j + 1) = " << width * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;

		int to_y = width + y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : width + y;
		auto th = CreateThread(NULL, 0, CalculateMatrixCofactors,
			(PVOID)&SMiniMatrix(0, y, matrix->basicMatrix.size(), to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_numberThreads - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			width = 1;
		}
		y += width;
		if (!hThread.data()[id]) std::cout << "Error!" << std::endl;
	}
	width = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	CalculateMatrixCofactors((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), width));

	dw = WaitForMultipleObjects(m_numberThreads - 1, hThread.data(), TRUE, INFINITE);

}

void CMatrixHelperParallel::CalculateMatrixMinors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;
	int width = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	int y = width;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << width * id << std::endl;
		std::cout << "width * (j + 1) = " << width * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;

		int to_y = width + y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : width + y;
		auto th = CreateThread(NULL, 0, CalculateMatrixMinors,
			(PVOID)&SMiniMatrix(0, y, matrix->basicMatrix.size(), to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_numberThreads - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			width = 1;
		}
		y += width;
		if (!hThread.data()[id]) std::cout << "Error!" << std::endl;
	}
	width = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	CalculateMatrixMinors((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), width));
	dw = WaitForMultipleObjects(m_numberThreads - 1, hThread.data(), TRUE, INFINITE);
}

void CMatrixHelperParallel::CalculateTransposedMatrix()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;
	int width = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	int y = width;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << width * id << std::endl;
		std::cout << "width * (j + 1) = " << width * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;

		int to_y = width + y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : width + y;
		auto th = CreateThread(NULL, 0, CalculateTransposedMatrix,
			(PVOID)&SMiniMatrix(0, y, matrix->basicMatrix.size(), to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_numberThreads - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			width = 1;
		}
		y += width;
		if (!hThread.data()[id]) std::cout << "Error!" << std::endl;
	}
	width = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	CalculateTransposedMatrix((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), width));


	dw = WaitForMultipleObjects(m_numberThreads - 1, hThread.data(), TRUE, INFINITE);


}


Matrix CMatrixHelperSuccessively::GetTransposedMatrix()
{
	Matrix basicMatrix = matrix->copyBasicMatrix;
	Matrix minorsMatrix;
	minorsMatrix = GetMatrixMinors(basicMatrix);
	Output(minorsMatrix);
	minorsMatrix = GetMatrixCofactors(minorsMatrix);
	std::cout << "|***************************************************|" << std::endl;
	Output(minorsMatrix);
	basicMatrix = GetTransposedMatrix(minorsMatrix);
	std::cout << "|***************************************************|" << std::endl;
	return basicMatrix;
}

Matrix CMatrixHelperSuccessively::GetMatrixCofactors(Matrix matrix)
{
	int count = 1;
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix[i].size(); ++j)
		{
			matrix[i][j] *= pow(-1, i + j + 2);
		}
	}
	return matrix;
}

Matrix CMatrixHelperSuccessively::GetTransposedMatrix(Matrix const &matrix)
{
	vector<vector<int>> newMatrix;
	Resize(newMatrix, matrix.size());
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix[i].size(); ++j)
		{
			newMatrix[j][i] = matrix[i][j];
		}
	}
	return newMatrix;
}



Matrix CMatrixHelperSuccessively::GetMatrixMinors(Matrix matrix)
{
	Matrix copyMatrix = matrix;
	Matrix newMatrix;
	Resize(newMatrix, matrix.size());
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix[i].size(); ++j)
		{
			int result = 0;
			Erase(copyMatrix, i, j);
			int res = GetDeterminantMatrix(copyMatrix);
			newMatrix[i][j] = res;
			copyMatrix = matrix;
		}
	}
	return newMatrix;
}


