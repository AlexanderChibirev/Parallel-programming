#pragma once
#include "Matrixs.h"

using Matrix = std::vector<std::vector<float>>;

static std::unique_ptr<SMatrixs> matrix = std::make_unique<SMatrixs>();

class IMatrix
{
public:
	virtual Matrix GetInverseMatrix() = 0;
};