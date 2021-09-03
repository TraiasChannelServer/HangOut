#pragma once

/*
 * �f���Q�[�g�p�^�[��
 * 
 * ����̃C���X�^���X�̓���̊֐��������ł���
 * �R�[���o�b�N�֐��Ɏg��
 * 
 * void Func(_�C�ӂ̈������_&)
 * �������Q�Ƃł���̂ŁA�Ăяo����ł��̈����̒l��ύX�ł���
 * 
 * �z�肵�Ă���g�����F
 * �P�D���̒l�ɕύX���܂�����ƃR�[���o�b�N�֐����Ăяo��
 * �Q�D�Ăяo���悪���̒l�̓_���ł��ƒl�̕ύX�������Ȃ����Ƃ��ł���
 */

template<class T>
class DelegateArgBase
{
public:
	virtual void operator()(T& Value) = 0;
	virtual ~DelegateArgBase() = default;
};

template<class T, class U>
class DelegateArg : public DelegateArgBase<U>
{
public:
	using EventFunc = void (T::*)(U&);

public:
	DelegateArg(T& Obj, EventFunc Fnc)
		: m_Obj(Obj)
		, m_Fnc(Fnc)
	{
	}

	void operator()(U& Value) override
	{
		(m_Obj.*m_Fnc)(Value);
	}

private:
	T& m_Obj;
	EventFunc m_Fnc;
};