#include "stdafx.h"
#include "MatrixSuccessively.h"
#include "RangeForMatrix.h"

using namespace std;
CMatrixInverseSuccessively::CMatrixInverseSuccessively(std::vector<std::vector<float>> matrix)
	:m_baseMatrix(matrix)
{
		m_matrixSize = matrix.size();
}


float CMatrixInverseSuccessively::GetDeterminant() 
{
	float det = 0;
	std::vector<std::vector<float>> pd = m_baseMatrix;
	switch (m_matrixSize)
	{
	case 2:
	{
		det = pd[0][0] * pd[1][1] - pd[0][1] * pd[1][0];
		return det;
	}
	break;
	case 3:
	{
		return SearchDetThenMatrixSizeEqualThree(pd);
	}
	break;
	case 4:
	{
		return SearchDetThenMatrixSizeEqualFour();
	}
	break;
	case 5:
	{
		return SearchDetThenMatrixSizeEqualFive();
	}
	default:
	{
		int DIM = m_matrixSize;
		std::vector<CMatrixInverseSuccessively> temp;
		for (int i = 0; i < DIM; i++)
		{
			std::vector<std::vector<float>> matrix(DIM - 1, std::vector<float>(DIM - 1));
			temp.push_back(CMatrixInverseSuccessively(matrix));
		}
		for (int k = 0; k < DIM; k++)
		{
			for (int i = 1; i < DIM; i++)
			{
				int j1 = 0;
				for (int j = 0; j < DIM; j++)
				{
					if (k == j)
						continue;
					temp[k].m_baseMatrix[i - 1][j1++] = this->m_baseMatrix[i][j];
				}
			}
		}
		float det = 0;
		for (int k = 0; k < DIM; k++)
		{
			if ((k % 2) == 0)
				det = det + (this->m_baseMatrix[0][k] * temp[k].GetDeterminant());
			else
				det = det - (this->m_baseMatrix[0][k] * temp[k].GetDeterminant());
		}
		return det;
	}
	break;
	}
}

float CMatrixInverseSuccessively::SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd)
{
	float det = (pd[0][0] * pd[1][1] * pd[2][2] + pd[0][1] * pd[1][2] * pd[2][0] + pd[0][2] * pd[1][0] * pd[2][1]);
	det = det - pd[0][0] * pd[1][2] * pd[2][1];
	det = det - pd[0][1] * pd[1][0] * pd[2][2];
	det = det - pd[0][2] * pd[1][1] * pd[2][0];
	return det;
}

float CMatrixInverseSuccessively::SearchDetThenMatrixSizeEqualFour()
{
	std::vector<CMatrixInverseSuccessively*> temp(4);
	for (int i = 0; i < 4; i++)
	{
		std::vector<std::vector<float>> matrix(3, std::vector<float>(3));
		temp[i] = new CMatrixInverseSuccessively(matrix);
	}
	for (int k = 0; k < 4; k++)
	{
		for (int i = 1; i < 4; i++)
		{
			int j1 = 0;
			for (int j = 0; j < 4; j++)
			{
				if (k == j)
					continue;
				temp[k]->m_baseMatrix[i - 1][j1++]
					= this->m_baseMatrix[i][j];
			}
		}
	}
	float det = this->m_baseMatrix[0][0] * temp[0]->GetDeterminant()
		- this->m_baseMatrix[0][1] * temp[1]->GetDeterminant()
		+ this->m_baseMatrix[0][2] * temp[2]->GetDeterminant()
		- this->m_baseMatrix[0][3] * temp[3]->GetDeterminant();
	return det;
}

float CMatrixInverseSuccessively::SearchDetThenMatrixSizeEqualFive()
{
	CMatrixInverseSuccessively *temp[5];
	for (int i = 0; i < 5; i++)
	{
		std::vector<std::vector<float>> matrix(4, std::vector<float>(4));
		temp[i] = new CMatrixInverseSuccessively(matrix);
	}
	for (int k = 0; k < 5; k++)
	{
		for (int i = 1; i < 5; i++)
		{
			int j1 = 0;
			for (int j = 0; j < 5; j++)
			{
				if (k == j)
					continue;
				temp[k]->m_baseMatrix[i - 1][j1++] = this->m_baseMatrix[i][j];
			}
		}
	}
	float det = this->m_baseMatrix[0][0] * temp[0]->GetDeterminant()
		- this->m_baseMatrix[0][1] * temp[1]->GetDeterminant()
		+ this->m_baseMatrix[0][2] * temp[2]->GetDeterminant()
		- this->m_baseMatrix[0][3] * temp[3]->GetDeterminant()
		+ this->m_baseMatrix[0][4] * temp[4]->GetDeterminant();
	return det;
}

CMatrixInverseSuccessively CMatrixInverseSuccessively::CoFactor()
{
	std::vector<std::vector<float>> cof(m_matrixSize, std::vector<float>(m_matrixSize));
	CMatrixInverseSuccessively cofactor(cof);
	if (m_matrixSize < 2)
		return cofactor;
	else if (m_matrixSize == 2)
	{
		cofactor.m_baseMatrix[0][0] = m_baseMatrix[1][1];
		cofactor.m_baseMatrix[0][1] = -m_baseMatrix[1][0];
		cofactor.m_baseMatrix[1][0] = -m_baseMatrix[0][1];
		cofactor.m_baseMatrix[1][1] = m_baseMatrix[0][0];
		return cofactor;
	}
	else if (m_matrixSize >= 3)
	{
		int DIM = m_matrixSize;
		CMatrixInverseSuccessively ***temp = new CMatrixInverseSuccessively**[DIM];
		for (int i = 0; i < DIM; i++)
			temp[i] = new CMatrixInverseSuccessively*[DIM];
		
		for (int i = 0; i < DIM; i++)
			for (int j = 0; j < DIM; j++)
			{
				std::vector<std::vector<float>> newMatrix(m_matrixSize - 1, std::vector<float>(m_matrixSize - 1));
				temp[i][j] = new CMatrixInverseSuccessively(newMatrix);
			}
		for (int k1 = 0; k1 < DIM; k1++)
		{
			for (int k2 = 0; k2 < DIM; k2++)
			{
				int i1 = 0;
				for (int i = 0; i < DIM; i++)
				{
					int j1 = 0;
					for (int j = 0; j < DIM; j++)
					{
						if (k1 == i || k2 == j)
							continue;
						temp[k1][k2]->m_baseMatrix[i1][j1++]	= this->m_baseMatrix[i][j];
					}
					if (k1 != i)
						i1++;
				}
			}
		}
		bool flagPositive = true;
		for (int k1 = 0; k1 < DIM; k1++)
		{
			flagPositive = ((k1 % 2) == 0);
			for (int k2 = 0; k2 < DIM; k2++)
			{
				if (flagPositive == true)
				{
					cofactor.m_baseMatrix[k1][k2] = temp[k1][k2]->GetDeterminant();
					flagPositive = false;
				}
				else
				{
					cofactor.m_baseMatrix[k1][k2] =- temp[k1][k2]->GetDeterminant();
					flagPositive = true;
				}
			}
		}
		for (int i = 0; i < DIM; i++)
			for (int j = 0; j < DIM; j++)
				delete temp[i][j];
		for (int i = 0; i < DIM; i++)
			delete[] temp[i];
		delete[] temp;
	}
	return cofactor;
}

std::vector<std::vector<float>> CMatrixInverseSuccessively::GetInverseMatrix()
{
	std::vector<std::vector<float>> cofM(m_matrixSize, std::vector<float>(m_matrixSize));
	std::vector<std::vector<float>> invM(m_matrixSize, std::vector<float>(m_matrixSize));
	CMatrixInverseSuccessively cofactorMatrix(cofM);
	CMatrixInverseSuccessively inverseMatrix(invM);
	// to find out Determinant
	float det = GetDeterminant();
	cofactorMatrix = this->CoFactor();
	// inv = transpose of cofactor / Determinant
	for (int i = 0; i < m_matrixSize; i++)
	{
		for (int j = 0; j < m_matrixSize; j++)
		{
			inverseMatrix.m_baseMatrix[j][i] = cofactorMatrix.m_baseMatrix[i][j] / det;
		}
	}
	return inverseMatrix.m_baseMatrix;
}

