// Task1Var2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MatrixSuccessively.h"
#include "MatrixParallel.h"
#include "RangeForMatrix.h"
#include <ctime>



using namespace std;

void Output(Matrix const &matrix)
{
	for (auto el : matrix)
	{
		for (auto el2 : el)
		{
			std::cout << " " << el2;
		}
		std::cout << std::endl;
	}
}

int main(int argc, char * argv[])
{
//	size_t threadsCount = std::atoi(argv[1]);
	std::vector<std::vector<float>> matrix1;
	std::vector<std::vector<float>> matrix2;
	size_t threadsCount = 1;
	cout << "======================================" << endl;
	cout << "======== Parallel =========" << endl;
	unsigned int  start_time = clock();
	CMatrixParallel matrixParallel(threadsCount);
	matrix1 = matrixParallel.GetInverseMatrix();
	Output(matrix1);
	unsigned int end_time = clock();
	unsigned int  search_time = end_time - start_time;
	cout << "time proccess Parallel: " << float(search_time) / 1000 << endl;
	std::cout << "=====================================================" << std::endl;
	cout << "======================================" << endl;
	cout << "======= Successively =======" << endl;
	start_time = clock();
	CMatrixSuccessively matrixSuccessively(matrixParallel.m_matrix.base);
	matrix2 = matrixSuccessively.GetInverseMatrix();
	Output(matrix2);
	end_time = clock();
	search_time = end_time - start_time;
	cout << "time proccess Successively: " << float(search_time) / 1000 << endl << endl << endl;
	return 0;
}

