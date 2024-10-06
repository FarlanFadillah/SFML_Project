#pragma once
#include "GameEngine.hpp"
#include "Scene.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "EntityManager.hpp"

class SceneMenu : public Scene
{
private:
	
	std::string              m_title;
	std::vector<std::string> m_menuStrings;
	std::vector<std::string> m_levelPaths;
	sf::Text                 m_menuText;
	std::vector<std::string> m_buttonName;

	std::vector<const char*> m_buttons;
	
	size_t                   m_selectedMenuIndex = 0;
	EntityManager            m_entityManager;

	Vec2					mPos;


	void init();

	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;

	void setButton(std::vector<std::string>& name);

	void renderButton(std::vector<const char*>& name);

public:
	SceneMenu(GameEngine* gameEngine);
};