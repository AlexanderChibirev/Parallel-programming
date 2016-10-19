#pragma once
#include "Matrixs.h"

using Matrix = std::vector<std::vector<float>>;

static std::unique_ptr<MatrixData> matrix = std::make_unique<MatrixData>();

class IMatrix
{
public:
	virtual Matrix GetInverseMatrix() = 0;
};