#pragma once

/*
 * ����M���郁�b�Z�[�W���쐬����N���X
 * ���b�Z�[�W�̒����͌Œ蒷
 */

#include <string>
#include <vector>

class Command
{
public:
	// ������X�V������static const char* TypeToString(Type type)���X�V���邱��
	enum class Type
	{
		CONNECT,
		MESSAGE,
		CHANGE_NAME_MYSELF,
		CHANGE_NAME_GUEST,
		NEW_GUEST,
		DISCONNECT_GUEST,
		ALL_UPDATE,
	};

	enum class ReceiveResult
	{
		NONE,
		SUCCESS,
		ABNORMAL
	};

	union SingleData
	{
		int num = 0;
		bool flag;
	};
	union StringData
	{
		static constexpr int TEXT_SIZE = 100;
		char text[TEXT_SIZE] = {};
	};

	struct Message
	{
		Type type = Type::CONNECT;
		SingleData single;
		StringData string;
	};
	
	// ���b�Z�[�W�̒����͌Œ蒷
	static constexpr int SIZEOF_MESSAGE = sizeof(Message);

public:
	static Message MakeConnect(bool AcceptReject, const std::string& Name);
	static Message MakeMessage(int ID, const std::string& MessageString);
	static Message MakeChangeNameMySelf(const std::string& Name);
	static Message MakeChangeNameGuest(int ID, const std::string& Name);
	static Message MakeNewGuest(int ID, const std::string& Name);
	static Message MakeDisconnectGuest(int ID);
	static Message MakeAllUpdate();

	static void Send(int NetHandle, const Message& Msg);
	static ReceiveResult CheckReceive(int NetHandle);
	static Message Receive(int NetHandle);

	static const char* TypeToString(Type type);

private:
	Command() = delete;
};