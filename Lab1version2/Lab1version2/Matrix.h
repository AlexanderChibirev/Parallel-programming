#pragma once
#include "Difinition.h"

class Matrix
{
public:
	Matrix(const int quantityStream);
	int GetRank();
	~Matrix();
	row GenerateMatrixForOneThreads();
	int ProcessGetRank(row *matrix[MATRIX_SIZE]);
private:
	void Start();
	int m_threadQuantity = 0;
	int m_rank = 0;
};

