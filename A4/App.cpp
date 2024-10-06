#include "App.h"
#include "Scene_Computer.h"
#include <sstream>

void App::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

std::string App::formatNumber(int number)
{
	std::ostringstream oss;

	if (number < 10 && number >= 0)
	{
		oss << '0';
	}


	oss << number;

	return oss.str();
}

App::App()
{
}

App::App(SceneComputer* sceneComputer)
	:m_scene(sceneComputer)
{
}




bool App::isInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto& scale = e->getComponent<CTransform>().scale;
	auto& size = e->getComponent<CBoundingBox>().size;
	auto& ePos = e->getComponent<CTransform>().pos;

	Vec2 rad(size.x, size.y);

	float distX = fabs(pos.x - ePos.x);
	float distY = fabs(pos.y - ePos.y);

	return (distX < rad.x / 2 && distY < rad.y / 2);

}

void App::buttonPressed(Vec2 pos, const std::string& name, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities(name))
	{
		auto& eButton = e->getComponent<CButton>();
		if (isInside(pos, e))
		{
			eButton.clicked = m_mouseOnClick;
			if (!m_mouseOnClick)
			{
				sDoAction(eButton.action);
			}
		}
		else
		{
			eButton.clicked = false;
		}
	}
}

void App::buttonHover(Vec2& pos, const std::string& name, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities(name))
	{
		auto& eButton = e->getComponent<CButton>();
		eButton.mouseOver = isInside(pos, e);
	}
}


