#include "Command.h"

const char Command::TYPE_STRING[][TYPE_LENGTH + 1] =
{
	"000",
	"001",
	"002",
	"003",
	"004",
};

std::vector<std::string> Command::Interpret(const std::string& Message, Type& type)
{
	// https://marycore.jp/prog/cpp/std-string-split/

	if (Message.size() <= TYPE_LENGTH)
	{
		type = Type::ERROR_TYPE;
		return std::vector<std::string>();
	}

	std::string TypeString = Message.substr(0, TYPE_LENGTH - 1);
	type = String2Type(TypeString);

	if (type == Type::ERROR_TYPE)
	{
		return std::vector<std::string>();
	}

	std::string string = Message.substr(TYPE_LENGTH);

	std::vector<std::string> list;

	auto offset = std::string::size_type(0);
	while (1)
	{
		auto pos = string.find(SEPARATOR_STRING, offset);
		if (pos == std::string::npos)
		{
			list.push_back(string.substr(offset));
			break;
		}
		list.push_back(string.substr(offset, pos - offset));
		offset = pos + SEPARATOR_LENGTH;
	}
	return list;
}

std::string Command::MakeConnect(const std::string& AcceptReject, const std::string& Name)
{
	return Type2String(Type::CONNECT)
		.append(AcceptReject)
		.append(SEPARATOR_STRING)
		.append(Name);
}

std::string Command::MakeMessage(int ID, const std::string& Message)
{
	return Type2String(Type::MESSAGE)
		.append(std::to_string(ID))
		.append(SEPARATOR_STRING)
		.append(Message);
}

std::string Command::MakeChangeName(int ID, const std::string& Name)
{
	return Type2String(Type::CHANGE_NAME)
		.append(std::to_string(ID))
		.append(SEPARATOR_STRING)
		.append(Name);
}

std::string Command::MakeNewGuest(int ID, const std::string& Name)
{
	return Type2String(Type::NEW_GUEST)
		.append(std::to_string(ID))
		.append(SEPARATOR_STRING)
		.append(Name);
}

std::string Command::MakeDisconnectGuest(int ID)
{
	return Type2String(Type::DISCONNECT_GUEST)
		.append(std::to_string(ID));
}

std::string Command::Type2String(Type type)
{
	return TYPE_STRING[static_cast<int>(type)];
}

Command::Type Command::String2Type(const std::string& TypeString)
{
	int num = std::stoi(TypeString);
	if (0 <= num && num < TYPE_ENUM_COUNT)
		return Type(num);
	return Type::ERROR_TYPE;
}
