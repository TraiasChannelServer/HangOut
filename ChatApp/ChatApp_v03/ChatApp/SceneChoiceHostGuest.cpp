#include "SceneChoiceHostGuest.h"
#include "ScLabel.h"
#include "ScButton.h"
#include "FontManager.h"
#include "SceneCreator.h"
#include "Common.h"
#include "Logger.h"

SceneChoiceHostGuest::SceneChoiceHostGuest()
	: m_Next()
{
	// �R���X�g���N�^�ł͉�ʂ̕��i��z�u����
	Logger::Info("�z�X�g�E�Q�X�g�I����ʐ���");

	int SmallFont = FontManager::Inst().GetFontHandle(FontManager::Type::SMALL);
	int HugeFont = FontManager::Inst().GetFontHandle(FontManager::Type::HUGE);

	AddBaseComponent(new ScLabel("�z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩��I��ł�������", GetColor(0, 255, 0), 10, 10, SmallFont));

	{
		unsigned int ColorFore = GetColor(0, 0, 0);
		unsigned int ColorBack = GetColor(200, 180, 180);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		unsigned int ColorBackHover = GetColor(220, 200, 200);
		unsigned int ColorBackPress = GetColor(180, 160, 160);
		auto CallbackHost = new DelegateVoid<SceneChoiceHostGuest>(*this, &SceneChoiceHostGuest::BeHost);
		AddBaseComponent(new ScButton("�z�X�g", ColorFore, ColorBack, 100, 100, HugeFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
	{
		unsigned int ColorFore = GetColor(0, 0, 0);
		unsigned int ColorBack = GetColor(180, 180, 200);
		unsigned int ColorFrame = GetColor(50, 50, 100);
		unsigned int ColorBackHover = GetColor(200, 200, 220);
		unsigned int ColorBackPress = GetColor(160, 160, 200);
		auto CallbackGuest = new DelegateVoid<SceneChoiceHostGuest>(*this, &SceneChoiceHostGuest::BeGuest);
		AddBaseComponent(new ScButton("�Q�X�g", ColorFore, ColorBack, 100, 200, HugeFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackGuest));
	}
	{
		unsigned int ColorFore = GetColor(0, 0, 0);
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto CallbackHost = new DelegateVoid<SceneChoiceHostGuest>(*this, &SceneChoiceHostGuest::End);
		AddBaseComponent(new ScButton("�I��", ColorFore, ColorBack, 100, 500, HugeFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
}

Scene* SceneChoiceHostGuest::Update()
{
	UpdateBase();
	return m_Next;
}

void SceneChoiceHostGuest::Draw()
{
	ClearDrawScreen();
	DrawBase();
}

void SceneChoiceHostGuest::BeHost()
{
	if (m_Next == nullptr)
		m_Next = SceneCreator::Create(SceneCreator::Name::HOST);
}

void SceneChoiceHostGuest::BeGuest()
{
	if (m_Next == nullptr)
		m_Next = SceneCreator::Create(SceneCreator::Name::GUEST);
}

void SceneChoiceHostGuest::End()
{
	Common::ApplicationExecution = false;
}
