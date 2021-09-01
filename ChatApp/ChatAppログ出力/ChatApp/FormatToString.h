#pragma once

// An Embedded Engineer�fs Blog
// �Ƃ���g�ݍ��݃G���W�j�A�̔��Y�^�I�ȃu���O�ł��B
// C++�ŕ�����t�H�[�}�b�e�B���O(�����t�������񐶐�)
// https://an-embedded-engineer.hateblo.jp/entry/2020/08/23/161317

#include <string>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>

namespace FormatToString
{
// std::string�^��const char*�ɕϊ�
template<typename T, typename std::enable_if<std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value>::type* = nullptr>
auto Convert(T&& value)
{
	return std::forward<T>(value).c_str();
}

// std::string�^�ȊO�́A���̂܂܏o��
template<typename T, typename std::enable_if<!std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value>::type* = nullptr>
auto Convert(T&& value)
{
	return std::forward<T>(value);
}

// ������̃t�H�[�}�b�e�B���O(��������)
template<typename ... Args>
std::string Make(const std::string& FormatString, Args&& ... args)
{
	// �t�H�[�}�b�g��̕��������Z�o
	int str_len = std::snprintf(nullptr, 0, FormatString.c_str(), args ...);

	// �t�H�[�}�b�g���s
	if (str_len < 0)
	{
		//throw std::runtime_error("String Formatting Error");
		return std::string("[ERR] String Formatting Failed: ").append(FormatString);
	}

	// �o�b�t�@�T�C�Y���Z�o(������ + null�����T�C�Y)
	size_t buffer_size = str_len + sizeof(char);

	// �o�b�t�@�T�C�Y���������m��
	std::unique_ptr<char[]> buffer(std::make_unique<char[]>(buffer_size));

	// ������̃t�H�[�}�b�g
	std::snprintf(buffer.get(), buffer_size, FormatString.c_str(), std::forward<Args>(args) ...);

	// �������std::string�^�ɕϊ����ďo��
	return std::string(buffer.get(), buffer.get() + str_len);
}
}

// ������̃t�H�[�}�b�e�B���O
template<typename ... Args>
std::string MakeStringFromFormat(const std::string& FormatString, Args&& ... args)
{
	// �e�p�����[�^�̌^��ϊ����āA������̃t�H�[�}�b�e�B���O
	return FormatToString::Make(FormatString, FormatToString::Convert(std::forward<Args>(args)) ...);
}