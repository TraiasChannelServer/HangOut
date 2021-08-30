#pragma once

/*
 * ������̃��b�Z�[�W���쐬�E���߂���N���X
 * �f�[�^�̒ʐM�͑S�����̃N���X�����������ōs����OK
 */

#include <string>
#include <vector>

class Command
{
public:
	// �v�f����ς�����TYPE_STRING���ꏏ�ɕύX����
	// TYPE_LENGTH���ύX���Ȃ����C��t����(ERROR_TYPE������)
	enum class Type
	{
		CONNECT,
		MESSAGE,
		CHANGE_NAME,
		NEW_GUEST,
		DISCONNECT_GUEST,
		ERROR_TYPE
	};

	static constexpr char CONNECT_ACCEPT[] = "Accept";
	static constexpr char CONNECT_REJECT[] = "Reject";

private:
	static const char TYPE_STRING[][4];
	static constexpr int TYPE_LENGTH = 3;
	static constexpr int TYPE_ENUM_COUNT = static_cast<int>(Type::ERROR_TYPE);
	static constexpr char SEPARATOR_STRING[] = ", ";
	static constexpr int SEPARATOR_LENGTH = sizeof(SEPARATOR_STRING) - 1;


public:
	static std::vector<std::string> Interpret(const std::string& Message, Type& type);
	static std::string MakeConnect(const std::string& AcceptReject, const std::string& Name);
	static std::string MakeMessage(int ID, const std::string& Message);
	static std::string MakeChangeName(int ID, const std::string& Name);
	static std::string MakeNewGuest(int ID, const std::string& Name);
	static std::string MakeDisconnectGuest(int ID);

private:
	static std::string Type2String(Type type);
	static Type String2Type(const std::string& TypeString);

private:
	Command() = delete;
};