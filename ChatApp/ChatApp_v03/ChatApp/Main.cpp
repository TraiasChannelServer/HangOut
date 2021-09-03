#include "DxLib.h"
#include "SceneCreator.h"
#include "FontManager.h"
#include "Common.h"
#include "Logger.h"
#include "DataFileInOut.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	// DxLib_Init�֐��̑O�ɌĂԂׂ�DxLib�̐ݒ�֐��Q
	SetOutApplicationLogValidFlag(TRUE);
	ChangeWindowMode(TRUE);
	SetMainWindowText("ChatApp");
	SetWindowSizeChangeEnableFlag(TRUE);
	SetDoubleStartValidFlag(FALSE);
	SetGraphMode(Common::WINDOW_X_SIZE, Common::WINDOW_Y_SIZE, 16);
	//SetBackgroundColor(0xf4, 0xf5, 0xf7); // beautiful white
	SetBackgroundColor(0x15, 0x18, 0x1e); // beautiful black
	SetAlwaysRunFlag(TRUE);

	// DxLib�̏�����
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// �ݒ�f�[�^�ǂݍ���
	DataFileInOut::Inst().Input();

	// �t�H���g�̏�����
	FontManager::Inst().Initialize();

	// �ŏ��ɕ\������V�[���𐶐�
	Scene* scene = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);

	Logger::Info("���C�����[�v�J�n");

	// ���C�����[�v
	while (ProcessMessage() == 0 && Common::ApplicationExecution == true) {
		// ���݂̃V�[�����X�V
		Scene* next = scene->Update();
		scene->Draw();

		if (next != nullptr)
		{
			// ���݂̃V�[����j��
			delete scene;
			// ���̃V�[���Ɉڂ�
			scene = next;
		}

		// Debug���}���u�F��ʕ\���ŏ����̃t���[�Y��m�邱�Ƃ��ł���
		{
			static int FrameCount = 0;
			static int Color = GetColor(255, 0, 0);
			DrawString(Common::WINDOW_X_SIZE - 100, 0, std::to_string(++FrameCount).c_str(), Color);
		}

		// �`��
		ScreenFlip();

		// CPU�g�p����������(60FPS���v��Ȃ��Ǝv������)
		WaitTimer(40);
	}

	Logger::Info("���C�����[�v�I��");

	// �V�[����j��
	delete scene;
	scene = nullptr;

	// �t�H���g�̔j��
	FontManager::Inst().Destroy();

	// �ݒ�f�[�^��������
	DataFileInOut::Inst().Output();

	// DxLib�̏I������
	DxLib_End();

	return 0;
}

/*
https://27bit.com/question-number.html
�|�[�g�ԍ���0�`65535�܂ł���܂����A���ׂĎg�p�o����킯�ł͂���܂���B
�|�[�g�ԍ��͑傫��3�ɂ킩��A0�`1023�܂ł̓E�F���m�E���|�[�g�ƌ����A���߂��Ă�����̂������A�g�p�o���Ȃ����̂����X����܂��B
�Ⴆ�΁A�����������i�g���Ă���C���^�[�l�b�g�A�����80�Ԃ̃|�[�g�ԍ����g�p���Ă��܂��B���̂ق��ɂ́A���[���̎�M�|�[�g��110�ԁB���M�|�[�g��25�ԂƂ����悤�Ɍ��܂������̂��g���Ă��܂��B
1024�`49512�܂ł̔ԍ��́A�o�^�|�[�g�ƌ����A�����̃\�t�g�E�F�A�ɂ���Ċ��蓖�Ă��܂��B�g�p�\�Ȃ��̂�����܂����A���炩�̃\�t�g�E�F�A�Əd������\��������܂��B
49513�`65535�܂ł̔ԍ��́A�_�C�i�~�b�N/�v���C�x�[�g�|�[�g�ƌ����A��{�I�ɂ́A���̊Ԃ����R�Ɏg����|�[�g�ԍ��ł��B
*/
