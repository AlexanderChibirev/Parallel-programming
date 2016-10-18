#include "stdafx.h"
#include "MatrixParallel.h"
#include "MiniMatrix.h"

DWORD WINAPI CMatrixParallel::CalculateMatrixCofactors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			matrix->minorsMatrix[i][j] *= (int)pow(-1, int(i) + int(j) + 2);
		}
	}
	return 0;
}

DWORD WINAPI CMatrixParallel::CalculateTransposedMatrix(PVOID pvParam)
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

DWORD CMatrixParallel::CalculateMatrixMinors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	Matrix copyMatrix = matrix->basicMatrix;
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			Erase(copyMatrix, i, j);
			float res = GetDeterminantMatrix(copyMatrix);
			matrix->minorsMatrix[i][j] = res;
			copyMatrix = matrix->basicMatrix;
		}
	}
	return 0;
}


CMatrixParallel::CMatrixParallel(size_t numberThreads)
	: m_threadsCount(numberThreads)
{
}

Matrix CMatrixParallel::GetInverseMatrix()
{
	float number = (GetDeterminantMatrix(matrix->basicMatrix));
	m_matrix = (matrix->basicMatrix);
	CalculateMatrixMinors();
	CalculateMatrixCofactors();
	CalculateTransposedMatrix();
	for (auto &row : matrix->basicMatrix)
	{
		for (auto &cell : row)
		{
			if (number < 0)
			{
				cell = -cell / -number;
			}
			else
			{
				cell = cell / number;
			}
		}
	}
	return matrix->basicMatrix;
}

void CMatrixParallel::CalculateMatrixCofactors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_threadsCount - 1);
	std::vector<HANDLE> hThread;

	size_t lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount);
	if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
	{
		lengthByX = int(matrix->basicMatrix[0].size() / m_threadsCount) + 1;
	}
	size_t lengthByY = matrix->basicMatrix[0].size();
	size_t from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_threadsCount)
	{
		lengthByX = 1;
		if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int((m_threadsCount / matrix->basicMatrix[0].size()) + 1));
		}
		else
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int(m_threadsCount / matrix->basicMatrix[0].size()));
		}
	}
	size_t to_x = lengthByY;
	size_t from_x = 0;
	size_t to_y;
	for (size_t id = 1; id != m_threadsCount; ++id)
	{
		if (id + 1 != m_threadsCount)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : lengthByX + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateMatrixCofactors, (PVOID)&SMiniMatrix(from_x, from_y, to_x, to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_threadsCount - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			lengthByX = 1;
		}
		from_y += lengthByX;
		if (from_y == matrix->basicMatrix.size() && id + 1 != m_threadsCount)
		{
			from_y = 1;
			to_x += lengthByY;
			from_x += lengthByY;
			if (m_threadsCount - id - 1 <= matrix->basicMatrix.size() || to_x > matrix->basicMatrix.size())
			{
				to_x = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			to_y = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount);
	if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
	{
		lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount) + 1;
	}

	CalculateMatrixCofactors((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(static_cast<DWORD>(m_threadsCount - 1), hThread.data(), TRUE, INFINITE);

}

void CMatrixParallel::CalculateMatrixMinors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_threadsCount - 1);
	std::vector<HANDLE> hThread;

	size_t lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount);
	if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
	{
		lengthByX = int(matrix->basicMatrix[0].size() / m_threadsCount) + 1;
	}
	size_t lengthByY = matrix->basicMatrix[0].size();
	size_t from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_threadsCount)
	{
		lengthByX = 1;
		if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int((m_threadsCount / matrix->basicMatrix[0].size()) + 1));
		}
		else
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int(m_threadsCount / matrix->basicMatrix[0].size()));
		}
	}
	size_t to_x = lengthByY;
	size_t from_x = 0;
	size_t to_y;
	for (size_t id = 1; id != m_threadsCount; ++id)
	{
		if (id + 1 != m_threadsCount)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : lengthByX + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateMatrixMinors, (PVOID)&SMiniMatrix(from_x, from_y, to_x, to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_threadsCount - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			lengthByX = 1;
		}
		from_y += lengthByX;
		if (from_y == matrix->basicMatrix.size() && id + 1 != m_threadsCount)
		{
			from_y = 1;
			to_x += lengthByY;
			from_x += lengthByY;
			if (m_threadsCount - id - 1 <= matrix->basicMatrix.size() || to_x > matrix->basicMatrix.size())
			{
				to_x = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			to_y = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount);
	if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
	{
		lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount) + 1;
	}

	CalculateMatrixMinors((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(static_cast<DWORD>(m_threadsCount - 1), hThread.data(), TRUE, INFINITE);
}

void CMatrixParallel::CalculateTransposedMatrix()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_threadsCount - 1);
	std::vector<HANDLE> hThread;

	size_t lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount);
	if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
	{
		lengthByX = int(matrix->basicMatrix[0].size() / m_threadsCount) + 1;
	}
	size_t lengthByY = matrix->basicMatrix[0].size();
	size_t from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_threadsCount)
	{
		lengthByX = 1;
		if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int((m_threadsCount / matrix->basicMatrix[0].size()) + 1));
		}
		else
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int(m_threadsCount / matrix->basicMatrix[0].size()));
		}
	}
	size_t to_x = lengthByY;
	size_t from_x = 0;
	size_t to_y;
	for (size_t id = 1; id != m_threadsCount; ++id)
	{
		if (id + 1 != m_threadsCount)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : lengthByX + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateTransposedMatrix, (PVOID)&SMiniMatrix(from_x, from_y, to_x, to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_threadsCount - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			lengthByX = 1;
		}
		from_y += lengthByX;
		if (from_y == matrix->basicMatrix.size() && id + 1 != m_threadsCount)
		{
			from_y = 1;
			to_x += lengthByY;
			from_x += lengthByY;
			if (m_threadsCount - id - 1 <= matrix->basicMatrix.size() || to_x > matrix->basicMatrix.size())
			{
				to_x = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			to_y = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount);
	if (matrix->basicMatrix[0].size() % m_threadsCount > 0)
	{
		lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_threadsCount) + 1;
	}

	CalculateTransposedMatrix((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(static_cast<DWORD>(m_threadsCount - 1), hThread.data(), TRUE, INFINITE);
}
