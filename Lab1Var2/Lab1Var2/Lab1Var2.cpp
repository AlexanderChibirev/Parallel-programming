// Lab1Var2.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

#include "Matrix.h"
#include "MatrixSuccessively.h"
#include "MatrixParallel.h"

#include <ctime>


using namespace std;
const int MATRIX_SIZE = 6;


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
	////////////��� ���������������//////////
	StartProgrammWithoutMultithreading(matrix);
	////////////C ����������������///////////
	const int quantityStream = 2;
	StartProgrammWithMultithreading(quantityStream, matrix);
    return 0;
}
