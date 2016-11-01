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
	for (auto row : matrix)
	{
		for (auto column : row)
		{
			std::cout << " " << column;
		}
		std::cout << std::endl;
	}
}

int main(int argc, char * argv[])
{
//	size_t threadsCount = std::atoi(argv[1]);
	vector<vector<float>> matrix1;
	vector<vector<float>> matrix2;
	MatrixData matrixData;
	size_t threadsCount = 5;
	cout << "======================================" << endl;
	cout << "======== Parallel =========" << endl;
	unsigned int  start_time = clock();
	CMatrixParallel matrixParallel(threadsCount, matrixData);
	matrix1 = matrixParallel.GetInverseMatrix();
	Output(matrix1);
	unsigned int end_time = clock();
	unsigned int  search_time = end_time - start_time;
	cout << "time proccess Parallel: " << float(search_time) / 1000 << endl;
	cout << "=================================" << std::endl;
	cout << "======================================" << endl;
	cout << "======= Successively =======" << endl;
	start_time = clock();
	CMatrixSuccessively matrixSuccessively(matrixData.basicMatrix);
	matrix2 = matrixSuccessively.GetInverseMatrix();
	Output(matrix2);
	end_time = clock();
	search_time = end_time - start_time;
	cout << "time proccess Successively: " << float(search_time) / 1000 << endl << endl << endl;
	cout << "======================================" << endl;
	cout << "======= Inverse Matrix =======" << endl;
	return 0;
}

