#include "Command.h"
#include "FormatToString.h"
#include <memory>

/*
 * 接続するかしないかを示すメッセージ
 * 
 * @param AcceptReject true:する, false:しない
 * @param Name         ホスト名
 */
Command::Message Command::MakeConnect(bool AcceptReject, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CONNECT;
	msg.single.flag = AcceptReject;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * チャットメッセージ
 *
 * @param ID            チャットメッセージの送信者のID
 * @param MessageString チャットメッセージ
 */
Command::Message Command::MakeMessage(int ID, const std::string& MessageString)
{
	Message msg = {};
	msg.type = Type::MESSAGE;
	msg.single.num = ID;
	memcpy_s(msg.string.text, sizeof(msg.string.text), MessageString.c_str(), MessageString.size());
	return msg;
}

/*
 * 自分の名前を変更したことを知らせるメッセージ
 *
 * @param Name 変更後の名前
 */
Command::Message Command::MakeChangeNameMySelf(const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CHANGE_NAME_MYSELF;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * ホストがゲストに名前を変更したゲストがいることを知らせるメッセージ
 *
 * @param ID   名前を変更した人のID
 * @param Name 変更後の名前
 */
Command::Message Command::MakeChangeName(int ID, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CHANGE_NAME;
	msg.single.num = ID;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * ホストがゲストに新しいゲストが接続したことを知らせるメッセージ
 *
 * @param ID   新しいゲストのID
 * @param Name 新しいゲストの名前
 */
Command::Message Command::MakeNewGuest(int ID, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::NEW_GUEST;
	msg.single.num = ID;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * ホストがゲストに誰かゲストが切断したことを知らせるメッセージ
 *
 * @param ID 切断したゲストのID
 */
Command::Message Command::MakeDisconnectGuest(int ID)
{
	Message msg = {};
	msg.type = Type::DISCONNECT_GUEST;
	msg.single.num = ID;
	return msg;
}

const char* Command::TypeToString(Type type)
{
	switch (type)
	{
	case Type::CONNECT:
		return "Command::Type::CONNECT";
	case Type::MESSAGE:
		return "Command::Type::MESSAGE";
	case Type::CHANGE_NAME_MYSELF:
		return "Command::Type::CHANGE_NAME_MYSELF";
	case Type::CHANGE_NAME:
		return "Command::Type::CHANGE_NAME";
	case Type::NEW_GUEST:
		return "Command::Type::NEW_GUEST";
	case Type::DISCONNECT_GUEST:
		return "Command::Type::DISCONNECT_GUEST";
	}

	static std::string ErrorText;
	ErrorText = FormatToString::Make("Command::Type::範囲外(%d)", static_cast<int>(type));
	return ErrorText.c_str();
}

