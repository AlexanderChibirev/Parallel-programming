#pragma once

using Matrix = std::vector<std::vector<float>>;

void Output(Matrix const &matrix);
void Erase(Matrix &matrix, size_t i, size_t j);
void Resize(Matrix &matrix, int number);
float GetDeterminantMatrix(Matrix matrix, float result = 0.f, int count = 1);

struct SMiniMatrix
{
	SMiniMatrix(size_t fromX = 0, size_t fromY = 0, size_t toX = 0, size_t toY = 0)
		: to_x(toX)
		, to_y(toY)
		, from_x(fromX)
		, from_y(fromY)
	{}
	size_t to_x;
	size_t to_y;
	size_t from_x;
	size_t from_y;
};