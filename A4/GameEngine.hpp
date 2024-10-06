#pragma once

#include "Assets.hpp"
#include "Scene.hpp"
#include "DateSystem.h"
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

class Scene; // Resolve circular dependency
using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;
struct WindowConfig { int W, H, FL, FS; std::string title; };



class GameEngine
{
protected:

	DateSystem m_sDate;
	sf::RenderWindow m_window;
	
	std::string      m_currentScene;
	bool             m_running      = true;

	void                   init(const std::string& filePath);
	void                   update();
	void                   sUserInput();

	float currentFrame = 0.0f;

public:
	sf::Clock deltaClock; 
	sf::Font ostrich_regular;
	std::mutex sceneMutex;
	std::shared_ptr<Scene> currentScene();
	Assets				m_assets;
	WindowConfig		m_windowConfig;
	std::map<std::string, std::shared_ptr<Scene>>			m_SceneMap;
	GameEngine(const std::string& filePath);

	bool              isRunning() const;
	const Assets&     assets()    const;
	sf::RenderWindow& window();
	void			  load(std::string title, std::string sceneName);

	DateSystem& getDateSystem();
	void run();
	void quit();
	void changeScene(const std::string& sceneName,
	                 std::shared_ptr<Scene> scene);

	void loadAssets(std::string sceneName);

	void setFullScreen();

	std::string getCurrentSceneName();

};