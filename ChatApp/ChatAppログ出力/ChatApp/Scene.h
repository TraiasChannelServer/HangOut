#pragma once

#include "ScreenComponent.h"
#include <vector>
#include <map>

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	virtual Scene* Update() = 0;
	virtual void Draw() = 0;

protected:
	// ��{�̉�ʕ��i�̒ǉ��E�X�V�E�`��
	void AddBaseComponent(ScreenComponent* component);
	void UpdateBase();
	void DrawBase() const;

	// �q�N���X�ŗL�̉�ʕ��i�̍X�V�E�`��E�j��
	void UpdateComponent(std::vector<ScreenComponent*>& Component);
	void DrawComponent(const std::vector<ScreenComponent*>& Component) const;
	void DeleteComponent(std::vector<ScreenComponent*>& Component);

private:
	// ��{��ʕ��i
	std::vector<ScreenComponent*> m_BaseComponent;
};

inline void Scene::UpdateBase()
{
	UpdateComponent(m_BaseComponent);
}
inline void Scene::DrawBase() const
{
	DrawComponent(m_BaseComponent);
}
