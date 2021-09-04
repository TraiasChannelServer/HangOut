#include "Command.h"
#include "FormatToString.h"
#include "Logger.h"
#include "DxLib.h"
#include <memory>

/*
 * �z�X�g���Q�X�g�ɐڑ��������邩���Ȃ������������b�Z�[�W
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
Command::Message Command::MakeChangeNameGuest(int ID, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CHANGE_NAME_GUEST;
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

/*
 * �Q�X�g���z�X�g�ɑS�f�[�^���ēǂݍ��݂��������Ƃ�m�点�郁�b�Z�[�W
 *
 * �z�X�g�̓Q�X�g�Ƀ��b�Z�[�W�𑗂�܂���̂�
 * �Q�X�g�͎��g�̃f�[�^�ȊO�S���������đҋ@����
 */
Command::Message Command::MakeAllUpdate()
{
	Message msg = {};
	msg.type = Type::ALL_UPDATE;
	return msg;
}

/*
 * ���b�Z�[�W�𑗐M����
 */
void Command::Send(int NetHandle, const Message& Msg)
{
	NetWorkSend(NetHandle, &Msg, Command::SIZEOF_MESSAGE);

	Logger::Info("���b�Z�[�W�𑗐M�FNetHandle = %d, Msg = {Type = %s, Single = %d, String = %s}"
		, NetHandle, Command::TypeToString(Msg.type), Msg.single.num, Msg.string.text);
}

/*
 * ���b�Z�[�W����M���Ă��邩���m�F����
 * 
 * @return ReceiveResult::NONE     �c ��M���Ă��Ȃ�
 *         ReceiveResult::SUCCESS  �c ��M���Ă���
 *         ReceiveResult::ABNORMAL �c �Œ蒷�����̃��b�Z�[�W����M�����̂Ŏ̂Ă�
 */
Command::ReceiveResult Command::CheckReceive(int NetHandle)
{
	int DataLength = GetNetWorkDataLength(NetHandle);
	if (DataLength >= Command::SIZEOF_MESSAGE)
	{
		// �Œ蒷�ȏ�̃��b�Z�[�W�������Ă����Ƃ�
		return ReceiveResult::SUCCESS;
	}
	else if (DataLength > 0)
	{
		// �Œ蒷�����̃��b�Z�[�W�������Ă����Ƃ�
		// ���b�Z�[�W��ǂݎ̂Ă�
		char Buf[Command::SIZEOF_MESSAGE] = {};
		NetWorkRecv(NetHandle, &Buf, DataLength);
		Logger::Warn("�Œ蒷���Z�����b�Z�[�W���͂����̂Ŏ̂Ă�FNetHandle = %d", NetHandle);

		return ReceiveResult::ABNORMAL;
	}
	return ReceiveResult::NONE;
}

/*
 * ���b�Z�[�W����M����
 */
Command::Message Command::Receive(int NetHandle)
{
	Command::Message Msg = {};
	NetWorkRecv(NetHandle, &Msg, Command::SIZEOF_MESSAGE);

	Logger::Info("���b�Z�[�W����M�FNetHandle = %d, Msg = {Type = %s, Single = %d, String = %s}"
		, NetHandle, Command::TypeToString(Msg.type), Msg.single.num, Msg.string.text);

	return Msg;
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
	case Type::CHANGE_NAME_GUEST:
		return "Command::Type::CHANGE_NAME_GUEST";
	case Type::NEW_GUEST:
		return "Command::Type::NEW_GUEST";
	case Type::DISCONNECT_GUEST:
		return "Command::Type::DISCONNECT_GUEST";
	case Type::ALL_UPDATE:
		return "Command::Type::ALL_UPDATE";
	}

	static std::string ErrorText;
	ErrorText = FormatToString::Make("Command::Type::�͈͊O(%d)", static_cast<int>(type));
	return ErrorText.c_str();
}

