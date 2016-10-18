#pragma once
#include "IMatrix.h"



class CMatrixParallel : IMatrix
{
public:
	CMatrixParallel(size_t threadsCount);
	Matrix GetInverseMatrix() override;
	SMatrixs m_matrix;
private:
	void CalculateMatrixCofactors();
	void CalculateMatrixMinors();
	void CalculateTransposedMatrix();

	static DWORD WINAPI CalculateMatrixCofactors(PVOID pvParam);
	static DWORD WINAPI CalculateMatrixMinors(PVOID pvParam);
	static DWORD WINAPI CalculateTransposedMatrix(PVOID pvParam);
private:
	size_t m_threadsCount;
	
};