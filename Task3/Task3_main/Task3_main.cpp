#include "stdafx.h"

#include "CreaterPipe.h"

void Helper()
{
	std::cout << "Error! Usage program.exe <processes number> <iterations number>" << std::endl;
	std::cout << "for call helper usage program.exe </?> " << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		Helper();
		return 1;
	}
	try
	{
		size_t procNumber = atoi(argv[1]);
		size_t  itNumber =  atoi(argv[2]);
		CCreaterPipe pipe(procNumber, itNumber);
		pipe.StartTransfer();
		return 0;
	}
	catch(std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

