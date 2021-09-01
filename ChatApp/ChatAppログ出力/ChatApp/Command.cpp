#include "Command.h"
#include "FormatToString.h"
#include <memory>

/*
 * �ڑ����邩���Ȃ������������b�Z�[�W
 * 
 * @param AcceptReject true:����, false:���Ȃ�
 * @param Name         �z�X�g��
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
 * �`���b�g���b�Z�[�W
 *
 * @param ID            �`���b�g���b�Z�[�W�̑��M�҂�ID
 * @param MessageString �`���b�g���b�Z�[�W
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
 * �����̖��O��ύX�������Ƃ�m�点�郁�b�Z�[�W
 *
 * @param Name �ύX��̖��O
 */
Command::Message Command::MakeChangeNameMySelf(const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CHANGE_NAME_MYSELF;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * �z�X�g���Q�X�g�ɖ��O��ύX�����Q�X�g�����邱�Ƃ�m�点�郁�b�Z�[�W
 *
 * @param ID   ���O��ύX�����l��ID
 * @param Name �ύX��̖��O
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
 * �z�X�g���Q�X�g�ɐV�����Q�X�g���ڑ��������Ƃ�m�点�郁�b�Z�[�W
 *
 * @param ID   �V�����Q�X�g��ID
 * @param Name �V�����Q�X�g�̖��O
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
 * �z�X�g���Q�X�g�ɒN���Q�X�g���ؒf�������Ƃ�m�点�郁�b�Z�[�W
 *
 * @param ID �ؒf�����Q�X�g��ID
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
	ErrorText = FormatToString::Make("Command::Type::�͈͊O(%d)", static_cast<int>(type));
	return ErrorText.c_str();
}

