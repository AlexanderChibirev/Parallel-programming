#pragma once

using Matrix = std::vector<std::vector<float>>;

struct MatrixData
{
	MatrixData();
	Matrix basicMatrix;
	Matrix minorsMatrix;
	Matrix copyBasicMatrix;
};