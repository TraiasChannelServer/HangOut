#pragma once

/*
 * ��ʕ��i�F�g�O��(ON-OFF�X�C�b�`)
 * ON-OFF���ύX���ꂽ�Ƃ��̃R�[���o�b�N�֐���
 * DelegateArgBase�N���X�ɂ��ēn��
 */

#include "ScLabel.h"
#include "DelegateArg.h"
#include <string>
#include <memory>

class ScToggle : public ScLabel
{
public:
	// �w�i�ɐF������{�^���A���g�̋��E�͍D���ɐݒ�ł���
	// @param ColorFore-FontHandle ���N���X(ScLabel)�ɓ���
	// @param Callback             ON-OFF���ύX���ꂽ�Ƃ��̃R�[���o�b�N�֐�
	ScToggle(unsigned int ColorFore, unsigned int ColorBack, Bound* bound, int FontHandle,
		DelegateArgBase<bool>* Callback);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

protected:
	bool m_Hover;
	int m_PressCount;
	int m_Padding;
	bool m_OffOn;
	std::unique_ptr<DelegateArgBase<bool>> m_Callback;
};
