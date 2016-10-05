#pragma once

const int MATRIX_SIZE = 3;

const double EPS = 1E-9;

struct row
{
	int value[MATRIX_SIZE];
	int rnd;
};

static DWORD WINAPI GenerateMatrix(void *pvParam)
{
	row *r = (row *) pvParam;
	srand(r->rnd);
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		r->value[i] = (rand() % 10);
	}
	ExitThread(0);
	return 0;
}
