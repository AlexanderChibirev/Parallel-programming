#pragma once

using Matrix = std::vector<std::vector<float>>;

struct SMatrixs
{
	SMatrixs(std::vector<std::vector<float>> matrix);
	SMatrixs();
	Matrix basicMatrix;
	Matrix minorsMatrix;
	Matrix copyBasicMatrix;
	Matrix base;
};