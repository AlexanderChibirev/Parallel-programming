#pragma once
#include "Matrixs.h"

using Matrix = std::vector<std::vector<float>>;

class IMatrix
{
public:
	virtual Matrix GetInverseMatrix() = 0;
};