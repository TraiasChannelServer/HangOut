#include "KeyInputter.h"
#include "Common.h"
#include "DxLib.h"

#include <memory>

KeyInputter::KeyInputter()
	: m_Handle(-1)
	, m_User(nullptr)
	, m_X()
	, m_Y()
	, m_Length()
{
}

/*
 * ���̊֐����Ă񂾌�͕K��End�֐����ĂԂ���
 * 
 * @param User   �L�[���͂��g�p����N���X�̃A�h���X
 * @param x      ���͒��̕������`�悷��X���W
 * @param y      ���͒��̕������`�悷��Y���W
 * @param Length ���͂ł���ő啶�����i���p��T�C�Y, \0���܂܂Ȃ��j
 */
bool KeyInputter::Start(const void* User, int x, int y, int Length)
{
	if (GetState(User) != State::FREE)
	{
		return false;
	}

	m_Handle = MakeKeyInput(Length, TRUE, FALSE, FALSE);

	if (m_Handle == -1)
	{
		return false;
	}

	m_User = User;
	m_X = x;
	m_Y = y;
	m_Length = Length;

	SetActiveKeyInput(m_Handle);

	return true;
}

/*
 * @return
 *     �N���g���ĂȂ�      �FState::FREE
 *     �g�p�҂ƈ������s��v�FState::INVALID
 *     �g�p�҂ƈ�������v  �F���͒��FState::ACTIVE
 *                           �I��  �FState::SUCCESS or CANCEL or FAIL
 */
KeyInputter::State KeyInputter::GetState(const void* User)
{
	if (User == nullptr)
	{
		return State::INVALID;
	}

	if (m_User == nullptr || m_Handle == -1)
	{
		return State::FREE;
	}

	if (User != m_User)
	{
		return State::INVALID;
	}

	switch (CheckKeyInput(m_Handle))
	{
	case 0: return State::ACTIVE;
	case 1: return State::SUCCESS;
	case 2: return State::CANCEL;
	}
	return State::FAIL;
}

/*
 * @return true:End�֐����Ă�Ō��ʂ��󂯎����Ԃł���
 */
bool KeyInputter::IsStateFinish(const void* User)
{
	State state = GetState(User);
	if ((state == State::SUCCESS)
		|| (state == State::CANCEL)
		|| (state == State::FAIL))
	{
		return true;
	}
	return false;
}

/*
 * �L�[���͌��ʂ̕�������󂯎��Ɠ�����
 * �N���X�̏�Ԃ��܂�Start�֐����Ăׂ��Ԃɂ���
 * 
 * GetState(User) == State::SUCCESS�̂Ƃ���
 * ��x�����߂�l���󂯎�邱�Ƃ��ł���
 */
std::string KeyInputter::End(const void* User)
{
	if (GetState(User) != State::SUCCESS)
	{
		DeleteKeyInput(m_Handle);
		m_Handle = -1;
		m_User = nullptr;

		return std::string();
	}

	// ���͂ł���ő啶������m_Length�����A�󂯎��Ƃ���'\0'������̂� +1 ���Ă���
	std::unique_ptr<char[]> Buf = std::unique_ptr<char[]>(new char[m_Length + 1]);
	GetKeyInputString(Buf.get(), m_Handle);

	DeleteKeyInput(m_Handle);
	m_Handle = -1;
	m_User = nullptr;

	return std::string(Buf.get());
}

/*
 * ���͒��̂Ƃ��������̗l�q��`�悷��
 */
void KeyInputter::Draw()
{
	if (GetState(m_User) != State::ACTIVE)
	{
		return;
	}
	DrawKeyInputString(m_X, m_Y, m_Handle);
	DrawKeyInputModeString(Common::WINDOW_X_SIZE - 100, Common::WINDOW_Y_SIZE - 16);
}
