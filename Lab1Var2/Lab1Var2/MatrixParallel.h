#pragma once
#include "IMatrix.h"
#include <memory>
#include <algorithm>

struct ColumnInverse
{
	ColumnInverse(std::vector<std::vector<float>> &inverseMatrix, std::vector<std::vector<float>> &cofactorMatrix, int i, float det, int matrixSize)
		: m_inverseMatrix(inverseMatrix)
		, m_cofactorMatrix(cofactorMatrix)
		, m_i(i)
		, m_det(det)
		, m_matrixSize(matrixSize)
	{}
	int m_i;
	float m_det;
	int	m_matrixSize;
	std::vector<std::vector<float>> m_cofactorMatrix;
	std::vector<std::vector<float>> m_inverseMatrix;
};


class CMatrixParallel : IMatrix
{
public:
	CMatrixParallel(const int quantityThread, std::vector<std::vector<float>> &matrix);
	std::vector<std::vector<float>> GetInverseMatrix() override;
	float GetDeterminant();
	CMatrixParallel CoFactor();
	DWORD WINAPI GetDeterminantTread(PVOID pvParam);

private:
	int m_matrixSize;
	int m_quantityThread;
	
	static DWORD WINAPI InverseColumnMatrix(void *pvParam);
	static DWORD WINAPI DeterminantProcessThenMatrixSizeMoreFive(PVOID pvParam);
	
	float SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd);
	float SearchDetThenMatrixSizeEqualFour();
	float SearchDetThenMatrixSizeEqualFive();
	static DWORD WINAPI CalculateMatrixCofactors(PVOID pvParam);
	void FillThreadsChargeMap();
	std::vector<std::vector<float>> m_pData;
	std::vector<std::vector<float>> m_inverseData;
	std::map<size_t, std::vector<size_t>> m_threadsChargeMap;
	float m_det;
	std::vector<std::vector<float>> m_coaf;
};

struct DeterminantProcess
{
	DeterminantProcess(std::vector<CMatrixParallel> &temp, std::vector<std::vector<float>> &pData, int DIM, int k)
		: m_pData(pData)
		, m_DIM(DIM)
		, m_temp(temp)
		, m_k(k)
	{
	}
	DeterminantProcess()
	{}
	std::vector<CMatrixParallel> m_temp;
	std::vector<std::vector<float>> m_pData;
	int m_DIM;
	int m_k;
};

struct RangesForMatrix
{
	RangesForMatrix(int fromRow = 0, int fromColumn = 0, int toRow = 0, int toColumn = 0)
		: m_toRow(toRow)
		, m_toColumn(toRow)
		, m_fromRow(fromRow)
		, m_fromColumn(fromColumn)
	{}
	int m_toRow;
	int m_toColumn;
	int m_fromRow;
	int m_fromColumn;
};