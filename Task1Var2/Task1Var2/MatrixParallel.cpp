#include "stdafx.h"
#include "MatrixParallel.h"
#include "RangeForMatrix.h"

DWORD WINAPI CMatrixParallel::CalculateMatrixCofactors(PVOID pvParam)
{
	RangeForMatrix data = (*((RangeForMatrix *)pvParam));
	for (size_t i = data.m_fromRow; i != data.m_toRow; ++i)
	{
		for (size_t j = data.m_fromColumn; j != data.m_toColumn; ++j)
		{
			matrix->minorsMatrix[i][j] *= (int)pow(-1, int(i) + int(j) + 2);
		}
	}
	return 0;
}

DWORD WINAPI CMatrixParallel::CalculateTransposedMatrix(PVOID pvParam)
{
	RangeForMatrix data = (*((RangeForMatrix *)pvParam));
	for (size_t i = data.m_fromRow; i != data.m_toRow; ++i)
	{
		for (size_t j = data.m_fromColumn; j != data.m_toColumn; ++j)
		{
			matrix->basicMatrix[j][i] = matrix->minorsMatrix[i][j];
		}
	}
	return 0;
}

DWORD CMatrixParallel::CalculateMatrixMinors(PVOID pvParam)
{
	RangeForMatrix data = (*((RangeForMatrix *)pvParam));
	Matrix copyMatrix = matrix->basicMatrix;
	for (size_t i = data.m_fromRow; i != data.m_toRow; ++i)
	{
		for (size_t j = data.m_fromColumn; j != data.m_toColumn; ++j)
		{
			copyMatrix.erase(copyMatrix.begin() + i);
			for (size_t k = 0; k != copyMatrix.size(); ++k)
			{
				copyMatrix[k].erase(copyMatrix[k].begin() + j);
			}
			float res = GetDeterminantMatrix(copyMatrix);
			matrix->minorsMatrix[i][j] = res;
			copyMatrix = matrix->basicMatrix;
		}
	}
	return 0;
}


float CMatrixParallel::GetDeterminantMatrix(Matrix matrix)
{
	float result = 0.f;
	int count = 1;
	Matrix oldMatrix(matrix);
	size_t size = matrix.size();
	for (size_t i = 0; i != size; ++i)
	{
		float number = matrix[0][i];
		matrix.erase(matrix.begin() + 0);
		for (size_t k = 0; k != matrix.size(); ++k)
		{
			matrix[k].erase(matrix[k].begin() + i);
		}
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

CMatrixParallel::CMatrixParallel(size_t numberThreads)
	: m_threadsCount(numberThreads)
{
}

Matrix CMatrixParallel::GetInverseMatrix()
{
	float number = (GetDeterminantMatrix(matrix->basicMatrix));
	m_matrix.base = (matrix->basicMatrix);
	CalculateMatrixMinors();
	CalculateMatrixCofactors();
	CalculateTransposedMatrix();
	for (auto &row : matrix->basicMatrix)
	{
		for (auto &column : row)
		{
			if (number < 0)
			{
				column = -column / -number;
			}
			else
			{
				column = column / number;
			}
		}
	}
	return matrix->basicMatrix;
}

void CMatrixParallel::CalculateMatrixCofactors()
{
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_threadsCount - 1);
	std::vector<HANDLE> hThread;
	size_t matrixSizeForOneRow = matrix->basicMatrix[0].size();
	size_t matrixSize = matrix->basicMatrix.size();
	size_t lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = size_t(matrixSizeForOneRow / m_threadsCount) + 1;
	}
	size_t lengthColumn = matrixSizeForOneRow;
	size_t fromColumn = lengthRow;
	if (matrixSize < m_threadsCount)
	{
		lengthRow = 1;
		if (matrixSizeForOneRow % m_threadsCount > 0)
		{
			lengthColumn = static_cast<size_t>(matrixSize / size_t((m_threadsCount / matrixSizeForOneRow) + 1));
		}
		else
		{
			lengthColumn = static_cast<size_t>(matrixSize / size_t(m_threadsCount / matrixSizeForOneRow));
		}
	}
	size_t toRow = lengthColumn;
	size_t fromRow = 0;
	size_t toColumn;
	for (size_t id = 1; id != m_threadsCount; ++id)
	{
		if (id + 1 != m_threadsCount)
		{
			toColumn = lengthRow + fromColumn >= matrixSize ? matrixSize : lengthRow + fromColumn;
		}

		auto th = CreateThread(NULL, 0, CalculateMatrixCofactors, (PVOID)&RangeForMatrix(fromRow, fromColumn, toRow, toColumn), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_threadsCount - id - 1 == matrixSize - toColumn + 1)
		{
			lengthRow = 1;
		}
		fromColumn += lengthRow;
		if (fromColumn == matrixSize && id + 1 != m_threadsCount)
		{
			fromColumn = 1;
			toRow += lengthColumn;
			fromRow += lengthColumn;
			if (m_threadsCount - id - 1 <= matrixSize || toRow > matrixSize)
			{
				toRow = matrixSize;
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			toColumn = matrixSize;
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount) + 1;
	}

	CalculateMatrixCofactors((PVOID)&RangeForMatrix(0, 0, matrix->basicMatrix.size(), lengthRow));
	WaitForMultipleObjects(static_cast<DWORD>(m_threadsCount - 1), hThread.data(), TRUE, INFINITE);
}

void CMatrixParallel::CalculateMatrixMinors()
{

	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_threadsCount - 1);
	std::vector<HANDLE> hThread;
	size_t matrixSizeForOneRow = matrix->basicMatrix[0].size();
	size_t matrixSize = matrix->basicMatrix.size();
	size_t lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = int(matrixSizeForOneRow / m_threadsCount) + 1;
	}
	size_t lengthColumn = matrixSizeForOneRow;
	size_t from_y = lengthRow;
	if (matrixSize < m_threadsCount)
	{
		lengthRow = 1;
		if (matrixSizeForOneRow % m_threadsCount > 0)
		{
			lengthColumn = static_cast<size_t>(matrixSize / size_t((m_threadsCount / matrixSizeForOneRow) + 1));
		}
		else
		{
			lengthColumn = static_cast<size_t>(matrixSize / size_t(m_threadsCount / matrixSizeForOneRow));
		}
	}
	size_t toRow = lengthColumn;
	size_t fromRow = 0;
	size_t toColumn;
	for (size_t id = 1; id != m_threadsCount; ++id)
	{
		if (id + 1 != m_threadsCount)
		{
			toColumn = lengthRow + from_y >= matrixSize ? matrixSize : lengthRow + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateMatrixMinors, (PVOID)&RangeForMatrix(fromRow, from_y, toRow, toColumn), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_threadsCount - id - 1 == matrixSize - toColumn + 1)
		{
			lengthRow = 1;
		}
		from_y += lengthRow;
		if (from_y == matrixSize && id + 1 != m_threadsCount)
		{
			from_y = 1;
			toRow += lengthColumn;
			fromRow += lengthColumn;
			if (m_threadsCount - id - 1 <= matrixSize || toRow > matrixSize)
			{
				toRow = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			toColumn = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount) + 1;
	}

	CalculateMatrixMinors((PVOID)&RangeForMatrix(0, 0, matrix->basicMatrix.size(), lengthRow));
	WaitForMultipleObjects(static_cast<DWORD>(m_threadsCount - 1), hThread.data(), TRUE, INFINITE);
}

void CMatrixParallel::CalculateTransposedMatrix()
{
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_threadsCount - 1);
	std::vector<HANDLE> hThread;
	size_t matrixSizeForOneRow = matrix->basicMatrix[0].size();
	size_t matrixSize = matrix->basicMatrix.size();
	size_t lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = int(matrixSizeForOneRow / m_threadsCount) + 1;
	}
	size_t lengthColumn = matrixSizeForOneRow;
	size_t fromColumn = lengthRow;
	if (matrixSize < m_threadsCount)
	{
		lengthRow = 1;
		if (matrixSizeForOneRow % m_threadsCount > 0)
		{
			lengthColumn = static_cast<size_t>(matrixSize / size_t((m_threadsCount / matrixSizeForOneRow) + 1));
		}
		else
		{
			lengthColumn = static_cast<size_t>(matrixSize / size_t(m_threadsCount / matrixSizeForOneRow));
		}
	}
	size_t toRow = lengthColumn;
	size_t fromRow = 0;
	size_t toColumn;
	for (size_t id = 1; id != m_threadsCount; ++id)
	{
		if (id + 1 != m_threadsCount)
		{
			toColumn = lengthRow + fromColumn >= matrixSize ? matrixSize : lengthRow + fromColumn;
		}

		auto th = CreateThread(NULL, 0, CalculateTransposedMatrix, (PVOID)&RangeForMatrix(fromRow, fromColumn, toRow, toColumn), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_threadsCount - id - 1 == matrixSize - toColumn + 1)
		{
			lengthRow = 1;
		}
		fromColumn += lengthRow;
		if (fromColumn == matrixSize && id + 1 != m_threadsCount)
		{
			fromColumn = 1;
			toRow += lengthColumn;
			fromRow += lengthColumn;
			if (m_threadsCount - id - 1 <= matrixSize || toRow > matrixSize)
			{
				toRow = matrixSize;
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			toColumn = matrixSize;
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount) + 1;
	}

	CalculateTransposedMatrix((PVOID)&RangeForMatrix(0, 0, matrix->basicMatrix.size(), lengthRow));
	WaitForMultipleObjects(static_cast<DWORD>(m_threadsCount - 1), hThread.data(), TRUE, INFINITE);
}
