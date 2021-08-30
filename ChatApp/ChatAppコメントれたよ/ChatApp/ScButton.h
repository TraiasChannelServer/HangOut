#pragma once

/*
 * ��ʕ��i�F�{�^��
 * �����ꂽ���̃R�[���o�b�N�֐���
 * DelegateVoidBase�N���X�ɂ��ēn��
 */

#include "ScLabel.h"
#include "DelegateVoid.h"
#include <string>
#include <memory>

class ScButton : public ScLabel
{
private:
	static constexpr int FRAME_WIDTH_RELEASE = 2;
	static constexpr int FRAME_WIDTH_PRESS = 3;

public:
	// �w�i�ɐF������{�^���A���g�̋��E�͎����ݒ�
	ScButton(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, int x, int y, int FontHandle,
		unsigned int ColorFrame, unsigned int ColorBackHover, unsigned int ColorBackPress, DelegateVoidBase* Callback);

	// �w�i�ɐF������{�^���A���g�̋��E�͍D���ɐݒ�ł���
	// @param Text-FontHandle ���N���X(ScLabel)�ɓ���
	// @param ColorFrame      ���E���̐F
	// @param ColorBackHover  �}�E�X���d�˂��Ƃ��̔w�i�F
	// @param ColorBackPress  �}�E�X�������Ă���Ƃ��̔w�i�F
	// @param Callback        �{�^�����������Ƃ��̃R�[���o�b�N�֐�
	ScButton(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle,
		unsigned int ColorFrame, unsigned int ColorBackHover, unsigned int ColorBackPress, DelegateVoidBase* Callback);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

protected:
	unsigned int m_ColorFrame;
	unsigned int m_ColorBackHover;
	unsigned int m_ColorBackPress;

	// �}�E�X���d�Ȃ��Ă��邩�A�N���b�N����Ă邩��
	// ���m���邽�߂̕ϐ�
	bool m_Hover;
	int m_PressCount;

	std::unique_ptr<DelegateVoidBase> m_Callback;
};
