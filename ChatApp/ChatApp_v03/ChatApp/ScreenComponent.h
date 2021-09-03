#pragma once

/*
 * ��ʕ��i�̐e�N���X
*/

#include "Bound.h"
#include <memory>

class ScreenComponent
{
public:
	ScreenComponent();
	ScreenComponent(Bound* bound);
	virtual ~ScreenComponent() = default;

public:
	// �}�E�X����ɔ������邽�߂̊֐�
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMousePress(int x, int y) = 0;

	// ���g��`�悷�邽�߂̊֐�
	virtual void Draw() const = 0;

protected:
	// ���g�̑傫����\�����E
	std::unique_ptr<Bound> m_Bound;
};

inline ScreenComponent::ScreenComponent()
	: m_Bound()
{
}
inline ScreenComponent::ScreenComponent(Bound* bound)
	: m_Bound(bound)
{
}
