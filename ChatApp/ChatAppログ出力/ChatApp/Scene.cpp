#include "Scene.h"
#include "Logger.h"

Scene::Scene()
	: m_BaseComponent()
{
}

Scene::~Scene()
{
	Logger::Info("シーン基底クラスデストラクタ");
	DeleteComponent(m_BaseComponent);
}

void Scene::AddBaseComponent(ScreenComponent* component)
{
	Logger::Info("画面基本部品追加：component = %d", component);
	m_BaseComponent.push_back(component);
}

void Scene::UpdateComponent(std::vector<ScreenComponent*>& Component)
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		for (auto sc : Component)
		{
			int x = 0;
			int y = 0;
			GetMousePoint(&x, &y);
			sc->OnMousePress(x, y);
		}
	}
	else
	{
		for (auto sc : Component)
		{
			int x = 0;
			int y = 0;
			GetMousePoint(&x, &y);
			sc->OnMouseMove(x, y);
		}
	}
}

void Scene::DrawComponent(const std::vector<ScreenComponent*>& Component) const
{
	for (auto sc : Component)
	{
		sc->Draw();
	}
}

void Scene::DeleteComponent(std::vector<ScreenComponent*>& Component)
{
	for (auto sc : Component)
	{
		Logger::Info("画面部品破棄：component = %d", sc);
		delete sc;
	}
	Component.clear();
}

