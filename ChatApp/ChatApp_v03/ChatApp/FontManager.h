#pragma once

/*
 * �V���O���g��
 * �A�v���S�̂ŋ��L����t�H���g�n���h�����Ǘ�����
 */

#include <map>
#include "DxLib.h"

class FontManager
{
public:
	enum class Type
	{
		SMALL,
		MIDDLE,
		BIG,
		HUGE
	};

public:
	static FontManager& Inst();

private:
	FontManager();
	FontManager(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager& operator=(FontManager&&) = delete;

public:
	void Initialize();
	void Destroy();
	int GetFontHandle(Type type);

private:
	void AddFont(const char* FontName, int Size, Type type);

private:
	std::map<Type, int> m_FontHandle;
};

inline FontManager& FontManager::Inst()
{
	static FontManager s_Inst;
	return s_Inst;
}

