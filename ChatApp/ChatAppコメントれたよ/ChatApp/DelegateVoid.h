#pragma once

/*
 * �f���Q�[�g�p�^�[��
 * ����̃C���X�^���X�̓���̊֐��������ł���
 * �R�[���o�b�N�֐��Ɏg��
 * void Func(void)
 */

class DelegateVoidBase
{
public:
	virtual void operator()() = 0;
	virtual ~DelegateVoidBase() = default;
};

template<class T>
class DelegateVoid : public DelegateVoidBase
{
public:
	using EventFunc = void (T::*)();

public:
	DelegateVoid(T& Obj, EventFunc Fnc)
		: m_Obj(Obj)
		, m_Fnc(Fnc)
	{
	}

	void operator()() override
	{
		(m_Obj.*m_Fnc)();
	}

private:
	T& m_Obj;
	EventFunc m_Fnc;
};