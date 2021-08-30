#pragma once

/*
 * �z�X�g��p���
 */

#include "Scene.h"
#include "ScLabel.h"
#include "ScList.h"
#include <string>

class SceneHost : public Scene
{
private:
	// �V�K�Q�X�g�̐ڑ����󂯕t����i�K
	// �����ύX������ ACCEPT_STEP_STATE ���ύX���邱��
	enum class AcceptGuestStep
	{
		// ��tOFF
		OFF,

		// �ȉ��S����tON
		WAIT_CONNECT,
		REQUEST_NAME,
		SELECT_ACCEPT_REJECT,
		SELECTED
	};

	// �V�K�Q�X�g�̐ڑ����󂯕t����i�K��\��������
	static const char ACCEPT_STEP_STATE[][64];

public:
	SceneHost();
	~SceneHost();

public:
	Scene* Update() override;
	void Draw() override;

private:
	void ProcessNewGuest();
	void ProcessDissconectGuest();
	void ProcessReceiveCommand();

	// �R�[���o�b�N�֐�
	void SetPortNum(std::string& Port);
	void SetName(std::string& Name);
	void SetAcceptGuest(bool& OffOn);
	void AcceptGuest();
	void RejectGuest();
	void Disconnect();
	void End();

private:
	// ���F�E���ۂ�I������Ƃ��ɂ����\�����镔�i
	std::vector<ScreenComponent*> m_AcceptRejectComponent;

	Scene* m_Next;

	// �z�X�g�̃|�[�g�A���O
	int m_Port;
	std::string m_Name;

	// �V�K�Q�X�g�̐ڑ����󂯕t����i�K���Ǘ�����
	AcceptGuestStep m_AcceptGuestStep;

	// �V�K�Q�X�g�̐ڑ��̒i�K��i�ޒ��Ŏg���ϐ�
	ScLabel* m_AcceptState;
	ScLabel* m_NewGuestIP;
	ScLabel* m_NewGuestName;
	int m_NewGuestNetHandle;
	bool m_AcceptOrReject;

	// ���F���ꂽ�Q�X�g�B�����������ʕ��i
	ScList* m_AcceptedGuest;
};