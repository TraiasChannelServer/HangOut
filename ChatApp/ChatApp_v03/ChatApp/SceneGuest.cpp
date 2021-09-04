#include "SceneGuest.h"
#include "ScButton.h"
#include "ScEdit.h"
#include "FontManager.h"
#include "BoundRect.h"
#include "Common.h"
#include "SceneCreator.h"
#include "Command.h"
#include "Logger.h"
#include "DataFileInOut.h"

const char SceneGuest::CONNECT_STEP_STATE[][64] =
{
	"�ڑ����Ă��܂���",
	"���F�����̂�ҋ@���Ă��܂�...",
	"�ڑ����Ă��܂�"
};

SceneGuest::SceneGuest()
	: m_Next()
	, m_IP(DataFileInOut::Inst().GetIP())
	, m_Port(DataFileInOut::Inst().GetPortNum())
	, m_Name(DataFileInOut::Inst().GetName())
	, m_ConnectStep()
	, m_ConnectState()
	, m_NetHandle()
	, m_HostName()
	, m_ConnectingGuest()
{
	// �R���X�g���N�^�ł͉�ʂ̕��i��z�u����
	Logger::Info("�Q�X�g��ʐ���");

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
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, DataFileInOut::NAME_LENGTH_MAX, Callback));
	}

	y += Y_STEP_SMALL * 2;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 220);
		unsigned int ColorFrame = GetColor(50, 100, 100);
		unsigned int ColorBackHover = GetColor(200, 240, 240);
		unsigned int ColorBackPress = GetColor(160, 200, 200);
		auto Callback = new DelegateVoid<SceneGuest>(*this, &SceneGuest::TryConnect);
		AddBaseComponent(new ScButton("�ڑ������݂�", ColorFore, ColorBack, X_START + 10, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
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
		unsigned int ColorBack = GetColor(220, 220, 180);
		unsigned int ColorFrame = GetColor(100, 100, 50);
		unsigned int ColorBackHover = GetColor(240, 240, 200);
		unsigned int ColorBackPress = GetColor(200, 200, 160);
		auto Callback = new DelegateVoid<SceneGuest>(*this, &SceneGuest::RequestAllUpdate);
		AddBaseComponent(new ScButton("�S�X�V���N�G�X�g", ColorFore, ColorBack, X_START + 75, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}

	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto Callback = new DelegateVoid<SceneGuest>(*this, &SceneGuest::End);
		AddBaseComponent(new ScButton("�I��", ColorFore, ColorBack, X_START, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}
}

SceneGuest::~SceneGuest()
{
	Logger::Info("�Q�X�g��ʔj��");

	DataFileInOut::Inst().SetIP(m_IP);
	DataFileInOut::Inst().SetPortNum(m_Port);
	DataFileInOut::Inst().SetName(m_Name);

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
		Command::ReceiveResult result = Command::CheckReceive(m_NetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// ���b�Z�[�W�������Ă�����
			Command::Message Msg = Command::Receive(m_NetHandle);

			if (Msg.type == Command::Type::CONNECT)
			{
				// ���F�E���ۃ��b�Z�[�W�������Ƃ�
				if (Msg.single.flag)
				{
					// ���F���ꂽ�Ƃ�
					Logger::Info("[ConnectStep::WAIT_ACCEPT] ���F���ꂽ");

					// �󂯎�����z�X�g����ێ�
					m_HostName->ChangeText(Msg.string.text);
					// �ڑ��i�K�֏�����i��
					m_ConnectStep = ConnectStep::ON;
				}
				else
				{
					// ���ۂ��ꂽ�Ƃ�
					Logger::Info("[ConnectStep::WAIT_ACCEPT] ���ۂ��ꂽ");

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

		Command::ReceiveResult result = Command::CheckReceive(m_NetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// ���b�Z�[�W�������Ă�����
			Command::Message Msg = Command::Receive(m_NetHandle);

			switch (Msg.type)
			{
			case Command::Type::CONNECT:
			{
				if (Msg.single.flag == false)
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
			case Command::Type::CHANGE_NAME_MYSELF:
			{
				// �z�X�g�����O�ς��܂������b�Z�[�W�������Ƃ�
				m_HostName->ChangeText(Msg.string.text);
			}
			break;
			case Command::Type::CHANGE_NAME_GUEST:
			{
				// �N�������O�ς��܂������b�Z�[�W�������Ƃ�
				// �ڑ��ς݃Q�X�g����N�Ȃ̂���˂��~�߂�
				auto& list = m_ConnectingGuest->GetItem();
				for (auto& pair : list)
				{
					int ID = pair.first;
					if (ID == Msg.single.num)
					{
						// �˂��~�߂���ύX�𔽉f����
						m_ConnectingGuest->ChangeText(ID, Msg.string.text);
						break;
					}
				}
			}
			break;
			case Command::Type::NEW_GUEST:
			{
				// �V�K�Q�X�g������Ă��܂������b�Z�[�W�������Ƃ�
				m_ConnectingGuest->AddItem(Msg.single.num, Msg.string.text);
			}
			break;
			case Command::Type::DISCONNECT_GUEST:
			{
				// �N�����ؒf���܂����惁�b�Z�[�W�������Ƃ�

				// �ڑ��ς݃Q�X�g����N�Ȃ̂���˂��~�߂�
				auto& list = m_ConnectingGuest->GetItem();
				for (auto& pair : list)
				{
					int ID = pair.first;
					if (ID == Msg.single.num)
					{
						// �˂��~�߂��烊�X�g����폜
						m_ConnectingGuest->RemoveItem(ID);
						break;
					}
				}
			}
			break;
			default:
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
		Logger::Info("�����̖��O��ύX�F%s �� %s", m_Name.c_str(), Name.c_str());

		m_Name = Name;
		if (m_ConnectStep == ConnectStep::ON)
		{
			// �z�X�g�ɐڑ����Ă���Ƃ�
			// ���O�ύX���܂������b�Z�[�W�𑗐M
			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			Command::Send(m_NetHandle, msgSend);
		}
	}
}

void SceneGuest::TryConnect()
{
	// �ڑ������݂�{�^���̃R�[���o�b�N�֐�
	Logger::Info("�ڑ������݂�{�^������");

	if (m_ConnectStep == ConnectStep::OFF)
	{
		m_NetHandle = ConnectNetWork(m_IP, m_Port);
		if (m_NetHandle == -1)
		{
			Logger::Info("�ڑ����s");

			// �ڑ����s������OFF�̂܂܁i�ꉞ����j
			m_ConnectStep = ConnectStep::OFF;
		}
		else
		{
			Logger::Info("�ڑ�����");

			// �ڑ�����������܂��͖���錈�܂�Ȃ̂�
			// �����̖��O���z�X�g�ɑ��M����
			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			Command::Send(m_NetHandle, msgSend);

			// ���F�҂��i�K�֏�����i��
			m_ConnectStep = ConnectStep::WAIT_ACCEPT;
		}
		// �i�K�̕ω���\������e�L�X�g�ɔ��f
		m_ConnectState->ChangeText(CONNECT_STEP_STATE[static_cast<int>(m_ConnectStep)]);
	}
}

void SceneGuest::Disconnect()
{
	Logger::Info("�ؒf�FNetHandle = %d", m_NetHandle);

	if (m_NetHandle != -1)
		CloseNetWork(m_NetHandle);
	m_NetHandle = -1;
}

void SceneGuest::RequestAllUpdate()
{
	Logger::Info("�S�X�V���N�G�X�g�{�^������");

	if (m_ConnectStep == ConnectStep::ON)
	{
		// �z�X�g�ɐڑ����Ă���Ƃ�

		// ���O�Ȃǎ��g�̃f�[�^�ȊO���ׂč폜����
		m_ConnectingGuest->RemoveAllItem();

		// �X�V���N�G�X�g�𑗐M
		Command::Message msgSend = Command::MakeAllUpdate();
		Command::Send(m_NetHandle, msgSend);
	}
}

void SceneGuest::End()
{
	Logger::Info("�I���{�^�������Fm_Next = %d", m_Next);

	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}


