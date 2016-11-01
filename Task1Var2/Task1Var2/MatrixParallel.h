#pragma once
#include "IMatrix.h"
#include <memory>
#include <thread>


enum TypeCalculate
{
	CalculateCofactors,
	CalculateMinors,
	CalculateTransposed
};

class CMatrixInverseParallel : IMatrix
{
public:
	CMatrixInverseParallel(size_t threadsCount, MatrixData matrixData);
	Matrix GetInverseMatrix() override;
	MatrixData GetMatrixData();
	float GetDeterminantMatrix(Matrix matrix);
	void CalculateMatrixMinors(Matrix &copyMatrix, int i, int j);
	void CalculateMatrixCofactors(int i, int j);
	void CalculateTransposedMatrix(int i, int j);
private:
	void CalculateComponents(TypeCalculate type);
private:
	size_t m_threadsCount;
	MatrixData m_matrixData;
};