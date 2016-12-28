#pragma once


class CCreaterPipe
{
public:
	CCreaterPipe(size_t procNumber, size_t itNumber);
	void StartTransfer();
	~CCreaterPipe();
private:
	void WaitMessages();
	void SettingProcess(STARTUPINFO &si);
	void CCreaterPipe::WaitSend(std::vector<std::string>& messages, size_t messageNumber);
private:
	size_t m_procNumber;
	size_t m_itNumber;
	std::vector<STARTUPINFO> m_startUpInfos;
	std::vector<PROCESS_INFORMATION> m_processesInformation;
};