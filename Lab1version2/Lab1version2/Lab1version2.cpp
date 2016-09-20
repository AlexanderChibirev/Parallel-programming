// Lab1version2.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

const int MATRIX_SIZE = 3;


struct row
{
	int value[MATRIX_SIZE];
	int result;
	int rnd;
};

DWORD WINAPI generateAndCalc(void *data)
{
	//����������� ���������� ������ � ���� ���������
	row *r = (row *)data;
	//�������������� ��������� ��������� ����� ���������� ������
	srand(r->rnd);
	//���������� �������� ������
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		r->value[i] = rand() % 10;
	}
	//������� ������������ �������� ���������
	r->result = 1;
	for (int i = 0; i < MATRIX_SIZE; i += 2)
	{
		r->result *= r->value[i];
	}
	return 0;
}

int main()
{
	//�������������� ��������� ��������� �����
	srand(time(NULL));
	//���������� ����������� �������,
	//�������������� ������� � ��������� ��� ����� �������
	HANDLE thread[MATRIX_SIZE];
	DWORD thrId[MATRIX_SIZE];
	row rows[MATRIX_SIZE];

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		//���������� ��������� ����� ��� ������ ������
		rows[i].rnd = rand();
		//������� ������
		thread[i] = CreateThread(NULL, 0, &generateAndCalc, &rows[i], 0, &thrId[i]);
	}
	//����, ���� ��� ��� ������ ����������
	WaitForMultipleObjects(MATRIX_SIZE, thread, TRUE, INFINITE);
	//������� ��������� ������ ��������� �� �����
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			printf(" %d", rows[i].value[j]);
		}
		printf(" | multipl = %d\n", rows[i].result);
	}

	return 0;
}

