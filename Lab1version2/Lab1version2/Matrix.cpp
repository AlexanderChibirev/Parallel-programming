#include "stdafx.h"
#include "Matrix.h"
#include <memory>


Matrix::Matrix(const int quantityStream)
	:m_threadQuantity(quantityStream)
{
	Start();
}

int Matrix::GetRank()
{
	return m_rank;
}


Matrix::~Matrix()
{
}

int Matrix::ProcessGetRank(row *matrix[MATRIX_SIZE]) {
	int rank = MATRIX_SIZE;
	std::vector<bool> line_used(MATRIX_SIZE);
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		int j;
		for (j = 0; j < MATRIX_SIZE; ++j)
			if (!line_used[j] && abs(matrix[j]->value[i]) > EPS)
				break;
		if (j == MATRIX_SIZE)
			--rank;
		else
		{
			line_used[j] = true;
			for (int p = i + 1; p< MATRIX_SIZE; ++p)// проходимся по линии
				matrix[j]->value[p] /= matrix[j]->value[i];
			for (int k = 0; k< MATRIX_SIZE; ++k)
				if (k != j && abs(matrix[k]->value[i]) > EPS)//и больше 1
					for (int p = i + 1; p< MATRIX_SIZE; ++p)
						matrix[k]->value[p] -= matrix[j]->value [p] * matrix[k]->value[i];
		}
	}
	return rank;
}

row Matrix::GenerateMatrixForOneThreads()
{
	row rows[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j< MATRIX_SIZE; j++)
		{
			rows[i].value[j] = (rand() % 10);
		}
	}
	return rows[MATRIX_SIZE];
}

void Matrix::Start()
{
	row rows[MATRIX_SIZE];
	int line = 0;
	int count = 0;
	HANDLE *hThreads = new HANDLE[m_threadQuantity];
	DWORD *dwThreadsId = new DWORD[m_threadQuantity];
	if (m_threadQuantity == 1) {
		ProcessGetRank(GenerateMatrixForOneThreads());
	}
	while (line < MATRIX_SIZE)
	{
		for (int i = 0; i < m_threadQuantity; ++i)
		{
			rows[count].rnd = rand();
			hThreads[count] = CreateThread(NULL, 0, &GenerateMatrix, &rows[count], 0, &dwThreadsId[count]);
			++count;
			++line;
			if (line == MATRIX_SIZE)
			{
				break;
			}
		}
		WaitForMultipleObjects(m_threadQuantity, hThreads, true, INFINITE);
	}

}
