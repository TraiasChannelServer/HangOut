#include "FontManager.h"


FontManager::FontManager()
	: m_FontHandle()
{
}

void FontManager::Initialize()
{
	Destroy();
	AddFont("ÇlÇr ÉSÉVÉbÉN", 16, Type::SMALL);
	AddFont("ÇlÇr ÉSÉVÉbÉN", 20, Type::MIDDLE);
	AddFont("ÇlÇr ÉSÉVÉbÉN", 32, Type::BIG);
	AddFont("ÇlÇr ÉSÉVÉbÉN", 64, Type::HUGE);
}

void FontManager::Destroy()
{
	for (auto& Pair : m_FontHandle)
	{
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
	m_FontHandle[type] = CreateFontToHandle(FontName, Size, -1);
}
