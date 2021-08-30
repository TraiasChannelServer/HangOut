#pragma once

/*
 * 文字列のメッセージを作成・解釈するクラス
 * データの通信は全部このクラスを介した文字列で行えばOK
 */

#include <string>
#include <vector>

class Command
{
public:
	// 要素数を変えたらTYPE_STRINGも一緒に変更する
	// TYPE_LENGTHも変更がないか気を付ける(ERROR_TYPEを除く)
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