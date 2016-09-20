// Lab1version2.cpp: определяет точку входа для консольного приложения.
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
	//преобразуем полученные данные к типу структуры
	row *r = (row *)data;
	//инициализируем генератор случайных чисел полученным числом
	srand(r->rnd);
	//генерируем элементы строки
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		r->value[i] = rand() % 10;
	}
	//находим произведение нечетных элементов
	r->result = 1;
	for (int i = 0; i < MATRIX_SIZE; i += 2)
	{
		r->result *= r->value[i];
	}
	return 0;
}

int main()
{
	//инициализируем генератор случайных чисел
	srand(time(NULL));
	//определяем дескрипторы потоков,
	//идентификаторы потоков и структуры для строк матрицы
	HANDLE thread[MATRIX_SIZE];
	DWORD thrId[MATRIX_SIZE];
	row rows[MATRIX_SIZE];

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		//генерируем случайные числа для каждой строки
		rows[i].rnd = rand();
		//создаем потоки
		thread[i] = CreateThread(NULL, 0, &generateAndCalc, &rows[i], 0, &thrId[i]);
	}
	//ждем, пока все эти потоки завершатся
	WaitForMultipleObjects(MATRIX_SIZE, thread, TRUE, INFINITE);
	//выводим результат работы программы на экран
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

