#pragma once
#include <vector>

class IMatrix
{
public:
	virtual std::vector<std::vector<float>> GetInverseMatrix() = 0;
};