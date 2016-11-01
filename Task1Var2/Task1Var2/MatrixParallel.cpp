#include "stdafx.h"
#include "MatrixParallel.h"
#include "RangeForMatrix.h"

void Cofactors(CMatrixInverseParallel * matrix, RangeForMatrix& data)
{
	for (size_t i = data.m_fromRow; i != data.m_toRow; ++i)
	{
		for (size_t j = data.m_fromColumn; j != data.m_toColumn; ++j)
		{
			matrix->CalculateMatrixCofactors(i, j);
		}
	}
}

void Transposed(CMatrixInverseParallel * matrix, RangeForMatrix& data)
{
	for (size_t i = data.m_fromRow; i != data.m_toRow; ++i)
	{
		for (size_t j = data.m_fromColumn; j != data.m_toColumn; ++j)
		{
			matrix->CalculateTransposedMatrix(i, j);
		}
	}
}

void Minors(CMatrixInverseParallel * matrix, RangeForMatrix& data)
{
	Matrix copyMatrix = matrix->GetMatrixData().basicMatrix;
	for (size_t i = data.m_fromRow; i != data.m_toRow; ++i)
	{
		for (size_t j = data.m_fromColumn; j != data.m_toColumn; ++j)
		{
			matrix->CalculateMatrixMinors(copyMatrix, i, j);
		}
	}
}

void CMatrixInverseParallel::CalculateMatrixCofactors(int i, int j)
{
	m_matrixData.minorsMatrix[i][j] *= (int)pow(-1, int(i) + int(j) + 2);
}

void CMatrixInverseParallel::CalculateTransposedMatrix(int i, int j)
{
	m_matrixData.basicMatrix[j][i] = m_matrixData.minorsMatrix[i][j];
}

void CMatrixInverseParallel::CalculateMatrixMinors(Matrix &matrix, int i, int j)
{
	matrix.erase(matrix.begin() + i);
	for (size_t k = 0; k != matrix.size(); ++k)
	{
		matrix[k].erase(matrix[k].begin() + j);
	}
	float res = GetDeterminantMatrix(matrix);
	m_matrixData.minorsMatrix[i][j] = res;
	matrix = m_matrixData.basicMatrix;
}


float CMatrixInverseParallel::GetDeterminantMatrix(Matrix matrix)
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

CMatrixInverseParallel::CMatrixInverseParallel(size_t threadsCount, MatrixData matrixData)
	: m_threadsCount(threadsCount)
	, m_matrixData(matrixData)

{

}

Matrix CMatrixInverseParallel::GetInverseMatrix()
{
	float number = (GetDeterminantMatrix(m_matrixData.basicMatrix));
	CalculateComponents(CalculateMinors);
	CalculateComponents(CalculateCofactors);
	CalculateComponents(CalculateTransposed);

	for (auto &row : m_matrixData.basicMatrix)
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
	return m_matrixData.basicMatrix;
}

MatrixData CMatrixInverseParallel::GetMatrixData()
{
	return m_matrixData;
}

void CMatrixInverseParallel::CalculateComponents(TypeCalculate type)
{
	std::vector<std::thread> hTreads;
	size_t matrixSizeForOneRow = m_matrixData.basicMatrix[0].size();
	size_t matrixSize = m_matrixData.basicMatrix.size();
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
		toColumn = lengthRow + fromColumn >= matrixSize ? matrixSize : lengthRow + fromColumn;
		switch (type)
		{
		case CalculateMinors:
			hTreads.push_back(std::thread(Minors, this, RangeForMatrix(fromRow, fromColumn, toRow, toColumn)));
			break;
		case CalculateCofactors:
			hTreads.push_back(std::thread(Cofactors, this, RangeForMatrix(fromRow, fromColumn, toRow, toColumn)));
			break;
		case CalculateTransposed:
			hTreads.push_back(std::thread(Transposed, this, RangeForMatrix(fromRow, fromColumn, toRow, toColumn)));
			break;
		}
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
				toRow = m_matrixData.basicMatrix.size();
			}

		}
		if (id + 1 == m_threadsCount - 1)
		{
			toColumn = m_matrixData.basicMatrix.size();
		}
	}
	lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount);
	if (matrixSizeForOneRow % m_threadsCount > 0)
	{
		lengthRow = static_cast<size_t>(matrixSizeForOneRow / m_threadsCount) + 1;
	}
	switch (type)
	{
	case CalculateMinors:
		hTreads.push_back(std::thread(Minors, this, RangeForMatrix(0, 0, m_matrixData.basicMatrix.size(), lengthRow)));
		break;
	case CalculateCofactors:
		hTreads.push_back(std::thread(Cofactors, this, RangeForMatrix(0, 0, m_matrixData.basicMatrix.size(), lengthRow)));
		break;
	case CalculateTransposed:
		hTreads.push_back(std::thread(Transposed, this, RangeForMatrix(0, 0, m_matrixData.basicMatrix.size(), lengthRow)));
		break;
	}
	for (auto &it : hTreads)
	{
		it.join();
	}
}