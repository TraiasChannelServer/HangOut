#include "SceneHost.h"
#include "ScButton.h"
#include "ScEdit.h"
#include "ScToggle.h"
#include "FontManager.h"
#include "BoundRect.h"
#include "Common.h"
#include "SceneCreator.h"
#include "Command.h"

const char SceneHost::ACCEPT_STEP_STATE[][64] =
{
	"新規ゲストは受け付けていません",
	"新規ゲストを受け付け中です...",
	"新規ゲストが訪問しています...",
	"以下のゲストが訪問してきました",
	"処理中..."
};

SceneHost::SceneHost()
	: m_AcceptRejectComponent()
	, m_Next()
	, m_Port(9850)
	, m_Name("Host")
	, m_AcceptGuestStep()
	, m_AcceptState()
	, m_NewGuestIP()
	, m_NewGuestName()
	, m_NewGuestNetHandle(-1)
	, m_AcceptOrReject()
	, m_AcceptedGuest()
{
	// コンストラクタでは画面の部品を配置する

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

	AddBaseComponent(new ScLabel("あなたはホストです", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("ポート番号：", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetPortNum);
		AddBaseComponent(new ScEdit(std::to_string(m_Port), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 5, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("名前：", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetName);
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 16, Callback));
	}

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("新規ゲストの受付：", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(170, y, 80, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, bool>(*this, &SceneHost::SetAcceptGuest);
		AddBaseComponent(new ScToggle(ToggleColorFore, ToggleColorBack, bound, SmallFont, Callback));
	}
	y += Y_STEP_SMALL * 2;
	m_AcceptState = new ScLabel(ACCEPT_STEP_STATE[0], RedColor, 10, y, MiddleFont);
	AddBaseComponent(m_AcceptState);
	y += Y_STEP_MIDDLE;
	m_AcceptRejectComponent.push_back(new ScLabel("IP：", GreenColor, X_START, y, SmallFont));
	m_NewGuestIP = new ScLabel("xxx.xxx.xx.xxx", GreenColor, 80, y, SmallFont);
	m_AcceptRejectComponent.push_back(m_NewGuestIP);
	y += Y_STEP_SMALL;
	m_AcceptRejectComponent.push_back(new ScLabel("名前：", GreenColor, X_START, y, SmallFont));
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
		m_AcceptRejectComponent.push_back(new ScButton("承認", ColorFore, ColorBack, X_START + 10, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(220, 180, 180);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		unsigned int ColorBackHover = GetColor(240, 200, 200);
		unsigned int ColorBackPress = GetColor(200, 160, 160);
		auto CallbackHost = new DelegateVoid<SceneHost>(*this, &SceneHost::RejectGuest);
		m_AcceptRejectComponent.push_back(new ScButton("拒否", ColorFore, ColorBack, X_START + 110, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}

	y += Y_STEP_BIG + Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("承認済みゲスト：", GreenColor, X_START, y, SmallFont));

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
		AddBaseComponent(new ScButton("終了", ColorFore, ColorBack, X_START, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
}

SceneHost::~SceneHost()
{
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

void SceneHost::ProcessNewGuest()
{
	switch (m_AcceptGuestStep)
	{
	case AcceptGuestStep::OFF:
		// 新規ゲストの受付OFFのとき
		break;
	case AcceptGuestStep::WAIT_CONNECT:
		// 新規ゲストの受付ONのとき
		// 新規接続を待つ
		m_NewGuestNetHandle = GetNewAcceptNetWork();
		if (m_NewGuestNetHandle != -1)
		{
			// 新しい接続を発見したら
			IPDATA ip = {};
			GetNetWorkIP(m_NewGuestNetHandle, &ip);
			// 相手のIPを保持
			m_NewGuestIP->ChangeText(Common::IP2String(ip));
			// 処理をAcceptGuestStep::REQUEST_NAMEへ進む
			m_AcceptGuestStep = AcceptGuestStep::REQUEST_NAME;
		}
		break;
	case AcceptGuestStep::REQUEST_NAME:
	{
		// 接続してきた相手はまず自分の名前を名乗るはずなので
		// それを待つ
		int DataLength = GetNetWorkDataLength(m_NewGuestNetHandle);
		if (DataLength != 0)
		{
			// 相手からメッセージが送られてきたら
			char StrBuf[256] = {};
			NetWorkRecv(m_NewGuestNetHandle, StrBuf, DataLength);

			// メッセージを解釈する
			Command::Type type = {};
			auto Message = Command::Interpret(StrBuf, type);

			if (type == Command::Type::CHANGE_NAME)
			{
				// 相手が名乗ってきたら
				std::string ID = Message[0];
				std::string Name = Message[1];
				// 相手の名前を保持
				m_NewGuestName->ChangeText(Name);
				// 処理をAcceptGuestStep::SELECT_ACCEPT_REJECTへ進む
				m_AcceptGuestStep = AcceptGuestStep::SELECT_ACCEPT_REJECT;
			}
			else
			{
				// 相手が意味不明なメッセージを送ってきたら
				// 拒否メッセージを送信
				std::string Message = Command::MakeConnect(Command::CONNECT_REJECT, m_Name);
				NetWorkSend(m_NewGuestNetHandle, Message.c_str(), Message.size());
				// 処理をAcceptGuestStep::WAIT_CONNECTへ戻る
				m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
			}
		}
	}
	break;
	case AcceptGuestStep::SELECT_ACCEPT_REJECT:
		// 接続を求めてきた相手のNetHandleと名前が分かったら
		// 承認・拒否を選択する画面部品を表示（ここでは表示ではなく処理）する
		UpdateComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		// 承認・拒否が選択されたら（承認・拒否の決定の瞬間はコールバック関数）
		if (m_AcceptOrReject)
		{
			// 承認する場合
			// 承認メッセージを送信（このときホストも名乗り返す）
			std::string Message = Command::MakeConnect(Command::CONNECT_ACCEPT, m_Name);
			NetWorkSend(m_NewGuestNetHandle, Message.c_str(), Message.size());

			// 承認済みゲストに追加
			m_AcceptedGuest->AddItem(m_NewGuestNetHandle, m_NewGuestName->GetText());

			// 全承認済みゲストに新規ゲストの接続を知らせる（ゲスト本人を含む）
			auto& list = m_AcceptedGuest->GetItem();
			for (auto& pair : list)
			{
				int ID = pair.first;
				std::string Message = Command::MakeNewGuest(ID, m_NewGuestName->GetText());
				NetWorkSend(ID, Message.c_str(), Message.size());
			}
		}
		else
		{
			// 拒否する場合
			// 拒否メッセージを送信
			std::string Message = Command::MakeConnect(Command::CONNECT_REJECT, m_Name);
			NetWorkSend(m_NewGuestNetHandle, Message.c_str(), Message.size());
		}
		// 処理をAcceptGuestStep::WAIT_CONNECTへ戻る
		m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
		break;
	}
}

void SceneHost::ProcessDissconectGuest()
{
	// ゲストはいつ切断してくるか分からないので常に確認しておく
	bool f = false;
	int LostHandle = GetLostNetWork();
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		if (LostHandle == ID)
		{
			// 切断してるゲストを発見したら
			// 承認済みゲストから削除
			m_AcceptedGuest->RemoveItem(ID);
			f = true;
			break;
		}
	}
	if (f)
	{
		// ゲストを削除していたら
		// 全承認済みゲストに切断したゲストがいることを知らせる
		std::string Message = Command::MakeDisconnectGuest(LostHandle);
		for (auto& pair : list)
		{
			int ID = pair.first;
			NetWorkSend(ID, Message.c_str(), Message.size());
		}
	}
}
void SceneHost::ProcessReceiveCommand()
{
	// 全承認済みゲストからメッセージの受信を確認していく
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;

		int DataLength = GetNetWorkDataLength(ID);
		if (DataLength != 0)
		{
			// メッセージを受信していたら
			char StrBuf[256] = {};
			NetWorkRecv(ID, StrBuf, DataLength);

			// メッセージを解釈する
			Command::Type type = {};
			auto Message = Command::Interpret(StrBuf, type);

			switch (type)
			{
			case Command::Type::MESSAGE:
				// ゲストがチャットを打ったとのこと
				break;
			case Command::Type::CHANGE_NAME:
			{
				// ゲストが名前の変更をしたとのこと
				int id = std::stoi(Message[0]);
				std::string Name = Message[1];
				auto& list = m_AcceptedGuest->GetItem();
				// プログラム上100%一致するはずの条件
				if (id == ID)
				{
					// 届いて来た名前に変更する
					m_AcceptedGuest->ChangeText(ID, Name);

					// 他の承認済みゲストにも知らせる
					auto& list = m_AcceptedGuest->GetItem();
					for (auto& pair : list)
					{
						int ID2 = pair.first;
						if (ID2 == ID)
						{
							// 名前を変更したゲスト本人には送らない
							continue;
						}
						std::string Message = Command::MakeChangeName(ID2, Name);
						NetWorkSend(ID2, Message.c_str(), Message.size());
					}
				}
			}
			break;
			case Command::Type::ERROR_TYPE:
				break;
			}

		}
	}
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
		// 接続を求めてきた相手のNetHandleと名前が分かったら
		// 承認・拒否を選択する画面部品を表示する
		DrawComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		break;
	}
}

void SceneHost::End()
{
	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}

void SceneHost::Disconnect()
{
	std::string Message = Command::MakeConnect(Command::CONNECT_REJECT, m_Name);
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		NetWorkSend(ID, Message.c_str(), Message.size());
	}
	m_AcceptedGuest->RemoveAllItem();
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
		m_Name = Name;
		auto& list = m_AcceptedGuest->GetItem();
		for (auto& pair : list)
		{
			int ID = pair.first;
			std::string Message = Command::MakeChangeName(ID, m_Name);
			NetWorkSend(ID, Message.c_str(), Message.size());
		}
	}
}

void SceneHost::SetAcceptGuest(bool& OffOn)
{
	// 新規ゲストの受付トグルのコールバック関数

	if (m_AcceptGuestStep == AcceptGuestStep::OFF)
	{
		// OFFのときはいつでもONにしてOK
		if (OffOn)
			m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
	}
	else if (m_AcceptGuestStep == AcceptGuestStep::WAIT_CONNECT)
	{
		// 新規ゲストが接続を試みていないときはOFFにしてOK
		if (OffOn == false)
			m_AcceptGuestStep = AcceptGuestStep::OFF;
	}
	else
	{
		// 新規ゲストが接続を試みているときはONしか許さない
		OffOn = true;
	}

	// m_AcceptGuestStepの段階がどの状態かを表すラベルにも変更を反映
	m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);

	// DxLibの接続受付状態の変更
	if (OffOn)
		PreparationListenNetWork(m_Port);
	else
		StopListenNetWork();
}

void SceneHost::AcceptGuest()
{
	// 新規ゲストの承認ボタンのコールバック関数

	// プログラム上 100% true になるはずの条件
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// 承認フラグを立てて次の段階に進む
		m_AcceptOrReject = true;
		m_AcceptGuestStep = AcceptGuestStep::SELECTED;
		m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
	}
}

void SceneHost::RejectGuest()
{
	// 新規ゲストの拒否ボタンのコールバック関数

	// プログラム上 100% true になるはずの条件
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// 拒否フラグを立てて次の段階に進む
		m_AcceptOrReject = false;
		m_AcceptGuestStep = AcceptGuestStep::SELECTED;
		m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
	}
}


