#include "stdafx.h"

#include "../utils/Helper.h"
#include "CreaterPipe.h"


CCreaterPipe::CCreaterPipe(size_t procNumber, size_t itNumber)
	: m_procNumber(procNumber)
	, m_itNumber(itNumber)

{
}

void CCreaterPipe::StartTransfer()
{
	int failedProc = 0;
	for (size_t i = 0; i != m_procNumber; ++i)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		std::string commandLine = "Task3.exe " + std::to_string(m_itNumber) + " " + std::to_string(i + 1) + " pipe";
		SettingProcess(si);

		if (!CreateProcess(NULL, (LPSTR)commandLine.data(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			++failedProc;
			std::cout << "Could't create a process." << std::endl;
			continue;
		}


		m_startUpInfos.push_back(si);
		m_processesInformation.push_back(pi);
	}
	m_procNumber -= failedProc;

	WaitMessages();
}

void CCreaterPipe::SettingProcess(STARTUPINFO &si)
{
	si.dwFlags = STARTF_USESIZE | STARTF_USESHOWWINDOW;
	si.dwXSize = 500;
	si.dwYSize = 300;
	si.wShowWindow = SW_SHOW;
}

CCreaterPipe::~CCreaterPipe()
{
	for (size_t i = 0; i != m_procNumber; ++i)
	{
		CloseHandle(m_processesInformation[i].hProcess);
		CloseHandle(m_processesInformation[i].hThread);
	}
}

void CCreaterPipe::WaitMessages()
{
	auto start = std::chrono::system_clock::now();
	std::vector<std::string> messages;
	WaitSend(messages, m_procNumber);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time : " << diff.count() << std::endl;
	for (auto const &mes : messages)
	{
		std::cout << mes << std::endl;
	}
}

void CCreaterPipe::WaitSend(std::vector<std::string>& messages, size_t messageNumber)
{
	HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\myPipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		1024, 1024, 5000, NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed" << std::endl;
		return;
	}

	size_t innerCount = 0;
	while (innerCount < messageNumber)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			char buffer[1024];
			DWORD dwRead;
			if (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) == TRUE)
			{
				buffer[dwRead] = '\0';
			}
			messages.push_back(buffer);
			++innerCount;

		}
		DisconnectNamedPipe(hPipe);
	}
	CloseHandle(hPipe);
}


