#pragma once

// An Embedded Engineer�fs Blog
// �Ƃ���g�ݍ��݃G���W�j�A�̔��Y�^�I�ȃu���O�ł��B
// C++�Ń��O�o��
// https://an-embedded-engineer.hateblo.jp/entry/2020/08/29/230546

#include <fstream>

class FileLogger
{
public:
	FileLogger();
	FileLogger(const std::string& FilePath);
	~FileLogger();

	void Open(const std::string& FilePath);
	void Close();
	bool IsOpen();
	void Write(const std::string& Log);

private:
	std::string FilePath;
	std::ofstream Stream;
};

