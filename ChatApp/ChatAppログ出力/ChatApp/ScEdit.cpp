#include "ScEdit.h"
#include "BoundRect.h"
#include "DxLib.h"

ScEdit::ScEdit(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, int FontHandle,
	unsigned int ColorFrame, size_t TextSize, DelegateArgBase<std::string>* Callback)
	: ScLabel(Text, ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, FontHandle)
	, m_ColorFrame(ColorFrame)
	, m_Hover()
	, m_PressCount()
	, m_TextSize(TextSize)
	, m_Callback(Callback)
{
	RECT r = m_Bound->GetRect();
	m_Bound = std::unique_ptr<BoundRect>(new BoundRect(r.left - FRAME_WIDTH, r.top - FRAME_WIDTH, r.right - r.left + FRAME_WIDTH * 2, r.bottom - r.top + FRAME_WIDTH * 2));
}

void ScEdit::OnMouseMove(int x, int y)
{
	m_Hover = m_Bound->IsIn(x, y);

	if (m_Hover && m_PressCount > 0)
	{
		RECT r = m_Bound->GetRect();
		DrawBox(r.left, r.top, r.right, r.bottom, m_ColorBack, TRUE);
		ScreenFlip();
		std::unique_ptr<char[]> Buf = std::unique_ptr<char[]>(new char[m_TextSize + 1]);
		int result = KeyInputString(m_TextX, m_TextY, m_TextSize, Buf.get(), TRUE);
		if (result == 1)
		{
			m_Text = Buf.get();
			(*m_Callback)(m_Text);
		}
	}

	m_PressCount = 0;
}

void ScEdit::OnMousePress(int x, int y)
{
	if (m_Bound->IsIn(x, y) == false)
	{
		m_Hover = false;
		m_PressCount = 0;
	}
	if (m_Hover)
	{
		++m_PressCount;
	}
}

void ScEdit::Draw() const
{
	RECT r = m_Bound->GetRect();
	DrawBox(r.left, r.top, r.right, r.bottom, m_ColorFrame, TRUE);
	DrawBox(r.left + FRAME_WIDTH, r.top + FRAME_WIDTH, r.right - FRAME_WIDTH, r.bottom - FRAME_WIDTH, m_ColorBack, TRUE);
	DrawStringToHandle(m_TextX, m_TextY, m_Text.c_str(), m_ColorFore, m_FontHandle);
}
