#pragma once

/*
 * �V�[���̐������s���N���X
 * �������@�����G�ȃV�[���������Ă�
 * ���̃N���X��ʂ��ƊȒP�ɐ����ł���
 */

#include "Scene.h"

class SceneCreator
{
public:
	enum class Name
	{
		CHOICE_HOST_GUEST,
		HOST,
		GUEST
	};

public:
	static Scene* Create(Name name);

private:
	SceneCreator() = delete;
};