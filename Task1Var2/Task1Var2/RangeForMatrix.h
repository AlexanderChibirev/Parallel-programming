#pragma once

using Matrix = std::vector<std::vector<float>>;

struct RangeForMatrix
{
	RangeForMatrix(size_t fromRow, size_t fromColumn, size_t toRow, size_t toColumn);
	size_t m_toRow;
	size_t m_toColumn;
	size_t m_fromRow;
	size_t m_fromColumn;
};