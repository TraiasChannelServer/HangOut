#pragma once

/*
 * �z�X�g���Q�X�g����I�ԉ��
 */

#include "Scene.h"

class SceneChoiceHostGuest : public Scene
{
public:
	SceneChoiceHostGuest();

public:
	Scene* Update() override;
	void Draw() override;

private:
	// �R�[���o�b�N�֐�
	void BeHost();
	void BeGuest();
	void End();

private:
	Scene* m_Next;
};

