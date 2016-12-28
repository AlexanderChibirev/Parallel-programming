#include "stdafx.h"

#include "../utils/Helper.h"


using namespace std;

namespace
{
	const float radius = 10.f;

	void TransmitDataOnServer(size_t iterationsNumber, double Pi, std::string const &procNumber)
	{
		std::string message = "procNumber: " + procNumber + ", iterationsNumber: " + std::to_string(iterationsNumber) + ", Pi: " + std::to_string(Pi);
		//std::cout << message;
		HANDLE hPipe;
		do {
			hPipe = CreateFile(TEXT("\\\\.\\pipe\\myPipe"),
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
		} while (hPipe == INVALID_HANDLE_VALUE);

		DWORD dwWritten = static_cast<DWORD>(message.size());
		if (hPipe != INVALID_HANDLE_VALUE)
		{
			while (WriteFile(hPipe,
				message.data(),
				static_cast<DWORD>(message.size()),
				&dwWritten,
				NULL) == FALSE)
			{
			}

			CloseHandle(hPipe);
		}
	}



	double GetNumberPi(size_t iterationsNumber)
	{
		size_t Nmax = iterationsNumber;
		double Ncirc = 0;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(0.f, radius);

		for (size_t i = 0; i < Nmax; ++i)
		{
			double x = dist(gen);
			double y = dist(gen);
			if (pow(x, 2) + pow(y, 2) <= pow(radius, 2))
			{
				++Ncirc;
			}
		}

		double Pi = (Ncirc / Nmax) * 4;

		//std::cout << Nmax << ", " << Pi << std::endl;
		//std::cout << "Pi = " << Pi << std::endl;
		return Pi;
	}


}

int main(int argc, char *argv[])
{
	std::string procNumber = std::string(argv[1]);
	size_t iterationsNumber = atoi(argv[2]);
	TransmitDataOnServer(iterationsNumber, GetNumberPi(iterationsNumber), procNumber);
}