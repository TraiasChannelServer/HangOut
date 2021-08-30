#pragma once

/*
 * ��ʕ��i�F���x��
 * �e�L�X�g��\������̂�
 * �}�E�X����ɂ͔������Ȃ�
 */

#include "ScreenComponent.h"
#include "TextAlignment.h"
#include <string>

class ScLabel : public ScreenComponent
{
public:
	// �w�i�������̃��x���A���g�̋��E�͎����ݒ�
	ScLabel(const std::string& Text, unsigned int ColorFore, int x, int y, int FontHandle);

	// �w�i�ɐF�����郉�x���A���g�̋��E�͎����ݒ�
	ScLabel(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, int x, int y, int FontHandle);

	// �w�i�ɐF�����郉�x���A���g�̋��E�͍D���ɐݒ�ł���
	// @param Text       ������
	// @param ColorFore  �����F
	// @param ColorBack  �w�i�F
	// @param bound      ���E
	// @param gravity    ���񂹂Ƃ����������Ƃ�
	// @param FontHandle �t�H���g
	ScLabel(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle);

	virtual ~ScLabel() = default;

public:
	virtual void OnMouseMove(int x, int y) override;
	virtual void OnMousePress(int x, int y) override;
	virtual void Draw() const override;

	// �������ύX����
	// m_FitBoundMode �ɂ���ċ��E�͒��������
	void ChangeText(const std::string& Text);

	const std::string& GetText() const;

protected:
	// ���g�̋��E�𕶎���Ƀt�B�b�g������
	void FitBound();

protected:
	std::string m_Text;
	unsigned int m_ColorFore;
	unsigned int m_ColorBack;
	bool m_BkColorTransparent;
	TextAlignment::Gravity m_Gravity;
	int m_TextX;
	int m_TextY;
	int m_FontHandle;

	// ���g�̋��E�𕶎���Ƀt�B�b�g������̂��ۂ��A�Ă΂ꂽ�R���X�g���N�^�ɂ���ČŒ�
	bool m_FitBoundMode;
};

inline const std::string& ScLabel::GetText() const
{
	return m_Text;
}