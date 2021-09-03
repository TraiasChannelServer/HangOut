#include "FontManager.h"
#include "Logger.h"


FontManager::FontManager()
	: m_FontHandle()
{
}

void FontManager::Initialize()
{
	Destroy();
	AddFont("�l�r �S�V�b�N", 16, Type::SMALL);
	AddFont("�l�r �S�V�b�N", 20, Type::MIDDLE);
	AddFont("�l�r �S�V�b�N", 32, Type::BIG);
	AddFont("�l�r �S�V�b�N", 64, Type::HUGE);
}

void FontManager::Destroy()
{
	for (auto& Pair : m_FontHandle)
	{
		Logger::Info("�t�H���g�j���Ftype = %d", static_cast<int>(Pair.first));
		DeleteGraph(Pair.second);
	}
	m_FontHandle.clear();
}

int FontManager::GetFontHandle(Type type)
{
	return m_FontHandle.at(type);
}

void FontManager::AddFont(const char* FontName, int Size, Type type)
{
	Logger::Info("�t�H���g�ǉ��Ftype = %d", static_cast<int>(type));
	m_FontHandle[type] = CreateFontToHandle(FontName, Size, -1);
}
