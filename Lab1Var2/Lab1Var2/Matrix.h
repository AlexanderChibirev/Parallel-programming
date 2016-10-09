#pragma once

using Matrix = std::vector<std::vector<int>>;

void Output(Matrix const &matrix);

struct SMatrixs
{
	SMatrixs()
	{
		int size = 3;
		basicMatrix.resize(size);
		for (size_t i = 0; i != size; ++i)
		{
			basicMatrix[i].resize(size);
			for (size_t j = 0; j != size; ++j)
			{
				basicMatrix[i][j] = rand() % 10;
			}
		}
		/*basicMatrix = { { 6, 5, 4, 11, 12, 8 }
		,{ 3, 21, 14, 15, 1, 6 }
		,{ 14, 8, 9, 3, 4, 15 }
		,{ 4, 6, 9, 6, 7, 4, 5 }
		,{ 13, 7, 8, 4, 6, 9 }
		,{ 15, 11, 10, 8, 7, 6 } };*/
		minorsMatrix.resize(size);
		for (size_t i = 0; i != size; ++i)
		{
			minorsMatrix[i].resize(size);
		}
		copyBasicMatrix = basicMatrix;
	}
	Matrix basicMatrix;
	Matrix minorsMatrix;
	Matrix copyBasicMatrix;
};

struct SMiniMatrix
{
	SMiniMatrix(int fromX = 0, int fromY = 0, int toX = 0, int toY = 0)
		: to_x(toX)
		, to_y(toY)
		, from_x(fromX)
		, from_y(fromY)
	{}
	int to_x;
	int to_y;
	int from_x;
	int from_y;
};



class IMatrixHelper
{
public:
	virtual Matrix GetTransposedMatrix() = 0;
};


class CMatrixHelperParallel : IMatrixHelper
{
public:
	CMatrixHelperParallel(int numberThreads);
	Matrix GetTransposedMatrix() override;

private:
	void CalculateMatrixCofactors();
	void CalculateMatrixMinors();
	void CalculateTransposedMatrix();

	static DWORD WINAPI CalculateMatrixCofactors(PVOID pvParam);
	static DWORD WINAPI CalculateMatrixMinors(PVOID pvParam);
	static DWORD WINAPI CalculateTransposedMatrix(PVOID pvParam);
	//static int CalculateDeterminantMatrix(Matrix matrix, int result = 0, int count = 1);
private:
	int m_numberThreads;
};

class CMatrixHelperSuccessively : IMatrixHelper
{
public:
	CMatrixHelperSuccessively() = default;
	Matrix GetTransposedMatrix() override;
private:
	Matrix GetMatrixCofactors(Matrix matrix);
	Matrix GetTransposedMatrix(Matrix const &matrix);
	Matrix GetMatrixMinors(Matrix matrix);
	//int GetDeterminantMatrix(Matrix matrix, int result = 0, int count = 1);
};