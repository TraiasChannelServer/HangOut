#pragma once

// An Embedded Engineer�fs Blog
// �Ƃ���g�ݍ��݃G���W�j�A�̔��Y�^�I�ȃu���O�ł��B
// C++�Ń��O�o��
// https://an-embedded-engineer.hateblo.jp/entry/2020/08/29/230546

#include "FormatToString.h"
#include "FileLogger.h"

#include <string>
#include <mutex>

#define LOGGER_ON

// ���̃}�N����ʂ���Logger�N���X�𗘗p����ƃ��O�o�͂�ON/OFF���\
#ifdef LOGGER_ON
#define LOG_INFO(...) Logger::Info(__VA_ARGS__)
#define LOG_WARN(...) Logger::Warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::Error(__VA_ARGS__)
#define LOG_BEGIN_END(...) Logger::AutoBeginEnd LoggerAutoBeginEnd(Logger::BeginEnd(__VA_ARGS__))
#else
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_BEGIN_END(...) 
#endif

class Logger
{
public:
	// Begin/End���O�����o�̓N���X
	// �I�u�W�F�N�g�̐������Ԃ𗘗p���Ďg��
	class AutoBeginEnd
	{
	public:
		AutoBeginEnd(const std::string& Message)
			: Message_(Message)
		{
			Logger::GetInstance().LogBegin(Message_);
		}

		~AutoBeginEnd()
		{
			Logger::GetInstance().LogEnd(Message_);
		}

	private:
		const std::string Message_;
	};

public:
	// �����w��Information���O
	template<typename ... Args>
	static void Info(const std::string& FormatString, Args&& ... args)
	{
		Logger::GetInstance().LogInfo(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

	// �����w��Warning���O
	template<typename ... Args>
	static void Warn(const std::string& FormatString, Args&& ... args)
	{
		Logger::GetInstance().LogWarn(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

	// �����w��Error���O 
	template<typename ... Args>
	static void Error(const std::string& FormatString, Args&& ... args)
	{
		Logger::GetInstance().LogError(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

	// �����w��BeginEnd�������O
	template<typename ... Args>
	static Logger::AutoBeginEnd BeginEnd(const std::string& FormatString, Args&& ... args)
	{
		return Logger::AutoBeginEnd(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

private:
	static Logger& GetInstance();

private:
	Logger();
	~Logger();

	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger& operator=(Logger&&) = delete;

private:
	void LogInfo(const std::string& Message);
	void LogWarn(const std::string& Message);
	void LogError(const std::string& Message);
	void LogBegin(const std::string& Message);
	void LogEnd(const std::string& Message);

	std::string GetNowTime();

private:
	std::mutex MyMutex;
	FileLogger MyFileLogger;
};

