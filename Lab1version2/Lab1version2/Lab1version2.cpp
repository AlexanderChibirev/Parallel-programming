// Lab1version2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <ctime>

const int MATRIX_SIZE = 500;
const double EPS = 1E-9;

using namespace std;

struct row
{
	int value[MATRIX_SIZE];
	int rnd;
};

int GetRang(std::vector<vector<float>> &matrix)
{
	int rank = MATRIX_SIZE;
	vector<bool> line_used(MATRIX_SIZE);
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		int j;
		for (j = 0; j < MATRIX_SIZE; ++j)
			if (!line_used[j] && abs(matrix[j][i]) > EPS)
				break;
		if (j == MATRIX_SIZE)
			--rank;
		else 
		{
			line_used[j] = true;
			for (int p = i + 1; p< MATRIX_SIZE; ++p)
				matrix[j][p] /= matrix[j][i];
			for (int k = 0; k< MATRIX_SIZE; ++k)
				if (k != j && abs(matrix[k][i]) > EPS)
					for (int p = i + 1; p< MATRIX_SIZE; ++p)
						matrix[k][p] -= matrix[j][p] * matrix[k][i];
		}
	}
	return rank;
}

vector<vector<float>> GenerateMatrix()
{
	vector<vector<float>> matrix(MATRIX_SIZE);

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j< MATRIX_SIZE; j++)
		{
			matrix[i].push_back(rand() % 10);
			//cout << matrix[i][j] << '\t';
		}
		//cout << '\n';
	}
	return matrix;
}

DWORD WINAPI GenerateMatrix(void *data)
{
	row *r = (row *)data;
	srand(r->rnd);
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		r->value[i] = (rand() % 10);
	}
	return 0;
}

void StartProgrammWithoutMultithreading() 
{
	cout << "======================================" << endl;
	cout << "======= Without Multithreading =======" << endl;
	unsigned int start_time = clock();
	cout << "rang: " << GetRang(GenerateMatrix()) << endl;
	/////////////////////////////////////////////////
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	cout << "time proccess: " << float(search_time) / 1000 << endl << endl << endl;

	cout << "======================================" << endl;
	cout << "======== With Multithreading =========" << endl;
}
void StartProgrammWithMultithreading() 
{
	unsigned int  start_time = clock();
	srand(time(NULL));
	HANDLE thread[MATRIX_SIZE];
	DWORD thrId[MATRIX_SIZE];
	row rows[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		rows[i].rnd = rand();
		thread[i] = CreateThread(NULL, 0, &GenerateMatrix, &rows[i], 0, &thrId[i]);
	}
	WaitForMultipleObjects(MATRIX_SIZE, thread, TRUE, INFINITE);
	unsigned int end_time = clock();
	unsigned int  search_time = end_time - start_time;
	cout << "time proccess: " << float(search_time) / 1000 << endl;
}
//количество потоков, создаваемых одним процессом ограничено адресным пространством этого процесса ( 2 Гб ).
//Каждый поток имеет свой стек ( VC по умолчанию делает его равным 1Мб ). 
//Получается, что процесс сможет создать примерно 2000 потоков со стеком в 1Мб. Можно уменьшить величину стека ( опция линкера ). 
int main()
{
	////////////БЕЗ МНОГОПОТОЧНОСТИ//////////
	StartProgrammWithoutMultithreading();
	////////////C МНОГОПОТОЧНОСТЬЮ///////////
	StartProgrammWithMultithreading();
	return 0;
}

