#pragma once
#include "IMatrix.h"

class CMatrixSuccessively : IMatrix
{

public:
	CMatrixSuccessively(std::vector<std::vector<float>> matrix);
	std::vector<std::vector<float>> GetInverseMatrix() override;
private:
	int m_matrixSize;
	CMatrixSuccessively CoFactor();
	std::vector<std::vector<float>> m_pData;
	float GetDeterminant();
	float SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd);
	float SearchDetThenMatrixSizeEqualFour();
	float SearchDetThenMatrixSizeEqualFive();
};

