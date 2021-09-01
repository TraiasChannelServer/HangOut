#include "Logger.h"
#include <Windows.h>

Logger& Logger::GetInstance()
{
	static Logger Instance;
	return Instance;
}

Logger::Logger()
	: MyMutex()
	, MyFileLogger()
{
	time_t NowTime = time(nullptr);
	tm DateTime = {};
	errno_t e = localtime_s(&DateTime, &NowTime);
	std::string FilePath = MakeStringFromFormat("Logs\\Log_%04d%02d%02d_%02d%02d%02d.txt", DateTime.tm_year + 1900, DateTime.tm_mon + 1, DateTime.tm_mday, DateTime.tm_hour, DateTime.tm_min, DateTime.tm_sec);
	MyFileLogger.Open(FilePath);
}

Logger::~Logger()
{
	MyFileLogger.Close();
}

void Logger::LogInfo(const std::string& Message)
{
	// �~���[�e�b�N�X�ɂ��r������
	std::lock_guard<std::mutex> Lock(MyMutex);

	// Information���O�𐶐�
	std::string LogMessage = MakeStringFromFormat("%s [INF] %s", GetNowTime(), Message);

	// Information���O���t�@�C���ɏo��
	MyFileLogger.Write(LogMessage);
}

void Logger::LogWarn(const std::string& Message)
{
	std::lock_guard<std::mutex> Lock(MyMutex);
	std::string LogMessage = MakeStringFromFormat("%s [WRN] %s", GetNowTime(), Message);
	MyFileLogger.Write(LogMessage);
}

void Logger::LogError(const std::string& Message)
{
	std::lock_guard<std::mutex> Lock(MyMutex);
	std::string LogMessage = MakeStringFromFormat("%s [ERR] %s", GetNowTime(), Message);
	MyFileLogger.Write(LogMessage);
}

void Logger::LogBegin(const std::string& Message)
{
	std::lock_guard<std::mutex> Lock(MyMutex);
	std::string LogMessage = MakeStringFromFormat("%s [BGN] %s", GetNowTime(), Message);
	MyFileLogger.Write(LogMessage);
}

void Logger::LogEnd(const std::string& Message)
{
	std::lock_guard<std::mutex> Lock(MyMutex);
	std::string LogMessage = MakeStringFromFormat("%s [END] %s", GetNowTime(), Message);
	MyFileLogger.Write(LogMessage);
}

std::string Logger::GetNowTime()
{
	SYSTEMTIME SystemTime = {};
	GetSystemTime(&SystemTime);
	return MakeStringFromFormat("%02d:%02d:%02d.%03d", SystemTime.wHour + 9, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
}

