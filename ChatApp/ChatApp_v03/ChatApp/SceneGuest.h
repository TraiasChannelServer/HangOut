#pragma once

/*
 * �Q�X�g��p���
 */

#include "Scene.h"
#include "ScLabel.h"
#include "ScList.h"
#include <string>

class SceneGuest : public Scene
{
private:
	// �ڑ��̒i�K
	enum class ConnectStep
	{
		OFF,
		WAIT_ACCEPT,
		ON
	};

	// �ڑ��̒i�K��\��������
	static const char CONNECT_STEP_STATE[][64];

public:
	SceneGuest();
	~SceneGuest();

public:
	Scene* Update() override;
	void Draw() override;

private:
	// �R�[���o�b�N�֐�
	void SetIP1(std::string& IP);
	void SetIP2(std::string& IP);
	void SetIP3(std::string& IP);
	void SetIP4(std::string& IP);
	void SetPortNum(std::string& Port);
	void SetName(std::string& Name);
	void TryConnect();
	void Disconnect();
	void RequestAllUpdate();
	void End();

private:
	Scene* m_Next;

	// �z�X�g��IP,Port
	IPDATA m_IP;
	int m_Port;

	// �Q�X�g�̖��O
	std::string m_Name;

	// �ڑ��̒i�K���Ǘ�����
	ConnectStep m_ConnectStep;

	// �ڑ��̒i�K�𕶎���ŏ��������ʕ��i
	ScLabel* m_ConnectState;

	// �ڑ������z�X�g�̏��
	int m_NetHandle;
	ScLabel* m_HostName;

	// �����z�X�g�ɐڑ����Ă���Q�X�g�B�����������ʕ��i
	ScList* m_ConnectingGuest;
};