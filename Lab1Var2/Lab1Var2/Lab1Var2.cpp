// Lab1Var2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "MatrixSuccessively.h"
#include "MatrixParallel.h"

#include <ctime>


using namespace std;
const int MATRIX_SIZE = 3;


void FillSimulatedInput(std::vector<std::vector<float>> &matrix)
{
	for (auto &row : matrix)
	{
		for (auto &column : row)
		{
			column = rand() % 10;
		}
	}
}

void OutputMatrix(std::vector<std::vector<float>> &matrix)
{
	for (auto &row : matrix)
	{
		for (auto &column : row)
		{
			std::cout << " " << column;
		}
		std::cout << std::endl;
	}
}

void StartProgrammWithoutMultithreading(std::vector<std::vector<float>> &matrix)
{
	cout << "======================================" << endl;
	cout << "======= Without Multithreading =======" << endl;
	unsigned int start_time = clock();
	CMatrixSuccessively matrixSuccessively(matrix);
	OutputMatrix(matrixSuccessively.GetInverseMatrix());
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	cout << "time proccess: " << float(search_time) / 1000 << endl << endl << endl;
}

void StartProgrammWithMultithreading(const int quantityStream, std::vector<std::vector<float>> &matrix)
{
	cout << "======================================" << endl;
	cout << "======== With Multithreading =========" << endl;
	unsigned int  start_time = clock();
	CMatrixParallel matrixWithMultithreading(quantityStream, matrix);
	OutputMatrix(matrixWithMultithreading.GetInverseMatrix());
	unsigned int end_time = clock();
	unsigned int  search_time = end_time - start_time;
	cout << "time proccess: " << float(search_time) / 1000 << endl;
}

int main()
{
	std::vector<std::vector<float>> matrix(MATRIX_SIZE, std::vector<float>(MATRIX_SIZE));
	FillSimulatedInput(matrix);
	////////////БЕЗ МНОГОПОТОЧНОСТИ//////////
	StartProgrammWithoutMultithreading(matrix);
	////////////C МНОГОПОТОЧНОСТЬЮ///////////
	const int quantityStream = 2;
	StartProgrammWithMultithreading(quantityStream, matrix);
    return 0;
}
