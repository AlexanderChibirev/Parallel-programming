#include "stdafx.h"
#include "MatrixParallel.h"
#include "IMatrix.h"


namespace
{
	DWORD WINAPI StartMultithreadedCalculator(PVOID pvParam)
	{
		auto data = static_cast<std::pair<CMatrixParallel*, int>*>(pvParam);
		data->first->GetDeterminantTread((PVOID)&data->second);
		return 0;
	}
}



CMatrixParallel::CMatrixParallel(const int quantityThread, std::vector<std::vector<float>> &matrix)
	:m_quantityThread(quantityThread),
	m_pData(matrix)
{
	m_matrixSize = matrix.size();
	m_inverseData.resize(m_matrixSize);
	for (int i = 0; i < m_matrixSize; i++)
	{
		m_inverseData[i].resize(m_matrixSize);
	}
	FillThreadsChargeMap();
}


DWORD CMatrixParallel::GetDeterminantTread(PVOID pvParam)
{
	auto threadNumber = *static_cast<int*>(pvParam);
	auto linesNumbers = m_threadsChargeMap.at(threadNumber);
	for (auto &it: linesNumbers)
	{
		for (size_t j = 0; j < m_matrixSize; ++j)
		{
			m_inverseData[j][it] = (m_coaf[it][j] / m_det);
		}
	}
	return 0;
}


std::vector<std::vector<float>> CMatrixParallel::GetInverseMatrix()
{
	HANDLE *hThreads = *std::make_shared<HANDLE*>(new HANDLE[m_quantityThread]);
	DWORD *dwThreadsId = *std::make_shared<DWORD*>(new DWORD[m_quantityThread]);
	std::vector<std::vector<float>> cofM(m_matrixSize, std::vector<float>(m_matrixSize));
	CMatrixParallel cofactorMatrix(m_quantityThread, cofM);
	m_det = GetDeterminant();
	cofactorMatrix = this->CoFactor();
	m_coaf = cofactorMatrix.m_pData;
	for (int i = 0; i < m_quantityThread; i++)
	{
		hThreads[i - 1] = CreateThread(NULL, 0, StartMultithreadedCalculator, (PVOID)&(std::make_pair(this, i)), 0, &dwThreadsId[i - 1]);
	}
	WaitForMultipleObjects(m_quantityThread, hThreads, TRUE, INFINITE);
	// to find out Determinant
	
	// inv = transpose of cofactor / Determinant
	return m_inverseData;
}

void CMatrixParallel::FillThreadsChargeMap()
{
	std::pair<int, int> matrixSize{ m_pData.size(), m_pData[0].size() };
	for (size_t i = 0; i <= m_quantityThread; i++)
	{
		m_threadsChargeMap.emplace(i, std::vector<size_t>());
	}
	for (size_t i = 0, threadNumber = 1; i != matrixSize.first; i++, threadNumber++)
	{
		if (threadNumber > m_quantityThread)
		{
			threadNumber = 1;
		}
		m_threadsChargeMap.at(threadNumber).push_back(i);
	}
}



DWORD CMatrixParallel::InverseColumnMatrix(void  *pvParam)
{
	ColumnInverse *r = (ColumnInverse *)pvParam;
	for (int column = 0; column < r->m_matrixSize; column++)
	{
		r->m_inverseMatrix[column][r->m_i] = r->m_cofactorMatrix[r->m_i][column] / r->m_det;
	}
	r->m_i++;
	return 0;
}


float CMatrixParallel::GetDeterminant()
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
		std::vector<CMatrixParallel> temp;
		for (int i = 0; i < DIM; i++)
		{
			std::vector<std::vector<float>> matrix(DIM - 1, std::vector<float>(DIM - 1));
			temp.push_back(CMatrixParallel(m_quantityThread, matrix));
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

DWORD CMatrixParallel::DeterminantProcessThenMatrixSizeMoreFive(PVOID pvParam)
{
	//DeterminantProcess *data = (DeterminantProcess *)pvParam;
	auto data = static_cast<std::pair<DeterminantProcess, int>*>(pvParam);
	int k = data->second;
	for (int i = 1; i < data->first.m_DIM; i++)
	{
		/*	int j1 = 0;
		for (int j = 0; j <  data->first.m_DIM; j++)
		{
		if (k == j)
		continue;
		int x = data->first.m_pData[i][j];
		data->first.m_temp[k].m_pData[i - 1][j1++] = x;
		}*/
	}
	return 0;
}

float CMatrixParallel::SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd)
{
	float h = pd[2][1];
	float i = pd[2][2];
	float det = (pd[0][0] * pd[1][1] * i + pd[0][1] * pd[1][2] * pd[2][0] + pd[0][2] * pd[1][0] * pd[2][1]);
	det = det - pd[0][0] * pd[1][2] * pd[2][1];
	det = det - pd[0][1] * pd[1][0] * i;
	det = det - pd[0][2] * pd[1][1] * pd[2][0];
	return det;
}

float CMatrixParallel::SearchDetThenMatrixSizeEqualFour()
{
	std::vector<CMatrixParallel> temp;
	for (int i = 0; i < 4; i++)
	{
		std::vector<std::vector<float>> matrix(3, std::vector<float>(3));
		temp.push_back(CMatrixParallel(m_quantityThread, matrix));
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
				temp[k].m_pData[i - 1][j1++] = this->m_pData[i][j];
			}
		}
	}
	float det = this->m_pData[0][0] * temp[0].GetDeterminant()
		- this->m_pData[0][1] * temp[1].GetDeterminant()
		+ this->m_pData[0][2] * temp[2].GetDeterminant()
		- this->m_pData[0][3] * temp[3].GetDeterminant();
	return det;
}

float CMatrixParallel::SearchDetThenMatrixSizeEqualFive()
{
	std::vector<CMatrixParallel> temp;
	for (int i = 0; i < 5; i++)
	{
		std::vector<std::vector<float>> matrix(4, std::vector<float>(4));
		temp.push_back(CMatrixParallel(m_quantityThread, matrix));
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
				temp[k].m_pData[i - 1][j1++] = this->m_pData[i][j];
			}
		}
	}
	float det = this->m_pData[0][0] * temp[0].GetDeterminant()
		- this->m_pData[0][1] * temp[1].GetDeterminant()
		+ this->m_pData[0][2] * temp[2].GetDeterminant()
		- this->m_pData[0][3] * temp[3].GetDeterminant()
		+ this->m_pData[0][4] * temp[4].GetDeterminant();
	return det;
}

DWORD CMatrixParallel::CalculateMatrixCofactors(PVOID pvParam)
{
	return 0;
}

CMatrixParallel CMatrixParallel::CoFactor()
{
	std::vector<std::vector<float>> cof(m_matrixSize, std::vector<float>(m_matrixSize));
	CMatrixParallel cofactor(m_quantityThread, cof);
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
		CMatrixParallel ***temp = new CMatrixParallel**[DIM];
		for (int i = 0; i < DIM; i++)
			temp[i] = new CMatrixParallel*[DIM];

		for (int i = 0; i < DIM; i++)
			for (int j = 0; j < DIM; j++)
			{
				std::vector<std::vector<float>> newMatrix(m_matrixSize - 1, std::vector<float>(m_matrixSize - 1));
				temp[i][j] = new CMatrixParallel(m_quantityThread, newMatrix);
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
						temp[k1][k2]->m_pData[i1][j1++] = this->m_pData[i][j];
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
					cofactor.m_pData[k1][k2] = -temp[k1][k2]->GetDeterminant();
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
