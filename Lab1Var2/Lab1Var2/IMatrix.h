#pragma once
#include <vector>
#include <memory>

using Matrix = std::vector<std::vector<float>>;

class IMatrix
{
public:
	virtual std::vector<std::vector<float>> GetInverseMatrix() = 0;
};
