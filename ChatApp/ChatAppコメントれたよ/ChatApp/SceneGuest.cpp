#include "SceneGuest.h"
#include "ScButton.h"
#include "ScEdit.h"
#include "FontManager.h"
#include "BoundRect.h"
#include "Common.h"
#include "SceneCreator.h"
#include "Command.h"

const char SceneGuest::CONNECT_STEP_STATE[][64] =
{
	"�ڑ����Ă��܂���",
	"���F�����̂�ҋ@���Ă��܂�...",
	"�ڑ����Ă��܂�"
};

SceneGuest::SceneGuest()
	: m_Next()
	, m_IP()
	, m_Port(9850)
	, m_Name("Guest")
	, m_ConnectStep()
	, m_ConnectState()
	, m_NetHandle()
	, m_HostName()
	, m_ConnectingGuest()
{
	// �R���X�g���N�^�ł͉�ʂ̕��i��z�u����

	int SmallFont = FontManager::Inst().GetFontHandle(FontManager::Type::SMALL);
	int MiddleFont = FontManager::Inst().GetFontHandle(FontManager::Type::MIDDLE);
	int BigFont = FontManager::Inst().GetFontHandle(FontManager::Type::BIG);

	unsigned int BlackColor = GetColor(0, 0, 0);
	unsigned int GreenColor = GetColor(0, 255, 0);
	unsigned int RedColor = GetColor(255, 50, 255);

	unsigned int EditColorFore = GetColor(0, 0, 0);
	unsigned int EditColorBack = GetColor(180, 180, 180);
	unsigned int EditColorFrame = GetColor(50, 50, 50);
	unsigned int ToggleColorFore = GetColor(0, 255, 255);
	unsigned int ToggleColorBack = GetColor(50, 50, 100);

	static constexpr int X_POS = 120;

	static constexpr int X_START = 10;
	static constexpr int Y_SIZE = 18;
	static constexpr int Y_STEP_SMALL = 24;
	static constexpr int Y_STEP_MIDDLE = 28;
	static constexpr int Y_STEP_BIG = 40;
	int y = 10;

	AddBaseComponent(new ScLabel("���Ȃ��̓Q�X�g�ł�", GreenColor, 10, y, SmallFont));

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("IP�F", GreenColor, 10, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 60, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP1);
		AddBaseComponent(new ScEdit(std::to_string(m_IP.d1), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
	}
	{
		BoundRect* bound = new BoundRect(X_POS + 60, y, 60, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP2);
		AddBaseComponent(new ScEdit(std::to_string(m_IP.d2), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
	}
	{
		BoundRect* bound = new BoundRect(X_POS + 120, y, 60, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP3);
		AddBaseComponent(new ScEdit(std::to_string(m_IP.d3), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
	}
	{
		BoundRect* bound = new BoundRect(X_POS + 180, y, 60, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP4);
		AddBaseComponent(new ScEdit(std::to_string(m_IP.d4), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("�|�[�g�ԍ��F", GreenColor, 10, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetPortNum);
		AddBaseComponent(new ScEdit(std::to_string(m_Port), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 5, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("���O�F", GreenColor, 10, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetName);
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 16, Callback));
	}

	y += Y_STEP_SMALL * 2;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 220);
		unsigned int ColorFrame = GetColor(50, 100, 100);
		unsigned int ColorBackHover = GetColor(200, 240, 240);
		unsigned int ColorBackPress = GetColor(160, 200, 200);
		auto CallbackHost = new DelegateVoid<SceneGuest>(*this, &SceneGuest::TryConnect);
		AddBaseComponent(new ScButton("�ڑ������݂�", ColorFore, ColorBack, X_START + 10, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}

	y += Y_STEP_SMALL * 2;
	m_ConnectState = new ScLabel(CONNECT_STEP_STATE[0], RedColor, 10, y, MiddleFont);
	AddBaseComponent(m_ConnectState);

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("�z�X�g���F", GreenColor, 10, y, SmallFont));

	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		BoundRect* bound = new BoundRect(100, y, 200, Y_SIZE);
		m_HostName = new ScLabel("--���ڑ�--", ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont);
		AddBaseComponent(m_HostName);
	}

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("�ڑ����̃Q�X�g�F", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		BoundRect* bound = new BoundRect(X_START + 10, y, 250, Y_SIZE);
		m_ConnectingGuest = new ScList(ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont, ColorFrame);
		AddBaseComponent(m_ConnectingGuest);
	}

	y = Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto CallbackHost = new DelegateVoid<SceneGuest>(*this, &SceneGuest::End);
		AddBaseComponent(new ScButton("�I��", ColorFore, ColorBack, X_START, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
}

SceneGuest::~SceneGuest()
{
	Disconnect();
}

Scene* SceneGuest::Update()
{
	UpdateBase();
	switch (m_ConnectStep)
	{
	case ConnectStep::OFF:
		// �z�X�g�ɐڑ����Ă��Ȃ����
		break;
	case ConnectStep::WAIT_ACCEPT:
	{
		// �����̖��O�𑗐M������
		// �z�X�g���珳�F�E���ۃ��b�Z�[�W��҂�
		int DataLength = GetNetWorkDataLength(m_NetHandle);
		if (DataLength != 0)
		{
			// ���b�Z�[�W���͂�����
			char StrBuf[256] = {};
			NetWorkRecv(m_NetHandle, StrBuf, DataLength);

			// ���b�Z�[�W�����߂���
			Command::Type type = {};
			auto Message = Command::Interpret(StrBuf, type);

			if (type == Command::Type::CONNECT)
			{
				// ���F�E���ۃ��b�Z�[�W�������Ƃ�
				std::string AcceptReject = Message[0];
				std::string Name = Message[1];
				if (AcceptReject == Command::CONNECT_ACCEPT)
				{
					// ���F���ꂽ�Ƃ�
					// �󂯎�����z�X�g����ێ�
					m_HostName->ChangeText(Name);
					// �ڑ��i�K�֏�����i��
					m_ConnectStep = ConnectStep::ON;
				}
				else
				{
					// ���ۂ��ꂽ�Ƃ�
					// �ؒf����
					Disconnect();
					// ���ڑ��i�K�֏�����߂�
					m_ConnectStep = ConnectStep::OFF;
				}
			}
			else
			{
				// �Ӗ��s���ȃ��b�Z�[�W�������Ƃ�
				// �ؒf����
				Disconnect();
				// ���ڑ��i�K�֏�����߂�
				m_ConnectStep = ConnectStep::OFF;
			}
			// �ڑ��i�K��\������e�L�X�g�ɔ��f����
			m_ConnectState->ChangeText(CONNECT_STEP_STATE[static_cast<int>(m_ConnectStep)]);
		}
	}
	break;
	case ConnectStep::ON:
	{
		// �z�X�g�ɏ��F��������Đڑ��ł��Ă���Ƃ�

		int DataLength = GetNetWorkDataLength(m_NetHandle);
		if (DataLength != 0)
		{
			// �z�X�g���烁�b�Z�[�W����M�����Ƃ�
			char StrBuf[256] = {};
			NetWorkRecv(m_NetHandle, StrBuf, DataLength);

			// ���b�Z�[�W�����߂���
			Command::Type type = {};
			auto Message = Command::Interpret(StrBuf, type);

			switch (type)
			{
			case Command::Type::CONNECT:
			{
				std::string AcceptReject = Message[0];
				std::string Name = Message[1];
				if (AcceptReject == Command::CONNECT_REJECT)
				{
					// �ؒf���܂��傤���b�Z�[�W�������Ƃ�
					Disconnect();
					m_ConnectStep = ConnectStep::OFF;
					m_ConnectState->ChangeText(CONNECT_STEP_STATE[static_cast<int>(m_ConnectStep)]);
				}
			}
			break;
			case Command::Type::MESSAGE:
				// �N�����`���b�g�ł��܂������b�Z�[�W�������Ƃ�
				break;
			case Command::Type::CHANGE_NAME:
			{
				// �N�������O�ς��܂������b�Z�[�W�������Ƃ�

				std::string ID = Message[0];
				std::string Name = Message[1];
				int id = std::stoi(ID);
				if (id == m_NetHandle)
				{
					// ���O��ς����̂��z�X�g�������Ƃ�
					m_HostName->ChangeText(Name);
				}
				else
				{
					// ���O��ς����̂��z�X�g����Ȃ������Ƃ�
					// �ڑ��ς݃Q�X�g����N�Ȃ̂���˂��~�߂�
					auto& list = m_ConnectingGuest->GetItem();
					for (auto& pair : list)
					{
						int ID = pair.first;
						if (id == ID)
						{
							// �˂��~�߂���ύX�𔽉f����
							m_ConnectingGuest->ChangeText(ID, Name);
							break;
						}
					}
				}
			}
			break;
			case Command::Type::NEW_GUEST:
			{
				// �V�K�Q�X�g������Ă��܂������b�Z�[�W�������Ƃ�

				int id = std::stoi(Message[0]);
				std::string Name = Message[1];
				m_ConnectingGuest->AddItem(id, Name);
			}
			break;
			case Command::Type::DISCONNECT_GUEST:
			{
				// �N�����ؒf���܂����惁�b�Z�[�W�������Ƃ�

				// �ڑ��ς݃Q�X�g����N�Ȃ̂���˂��~�߂�
				int id = std::stoi(Message[0]);
				auto& list = m_ConnectingGuest->GetItem();
				for (auto& pair : list)
				{
					int ID = pair.first;
					if (id == ID)
					{
						// �˂��~�߂��烊�X�g����폜
						m_ConnectingGuest->RemoveItem(ID);
						break;
					}
				}
			}
			break;
			case Command::Type::ERROR_TYPE:
				break;
			}

		}
	}
	break;
	}

	return m_Next;
}

void SceneGuest::Draw()
{
	ClearDrawScreen();
	DrawBase();
	switch (m_ConnectStep)
	{
	case ConnectStep::OFF:
		break;
	case ConnectStep::WAIT_ACCEPT:
		break;
	case ConnectStep::ON:
		break;
	}
}

void SceneGuest::SetIP1(std::string& IP)
{
	// IP1�G�f�B�b�g�e�L�X�g�̃R�[���o�b�N�֐�

	// �G�f�B�b�g�e�L�X�g�͐��l�ȊO����肤��̂�
	// ��ʕ��i�ƃf�[�^��IP����v�����Ȃ���΂Ȃ�Ȃ����ƂɋC��t����
	// IP2-4��Port�����l

	m_IP.d1 = std::atoi(IP.c_str());
	IP = std::to_string(m_IP.d1);
}

void SceneGuest::SetIP2(std::string& IP)
{
	m_IP.d2 = std::atoi(IP.c_str());
	IP = std::to_string(m_IP.d2);
}

void SceneGuest::SetIP3(std::string& IP)
{
	m_IP.d3 = std::atoi(IP.c_str());
	IP = std::to_string(m_IP.d3);
}

void SceneGuest::SetIP4(std::string& IP)
{
	m_IP.d4 = std::atoi(IP.c_str());
	IP = std::to_string(m_IP.d4);
}

void SceneGuest::SetPortNum(std::string& Port)
{
	m_Port = std::atoi(Port.c_str());
	Port = std::to_string(m_Port);
}

void SceneGuest::SetName(std::string& Name)
{
	// ���O�G�f�B�b�g�e�L�X�g�̃R�[���o�b�N�֐�

	if (Name == "")
	{
		// �󕶎��ւ̕ύX�͋p��
		Name = m_Name;
	}
	else
	{
		// ���O�̕ύX
		m_Name = Name;
		if (m_ConnectStep == ConnectStep::ON)
		{
			// �z�X�g�ɐڑ����Ă���Ƃ�
			// ���O�ύX���܂������b�Z�[�W�𑗐M
			std::string Message = Command::MakeChangeName(m_NetHandle, m_Name);
			NetWorkSend(m_NetHandle, Message.c_str(), Message.size());
		}
	}
}

void SceneGuest::TryConnect()
{
	// �ڑ������݂�{�^���̃R�[���o�b�N�֐�

	if (m_ConnectStep == ConnectStep::OFF)
	{
		m_NetHandle = ConnectNetWork(m_IP, m_Port);
		if (m_NetHandle == -1)
		{
			// �ڑ����s������OFF�̂܂܁i�ꉞ����j
			m_ConnectStep = ConnectStep::OFF;
		}
		else
		{
			// �ڑ�����������܂��͖���錈�܂�Ȃ̂�
			// �����̖��O���z�X�g�ɑ��M����
			std::string Message = Command::MakeChangeName(m_NetHandle, m_Name);
			NetWorkSend(m_NetHandle, Message.c_str(), Message.size());

			// ���F�҂��i�K�֏�����i��
			m_ConnectStep = ConnectStep::WAIT_ACCEPT;
		}
		// �i�K�̕ω���\������e�L�X�g�ɔ��f
		m_ConnectState->ChangeText(CONNECT_STEP_STATE[static_cast<int>(m_ConnectStep)]);
	}
}

void SceneGuest::Disconnect()
{
	if (m_NetHandle != -1)
		CloseNetWork(m_NetHandle);
	m_NetHandle = -1;
}

void SceneGuest::End()
{
	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}


