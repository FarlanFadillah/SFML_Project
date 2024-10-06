#pragma once
#include "Scene.hpp"
#include <EntityManager.hpp>
#include "Entity.hpp"
#include "App.h"
#include "Assets.hpp" 


#include <map>
#include <memory>
#include <App.h>

class App;

using Apps = std::map<std::string, std::shared_ptr<App>>;



class SceneComputer :
    public Scene
{

	std::shared_ptr<Entity> m_closeButton;
	uint64_t			m_lastTyping = 0;
	
	std::string			m_typeText = "";
	EntityManager		m_entityManager;
	sf::Texture			m_backGround;
	
	
	bool				m_drawStats = true;
	bool				m_drawCollision = false;
	bool				m_drawSearchingStatus = false;
	bool				m_appOpen = false;
	
	sf::Text				m_text;

	std::string			m_currentAppName = "";


	sf::Sprite bg;
	




	//APP
	Apps m_apps;




	void init(std::string path);

	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;
	

	void setUpComputer(const std::string path);



public:
	std::vector<std::string> m_stats;
	void closeApp();
	Vec2				m_mousePos = { 0.0f,0.0f };
	SceneComputer(GameEngine* gameEngine, std::string text);
};

