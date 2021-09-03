#pragma once

/*
 * ��ʕ��i�F���X�g
 * �����̕��������������
 * ������̒ǉ��E�폜�𓮓I�ɍs����
 * �ǉ��E�폜��������Item�ƌĂ�
 * ���Item�Ɉ��ID��ݒ肷��
 */

#include "ScreenComponent.h"
#include "TextAlignment.h"
#include <string>
#include <map>

class ScList : public ScreenComponent
{
public:
	// �`��̓x�ɃA���C�����g���v�Z���Ă��Ă͖��ʂȂ̂�
	// �v�Z����(x, y)��ێ����Ă���
	struct Item
	{
		std::string Text = {};
		int x = 0;
		int y = 0;
	};

public:
	// �w�i�ɐF�����郉�x���A���g�̋��E�͍D���ɐݒ�ł���
	// @param ColorFore  �����F
	// @param ColorBack  �w�i�F
	// @param bound      Item����̋��E
	// @param gravity    ���񂹂Ƃ����������Ƃ�
	// @param FontHandle �t�H���g
	// @param ColorFrame ���E���̐F
	ScList(unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle, unsigned int ColorFrame);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

	void AddItem(int ID, const std::string& Text);
	void RemoveItem(int ID);
	void RemoveAllItem();
	void ChangeText(int ID, const std::string& Text);

	const std::string& GetText(int ID) const;
	const std::map<int, Item>& GetItem() const;

protected:
	// ������̕\���ʒu���v�Z
	void Alignment();

protected:
	unsigned int m_ColorFore;
	unsigned int m_ColorBack;
	unsigned int m_ColorFrame;
	TextAlignment::Gravity m_Gravity;
	std::map<int, Item> m_Item;
	int m_FontHandle;
	int m_ItemYSize;
};
