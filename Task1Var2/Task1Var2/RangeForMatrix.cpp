#include "stdafx.h"
#include "RangeForMatrix.h"
#include "Matrixs.h"

using namespace std;

RangeForMatrix::RangeForMatrix(size_t fromRow, size_t fromColumn, size_t toRow, size_t toColumn)
	: m_toRow(toRow)
	, m_toColumn(toColumn)
	, m_fromRow(fromRow)
	, m_fromColumn(fromColumn)
{

}
