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
	enum class Type
	{
		CONNECT,
		MESSAGE,
		CHANGE_NAME_MYSELF,
		CHANGE_NAME,
		NEW_GUEST,
		DISCONNECT_GUEST,
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
	static Message MakeChangeName(int ID, const std::string& Name);
	static Message MakeNewGuest(int ID, const std::string& Name);
	static Message MakeDisconnectGuest(int ID);

	static const char* TypeToString(Type type);

private:
	Command() = delete;
};