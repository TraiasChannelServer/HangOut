#pragma once

/*
 * ��ʕ��i�F�G�f�B�b�g�e�L�X�g
 * �e�L�X�g���ύX���ꂽ�Ƃ��̃R�[���o�b�N�֐���
 * DelegateArgBase�N���X�ɂ��ēn��
 */

#include "ScLabel.h"
#include "DelegateArg.h"
#include <string>
#include <memory>

class ScEdit : public ScLabel
{
private:
	static constexpr int FRAME_WIDTH = 2;

public:
	// �w�i�ɐF������{�^���A���g�̋��E�͍D���ɐݒ�ł���
	// @param Text-FontHandle ���N���X(ScLabel)�ɓ���
	// @param ColorFrame      ���E���̐F
	// @param TextSize        ���͂ł���ő啶����
	// @param Callback        �e�L�X�g���ύX���ꂽ�Ƃ��̃R�[���o�b�N�֐�
	ScEdit(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, int FontHandle,
		unsigned int ColorFrame, size_t TextSize, DelegateArgBase<std::string>* Callback);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

protected:
	unsigned int m_ColorFrame;
	bool m_Hover;
	int m_PressCount;
	size_t m_TextSize;
	std::unique_ptr<DelegateArgBase<std::string>> m_Callback;
};
