// Task1Var2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MatrixSuccessively.h"
#include "MatrixParallel.h"
#include "MiniMatrix.h"
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


void StartProgrammWithoutMultithreading(std::vector<std::vector<float>> &matrix)
{
	cout << "======================================" << endl;
	cout << "======= Without Multithreading =======" << endl;
	unsigned int start_time = clock();


	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	cout << "time proccess: " << float(search_time) / 1000 << endl << endl << endl;
}

void StartProgrammWithMultithreading(const int threadsCount, std::vector<std::vector<float>> &matrix)
{
	cout << "======================================" << endl;
	cout << "======== With Multithreading =========" << endl;
	unsigned int  start_time = clock();


	unsigned int end_time = clock();
	unsigned int  search_time = end_time - start_time;
	cout << "time proccess: " << float(search_time) / 1000 << endl;
}


int main(int argc, char * argv[])
{
//	size_t threadsCount = std::atoi(argv[1]);
	size_t threadsCount = 1;
	CMatrixParallel matrixParallel(threadsCount);
	Output(matrixParallel.GetInverseMatrix());
	std::cout << "=====================================================" << std::endl;
	CMatrixSuccessively matrixSuccessively(matrixParallel.m_matrix.base);
	Output(matrixSuccessively.GetInverseMatrix());
	return 0;
}

