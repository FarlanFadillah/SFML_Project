#pragma once
#include "Scene.hpp"

class SceneLoading :
    public Scene
{
	sf::Text                 m_LoadText;
	sf::Text				 m_stat;
	std::string				 m_text;

	void init();

	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;

public:
	SceneLoading(GameEngine* gameEngine, std::string text);
};

