#include "SceneHost.h"
#include "ScButton.h"
#include "ScEdit.h"
#include "ScToggle.h"
#include "FontManager.h"
#include "BoundRect.h"
#include "Common.h"
#include "SceneCreator.h"
#include "Command.h"
#include "Logger.h"
#include "DataFileInOut.h"

const char SceneHost::ACCEPT_STEP_STATE[][64] =
{
	"�V�K�Q�X�g�͎󂯕t���Ă��܂���",
	"�V�K�Q�X�g���󂯕t�����ł�...",
	"�V�K�Q�X�g���K�₵�Ă��܂�...",
	"�ȉ��̃Q�X�g���K�₵�Ă��܂���",
	"������..."
};

SceneHost::SceneHost()
	: m_AcceptRejectComponent()
	, m_Next()
	, m_Port(DataFileInOut::Inst().GetPortNum())
	, m_Name(DataFileInOut::Inst().GetName())
	, m_AcceptGuestStep()
	, m_AcceptState()
	, m_NewGuestIP()
	, m_NewGuestName()
	, m_NewGuestNetHandle(-1)
	, m_AcceptOrReject()
	, m_RequestNameTimeStart()
	, m_AcceptedGuest()
{
	// �R���X�g���N�^�ł͉�ʂ̕��i��z�u����
	Logger::Info("�z�X�g��ʐ���");

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

	AddBaseComponent(new ScLabel("���Ȃ��̓z�X�g�ł�", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("�|�[�g�ԍ��F", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetPortNum);
		AddBaseComponent(new ScEdit(std::to_string(m_Port), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 5, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("���O�F", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetName);
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, DataFileInOut::NAME_LENGTH_MAX, Callback));
	}

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("�V�K�Q�X�g�̎�t�F", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(170, y, 80, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, bool>(*this, &SceneHost::SetAcceptGuest);
		AddBaseComponent(new ScToggle(ToggleColorFore, ToggleColorBack, bound, SmallFont, Callback));
	}
	y += Y_STEP_SMALL * 2;
	m_AcceptState = new ScLabel(ACCEPT_STEP_STATE[0], RedColor, 10, y, MiddleFont);
	AddBaseComponent(m_AcceptState);
	y += Y_STEP_MIDDLE;
	m_AcceptRejectComponent.push_back(new ScLabel("IP�F", GreenColor, X_START, y, SmallFont));
	m_NewGuestIP = new ScLabel("xxx.xxx.xx.xxx", GreenColor, 80, y, SmallFont);
	m_AcceptRejectComponent.push_back(m_NewGuestIP);
	y += Y_STEP_SMALL;
	m_AcceptRejectComponent.push_back(new ScLabel("���O�F", GreenColor, X_START, y, SmallFont));
	m_NewGuestName = new ScLabel("Guest", GreenColor, 80, y, SmallFont);
	m_AcceptRejectComponent.push_back(m_NewGuestName);

	y += Y_STEP_MIDDLE;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 180, 220);
		unsigned int ColorFrame = GetColor(50, 50, 100);
		unsigned int ColorBackHover = GetColor(200, 200, 240);
		unsigned int ColorBackPress = GetColor(160, 160, 200);
		auto CallbackHost = new DelegateVoid<SceneHost>(*this, &SceneHost::AcceptGuest);
		m_AcceptRejectComponent.push_back(new ScButton("���F", ColorFore, ColorBack, X_START + 10, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(220, 180, 180);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		unsigned int ColorBackHover = GetColor(240, 200, 200);
		unsigned int ColorBackPress = GetColor(200, 160, 160);
		auto CallbackHost = new DelegateVoid<SceneHost>(*this, &SceneHost::RejectGuest);
		m_AcceptRejectComponent.push_back(new ScButton("����", ColorFore, ColorBack, X_START + 110, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}

	y += Y_STEP_BIG + Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("���F�ς݃Q�X�g�F", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		BoundRect* bound = new BoundRect(X_START + 10, y, 250, Y_SIZE);
		m_AcceptedGuest = new ScList(ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont, ColorFrame);
		AddBaseComponent(m_AcceptedGuest);
	}

	y = Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto CallbackHost = new DelegateVoid<SceneHost>(*this, &SceneHost::End);
		AddBaseComponent(new ScButton("�I��", ColorFore, ColorBack, X_START, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
}

SceneHost::~SceneHost()
{
	Logger::Info("�z�X�g��ʔj��");

	DataFileInOut::Inst().SetPortNum(m_Port);
	DataFileInOut::Inst().SetName(m_Name);

	Disconnect();
	DeleteComponent(m_AcceptRejectComponent);
}

Scene* SceneHost::Update()
{
	UpdateBase();
	ProcessNewGuest();
	ProcessDissconectGuest();
	ProcessReceiveCommand();
	return m_Next;
}

void SceneHost::Draw()
{
	ClearDrawScreen();
	DrawBase();
	switch (m_AcceptGuestStep)
	{
	case AcceptGuestStep::OFF:
		break;
	case AcceptGuestStep::WAIT_CONNECT:
		break;
	case AcceptGuestStep::REQUEST_NAME:
		break;
	case AcceptGuestStep::SELECT_ACCEPT_REJECT:
		// �ڑ������߂Ă��������NetHandle�Ɩ��O������������
		// ���F�E���ۂ�I�������ʕ��i��\������
		DrawComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		break;
	}
}

void SceneHost::ProcessNewGuest()
{
	switch (m_AcceptGuestStep)
	{
	case AcceptGuestStep::OFF:
		// �V�K�Q�X�g�̎�tOFF�̂Ƃ�
		break;
	case AcceptGuestStep::WAIT_CONNECT:
		// �V�K�Q�X�g�̎�tON�̂Ƃ�

		// �V�K�ڑ���҂�
		m_NewGuestNetHandle = GetNewAcceptNetWork();
		if (m_NewGuestNetHandle != -1)
		{
			// �V�����ڑ��𔭌�������
			IPDATA ip = {};
			GetNetWorkIP(m_NewGuestNetHandle, &ip);
			Logger::Info("[AcceptGuestStep::WAIT_CONNECT] �V�����ڑ��𔭌��FNetHandle = %d, IP = %d.%d.%d.%d", m_NewGuestNetHandle, ip.d1, ip.d2, ip.d3, ip.d4);

			Logger::Info("[AcceptGuestStep::WAIT_CONNECT] ���݂̏��F�ς݃Q�X�g�̐ڑ����F%d/%d", m_AcceptedGuest->GetItem().size(), ACCEPTED_GUEST_MAX);

			if (m_AcceptedGuest->GetItem().size() < ACCEPTED_GUEST_MAX)
			{
				// �Q�X�g�̐ڑ������ő吔�ɒB���Ă��Ȃ���
				// �����IP��ێ�
				m_NewGuestIP->ChangeText(Common::IP2String(ip));
				// ������AcceptGuestStep::REQUEST_NAME�֐i��
				m_AcceptGuestStep = AcceptGuestStep::REQUEST_NAME;
				m_RequestNameTimeStart = GetNowCount();
			}
			else
			{
				// �Q�X�g�̐ڑ������ő吔�ɒB���Ă�����
				// �V�K�ڑ����悤�Ƃ��Ă���Q�X�g�ɋ��ۃ��b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�͖����Ȃ��j
				Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
				NetWorkSend(m_NewGuestNetHandle, &msgSend, Command::SIZEOF_MESSAGE);

				Logger::Info("[AcceptGuestStep::WAIT_CONNECT] ���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
					, m_NewGuestNetHandle, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);
			}
		}
		break;
	case AcceptGuestStep::REQUEST_NAME:
	{
		// �ڑ����Ă�������͂܂������̖��O�𖼏��͂��Ȃ̂�
		// �����҂�

		bool Reject = false;

		int DataLength = GetNetWorkDataLength(m_NewGuestNetHandle);
		if (DataLength >= Command::SIZEOF_MESSAGE)
		{
			// ���肩�烁�b�Z�[�W�������Ă�����
			Command::Message msg = {};
			NetWorkRecv(m_NewGuestNetHandle, &msg, Command::SIZEOF_MESSAGE);

			Logger::Info("[AcceptGuestStep::REQUEST_NAME] ���b�Z�[�W����M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, m_NewGuestNetHandle, Command::TypeToString(msg.type), msg.single.num, msg.string.text);

			if (msg.type == Command::Type::CHANGE_NAME_MYSELF)
			{
				// ���肪������Ă�����
				// ����̖��O��ێ�
				m_NewGuestName->ChangeText(msg.string.text);
				// ������AcceptGuestStep::SELECT_ACCEPT_REJECT�֐i��
				m_AcceptGuestStep = AcceptGuestStep::SELECT_ACCEPT_REJECT;
			}
			else
			{
				// ���肪�Ӗ��s���ȃ��b�Z�[�W�𑗂��Ă�����
				Logger::Warn("[AcceptGuestStep::REQUEST_NAME] ���肪�Ӗ��s���ȃ��b�Z�[�W�𑗂��Ă���");
				// ����
				Reject = true;
			}
		}
		else if (DataLength > 0)
		{
			// ���b�Z�[�W�������Ă������A�Œ蒷���Z���Ƃ�
			// ���b�Z�[�W��ǂݎ̂Ă�
			char Buf[Command::SIZEOF_MESSAGE] = {};
			NetWorkRecv(m_NewGuestNetHandle, &Buf, DataLength);
			Logger::Warn("[AcceptGuestStep::REQUEST_NAME] �Œ蒷���Z�����b�Z�[�W���͂����̂Ŏ̂Ă�FNetHandle = %d", m_NewGuestNetHandle);
			// ����
			Reject = true;
		}
		else if (GetNowCount() - m_RequestNameTimeStart >= REQUEST_NAME_TIMEOUT)
		{
			// �^�C���A�E�g
			Logger::Warn("[AcceptGuestStep::REQUEST_NAME] �^�C���A�E�g");
			// ����
			Reject = true;
		}

		if (Reject)
		{
			// �V�K�ڑ����悤�Ƃ��Ă���Q�X�g�ɋ��ۃ��b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�͖����Ȃ��j
			Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
			NetWorkSend(m_NewGuestNetHandle, &msgSend, Command::SIZEOF_MESSAGE);

			Logger::Info("[AcceptGuestStep::REQUEST_NAME] ���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, m_NewGuestNetHandle, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);

			// ������AcceptGuestStep::WAIT_CONNECT�֖߂�
			m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
		}
	}
	break;
	case AcceptGuestStep::SELECT_ACCEPT_REJECT:
		// �ڑ������߂Ă��������NetHandle�Ɩ��O������������
		// ���F�E���ۂ�I�������ʕ��i��\���i�����ł͕\���ł͂Ȃ������j����
		UpdateComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		// ���F�E���ۂ��I�����ꂽ��i���F�E���ۂ̌���̏u�Ԃ̓R�[���o�b�N�֐��j
		if (m_AcceptOrReject)
		{
			// ���F����ꍇ
			Logger::Info("[AcceptGuestStep::SELECTED] ���F");

			// ���F���b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�������Ԃ��j
			Command::Message msgSend = Command::MakeConnect(true, m_Name);
			NetWorkSend(m_NewGuestNetHandle, &msgSend, Command::SIZEOF_MESSAGE);

			Logger::Info("[AcceptGuestStep::SELECTED] ���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, m_NewGuestNetHandle, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);

			// ���F�ς݃Q�X�g�ɒǉ�
			m_AcceptedGuest->AddItem(m_NewGuestNetHandle, m_NewGuestName->GetText());

			// �S���F�ς݃Q�X�g�ɐV�K�Q�X�g�̐ڑ���m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j
			Logger::Info("[AcceptGuestStep::SELECTED] �S���F�ς݃Q�X�g�ɐV�K�Q�X�g�̐ڑ���m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j");
			auto& list = m_AcceptedGuest->GetItem();
			for (auto& pair : list)
			{
				int ID = pair.first;
				Logger::Info("[AcceptGuestStep::SELECTED] ID = %d, Name = %s", ID, pair.second.Text.c_str());

				if (ID == m_NewGuestNetHandle)
				{
					Logger::Info("[AcceptGuestStep::SELECTED] �V�K�Q�X�g�{�l�Ȃ̂Ń��b�Z�[�W�𑗐M���Ȃ�");
					continue;
				}
				Command::Message msgSend2 = Command::MakeNewGuest(m_NewGuestNetHandle, m_NewGuestName->GetText());
				NetWorkSend(ID, &msgSend2, Command::SIZEOF_MESSAGE);

				Logger::Info("[AcceptGuestStep::SELECTED] ���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
					, ID, Command::TypeToString(msgSend2.type), msgSend2.single.num, msgSend2.string.text);
			}

			// �V�K�Q�X�g�ɑS���F�ς݃Q�X�g��m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j
			Logger::Info("[AcceptGuestStep::SELECTED] �V�K�Q�X�g�ɑS���F�ς݃Q�X�g��m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j");
			for (auto& pair : list)
			{
				int ID = pair.first;
				Logger::Info("[AcceptGuestStep::SELECTED] ID = %d, Name = %s", ID, pair.second.Text.c_str());

				if (ID == m_NewGuestNetHandle)
				{
					Logger::Info("[AcceptGuestStep::SELECTED] �V�K�Q�X�g�{�l�Ȃ̂Ń��b�Z�[�W�𑗐M���Ȃ�");
					continue;
				}
				Command::Message msgSend2 = Command::MakeNewGuest(ID, pair.second.Text);
				NetWorkSend(m_NewGuestNetHandle, &msgSend2, Command::SIZEOF_MESSAGE);

				Logger::Info("[AcceptGuestStep::SELECTED] ���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
					, m_NewGuestNetHandle, Command::TypeToString(msgSend2.type), msgSend2.single.num, msgSend2.string.text);
			}
		}
		else
		{
			// ���ۂ���ꍇ
			Logger::Info("[AcceptGuestStep::SELECTED] ����");

			// ���ۃ��b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�͖����Ȃ��j
			Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
			NetWorkSend(m_NewGuestNetHandle, &msgSend, Command::SIZEOF_MESSAGE);

			Logger::Info("[AcceptGuestStep::SELECTED] ���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, m_NewGuestNetHandle, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);
		}
		// ������AcceptGuestStep::WAIT_CONNECT�֖߂�
		m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
		break;
	}
}

void SceneHost::ProcessDissconectGuest()
{
	// �Q�X�g�͂��ؒf���Ă��邩������Ȃ��̂ŏ�Ɋm�F���Ă���
	bool f = false;
	int LostHandle = GetLostNetWork();
	if (LostHandle == -1)
	{
		return;
	}

	Logger::Info("�ؒf�𔭌��FNetHandle = %d", LostHandle);

	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		if (LostHandle == ID)
		{
			// �ؒf���Ă�Q�X�g�𔭌�������
			// ���F�ς݃Q�X�g����폜
			m_AcceptedGuest->RemoveItem(ID);
			f = true;

			Logger::Info("�ؒf�҂�ID�����F�ς݃Q�X�g���ɔ��������̂ō폜", LostHandle);
			break;
		}
	}
	if (f)
	{
		// �Q�X�g���폜���Ă�����
		// �S���F�ς݃Q�X�g�ɐؒf�����Q�X�g�����邱�Ƃ�m�点��
		Command::Message msgSend = Command::MakeDisconnectGuest(LostHandle);
		for (auto& pair : list)
		{
			int ID = pair.first;
			Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());

			NetWorkSend(ID, &msgSend, Command::SIZEOF_MESSAGE);

			Logger::Info("���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, ID, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);
		}
	}
}
void SceneHost::ProcessReceiveCommand()
{
	// �S���F�ς݃Q�X�g���烁�b�Z�[�W�̎�M���m�F���Ă���
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;

		int DataLength = GetNetWorkDataLength(ID);
		if (DataLength >= Command::SIZEOF_MESSAGE)
		{
			// ���肩�烁�b�Z�[�W�������Ă�����
			Command::Message msg = {};
			NetWorkRecv(ID, &msg, Command::SIZEOF_MESSAGE);

			Logger::Info("���b�Z�[�W����M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, ID, Command::TypeToString(msg.type), msg.single.num, msg.string.text);

			switch (msg.type)
			{
			case Command::Type::MESSAGE:
				// �Q�X�g���`���b�g��ł����Ƃ̂���
				break;
			case Command::Type::CHANGE_NAME_MYSELF:
			{
				// �Q�X�g�����O�̕ύX�������Ƃ̂���

				// �͂��ė������O�ɕύX����
				m_AcceptedGuest->ChangeText(ID, msg.string.text);

				// �S���F�ς݃Q�X�g�ɒm�点��i���O��ς����Q�X�g�{�l�������j
				auto& list2 = m_AcceptedGuest->GetItem();
				for (auto& pair : list2)
				{
					int ID2 = pair.first;
					Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());

					if (ID2 == ID)
					{
						Logger::Info("���O��ύX�����Q�X�g�{�l�Ȃ̂Ń��b�Z�[�W�𑗐M���Ȃ�");
						continue;
					}
					Command::Message msgSend = Command::MakeChangeName(ID, msg.string.text);
					NetWorkSend(ID2, &msgSend, Command::SIZEOF_MESSAGE);

					Logger::Info("���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
						, ID2, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);
				}
			}
			break;
			default:
				break;
			}

		}
		else if (DataLength > 0)
		{
			// ���b�Z�[�W�������Ă������A�Œ蒷���Z���Ƃ�
			// ���b�Z�[�W��ǂݎ̂Ă�
			char Buf[Command::SIZEOF_MESSAGE] = {};
			NetWorkRecv(ID, &Buf, DataLength);

			Logger::Warn("�Œ蒷���Z�����b�Z�[�W���͂����̂Ŏ̂Ă�FNetHandle = %d", ID);
		}
	}
}

void SceneHost::SetPortNum(std::string& Port)
{
	m_Port = std::atoi(Port.c_str());
	Port = std::to_string(m_Port);
}

void SceneHost::SetName(std::string& Name)
{
	if (Name == "")
	{
		Name = m_Name;
	}
	else
	{
		Logger::Info("�z�X�g����ύX�F%s �� %s", m_Name.c_str(), Name.c_str());

		m_Name = Name;
		auto& list = m_AcceptedGuest->GetItem();
		for (auto& pair : list)
		{
			int ID = pair.first;
			Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());

			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			NetWorkSend(ID, &msgSend, Command::SIZEOF_MESSAGE);

			Logger::Info("���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
				, ID, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);
		}
	}
}

void SceneHost::SetAcceptGuest(bool& OffOn)
{
	// �V�K�Q�X�g�̎�t�g�O���̃R�[���o�b�N�֐�
	Logger::Info("�V�K�Q�X�g�̎�t�g�O���F%s�ɕύX�����݂�", OffOn ? "true" : "false");

	if (m_AcceptGuestStep == AcceptGuestStep::OFF)
	{
		// OFF�̂Ƃ��͂��ł�ON�ɂ���OK
		if (OffOn == true)
			m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
	}
	else if (m_AcceptGuestStep == AcceptGuestStep::WAIT_CONNECT)
	{
		// �V�K�Q�X�g���ڑ������݂Ă��Ȃ��Ƃ���OFF�ɂ���OK
		if (OffOn == false)
			m_AcceptGuestStep = AcceptGuestStep::OFF;
	}
	else
	{
		// �V�K�Q�X�g���ڑ������݂Ă���Ƃ���ON���������Ȃ�
		OffOn = true;
	}

	// m_AcceptGuestStep�̒i�K���ǂ̏�Ԃ���\�����x���ɂ��ύX�𔽉f
	m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);

	// DxLib�̐ڑ���t��Ԃ̕ύX
	if (OffOn)
		PreparationListenNetWork(m_Port);
	else
		StopListenNetWork();

	Logger::Info("�V�K�Q�X�g�̎�t�g�O���F%s�ɕύX", OffOn ? "true" : "false");
}

void SceneHost::AcceptGuest()
{
	// �V�K�Q�X�g�̏��F�{�^���̃R�[���o�b�N�֐�
	Logger::Info("���F�{�^������");

	// �v���O������ 100% true �ɂȂ�͂��̏���
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// ���F�t���O�𗧂ĂĎ��̒i�K�ɐi��
		m_AcceptOrReject = true;
		m_AcceptGuestStep = AcceptGuestStep::SELECTED;
		m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
	}
	else
	{
		Logger::Warn("���̃��O�͏o�͂���Ă͂Ȃ�Ȃ��Fm_AcceptGuestStep = %d", m_AcceptGuestStep);
	}
}

void SceneHost::RejectGuest()
{
	// �V�K�Q�X�g�̋��ۃ{�^���̃R�[���o�b�N�֐�
	Logger::Info("���ۃ{�^������");

	// �v���O������ 100% true �ɂȂ�͂��̏���
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// ���ۃt���O�𗧂ĂĎ��̒i�K�ɐi��
		m_AcceptOrReject = false;
		m_AcceptGuestStep = AcceptGuestStep::SELECTED;
		m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
	}
	else
	{
		Logger::Warn("���̃��O�͏o�͂���Ă͂Ȃ�Ȃ��Fm_AcceptGuestStep = %d", m_AcceptGuestStep);
	}
}

void SceneHost::Disconnect()
{
	// �S���F�ς݃Q�X�g�Ƀz�X�g���ؒf���邱�Ƃ�m�点��
	Logger::Info("�S���F�ς݃Q�X�g�Ƀz�X�g���ؒf���邱�Ƃ�m�点��");

	Command::Message msgSend = Command::MakeConnect(false, m_Name);
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());

		NetWorkSend(ID, &msgSend, Command::SIZEOF_MESSAGE);

		Logger::Info("���b�Z�[�W�𑗐M�FNetHandle = %d, msg = {type = %s, single.num = %d, string.text = %s}"
			, ID, Command::TypeToString(msgSend.type), msgSend.single.num, msgSend.string.text);
	}
	m_AcceptedGuest->RemoveAllItem();
}

void SceneHost::End()
{
	Logger::Info("�I���{�^�������Fm_Next = %d", m_Next);

	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}

