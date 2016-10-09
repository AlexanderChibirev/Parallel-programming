#include "stdafx.h"
#include "MatrixSuccessively.h"




CMatrixSuccessively::CMatrixSuccessively(std::vector<std::vector<float>> matrix)
	:m_pData(matrix)
{
		m_matrixSize = matrix.size();
}


float CMatrixSuccessively::GetDeterminant() 
{
	float det = 0;
	std::vector<std::vector<float>> pd = m_pData;
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
		std::vector<CMatrixSuccessively> temp;
		for (int i = 0; i < DIM; i++)
		{
			std::vector<std::vector<float>> matrix(DIM - 1, std::vector<float>(DIM - 1));
			temp.push_back(CMatrixSuccessively(matrix));
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
					temp[k].m_pData[i - 1][j1++] = this->m_pData[i][j];
				}
			}
		}
		float det = 0;
		for (int k = 0; k < DIM; k++)
		{
			if ((k % 2) == 0)
				det = det + (this->m_pData[0][k] * temp[k].GetDeterminant());
			else
				det = det - (this->m_pData[0][k] * temp[k].GetDeterminant());
		}
		return det;
	}
	break;
	}
}

float CMatrixSuccessively::SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd)
{
	float a = pd[0][0];
	float b = pd[0][1];
	float c = pd[0][2];
	float d = pd[1][0];
	float e = pd[1][1];
	float f = pd[1][2];
	float g = pd[2][0];
	float h = pd[2][1];
	float i = pd[2][2];
	float det = (a * e * i + b * f * g + c * d * h);
	det = det - a * f * h;
	det = det - b * d * i;
	det = det - c * e * g;
	return det;
}

float CMatrixSuccessively::SearchDetThenMatrixSizeEqualFour()
{
	CMatrixSuccessively *temp[4];
	for (int i = 0; i < 4; i++)
	{
		std::vector<std::vector<float>> matrix(3, std::vector<float>(3));
		temp[i] = new CMatrixSuccessively(matrix);
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
				temp[k]->m_pData[i - 1][j1++]
					= this->m_pData[i][j];
			}
		}
	}
	float det = this->m_pData[0][0] * temp[0]->GetDeterminant()
		- this->m_pData[0][1] * temp[1]->GetDeterminant()
		+ this->m_pData[0][2] * temp[2]->GetDeterminant()
		- this->m_pData[0][3] * temp[3]->GetDeterminant();
	return det;
}

float CMatrixSuccessively::SearchDetThenMatrixSizeEqualFive()
{
	CMatrixSuccessively *temp[5];
	for (int i = 0; i < 5; i++)
	{
		std::vector<std::vector<float>> matrix(4, std::vector<float>(4));
		temp[i] = new CMatrixSuccessively(matrix);
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
				temp[k]->m_pData[i - 1][j1++] = this->m_pData[i][j];
			}
		}
	}
	float det = this->m_pData[0][0] * temp[0]->GetDeterminant()
		- this->m_pData[0][1] * temp[1]->GetDeterminant()
		+ this->m_pData[0][2] * temp[2]->GetDeterminant()
		- this->m_pData[0][3] * temp[3]->GetDeterminant()
		+ this->m_pData[0][4] * temp[4]->GetDeterminant();
	return det;
}

CMatrixSuccessively CMatrixSuccessively::CoFactor()
{
	std::vector<std::vector<float>> cof(m_matrixSize, std::vector<float>(m_matrixSize));
	CMatrixSuccessively cofactor(cof);
	if (m_matrixSize < 2)
		return cofactor;
	else if (m_matrixSize == 2)
	{
		cofactor.m_pData[0][0] = m_pData[1][1];
		cofactor.m_pData[0][1] = -m_pData[1][0];
		cofactor.m_pData[1][0] = -m_pData[0][1];
		cofactor.m_pData[1][1] = m_pData[0][0];
		return cofactor;
	}
	else if (m_matrixSize >= 3)
	{
		int DIM = m_matrixSize;
		CMatrixSuccessively ***temp = new CMatrixSuccessively**[DIM];
		for (int i = 0; i < DIM; i++)
			temp[i] = new CMatrixSuccessively*[DIM];
		
		for (int i = 0; i < DIM; i++)
			for (int j = 0; j < DIM; j++)
			{
				std::vector<std::vector<float>> newMatrix(m_matrixSize - 1, std::vector<float>(m_matrixSize - 1));
				temp[i][j] = new CMatrixSuccessively(newMatrix);
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
						temp[k1][k2]->m_pData[i1][j1++]	= this->m_pData[i][j];
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
					cofactor.m_pData[k1][k2] = temp[k1][k2]->GetDeterminant();
					flagPositive = false;
				}
				else
				{
					cofactor.m_pData[k1][k2] =- temp[k1][k2]->GetDeterminant();
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

std::vector<std::vector<float>> CMatrixSuccessively::GetInverseMatrix()
{
	std::vector<std::vector<float>> cofM(m_matrixSize, std::vector<float>(m_matrixSize));
	std::vector<std::vector<float>> invM(m_matrixSize, std::vector<float>(m_matrixSize));
	CMatrixSuccessively cofactorMatrix(cofM);
	CMatrixSuccessively inverseMatrix(invM);
	// to find out Determinant
	float det = GetDeterminant();
	cofactorMatrix = this->CoFactor();
	// inv = transpose of cofactor / Determinant
	for (int i = 0; i < m_matrixSize; i++)
	{
		for (int j = 0; j < m_matrixSize; j++)
		{
			inverseMatrix.m_pData[j][i] = cofactorMatrix.m_pData[i][j] / det;
		}
	}
	return inverseMatrix.m_pData;
}

