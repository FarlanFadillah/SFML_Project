#include "Action.hpp"
#include "GameEngine.hpp"
#include "Scene_Menu.hpp"
#include "Scene_Play.hpp"
#include "Scene_MapEditor.hpp"
#include "Scene_Loading.hpp"
#include "Scene_Computer.h"
#include "Scene_Interview.h"
#include <iostream>
#include "Windows.h"

#include <fstream>
#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>


GameEngine::GameEngine(const std::string& filePath)
{
	//load font
	init(filePath);
	m_assets.loadFromFile("res/fonts/fonts.ini");
	ostrich_regular.loadFromFile("res/fonts/ostrich-regular.ttf");
}

void GameEngine::init(const std::string& filePath)
{
	std::ifstream fin(filePath);
	std::string token;

	while (fin.good())
	{
		fin >> token >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS >> m_windowConfig.title; 
	}
	if (m_windowConfig.FS)
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), m_windowConfig.title,
		                sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), m_windowConfig.title, sf::Style::Close);
	}
	ImGui::SFML::Init(m_window);
	ImGui::GetStyle().ScaleAllSizes(2.0f);
	ImGui::GetIO().FontGlobalScale = 1.5f;
	m_window.setFramerateLimit(m_windowConfig.FL); 

	


	//date
	m_sDate.loadDate("res/config/date.ini");
	load("Loading", "INTERVIEW_SCENE");
	// Change scene
	//changeScene("MENU", std::make_shared<SceneMenu>(this));
	//changeScene("PLAY", std::make_shared<ScenePlay>(this, "res/images/tile/Map2.tmx"));
	//changeScene("MAP_EDITOR", std::make_shared<SceneMapEditor>(this, "res/images/tile/Map2.tmx"));
}

void GameEngine::update()
{
	//m_sDate.update(currentFrame);
	sUserInput();
	ImGui::SFML::Update(m_window, deltaClock.restart());
	currentScene()->update();
	currentFrame++;
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_SceneMap[m_currentScene];
}

// System: User input
void GameEngine::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
		{
			quit();
		}
		else if (event.type == sf::Event::KeyPressed ||
		         event.type == sf::Event::KeyReleased)
		{
			// If current scene doesn't have an action associated with this key, skip it
			if (currentScene()->getActionMap().find(event.key.code) ==
			    currentScene()->getActionMap().end())
			{
				continue;
			}

			// Determine start or end action by whether key was pressed or released
			const std::string& actionType =
				(event.type == sf::Event::KeyPressed) ? "START" : "END";

			//std::cout << currentFrame << " " << actionType << "\n";
			//std::cout << event.key.code << "\n";

			// Look up the action and send it to current scene
			if (m_currentScene.find("COMPUTER") != std::string::npos)
			{
				currentScene()->sDoAction(
					Action(
						currentScene()->getActionMap().at(event.key.code),
						actionType, event.key.code
					)
				);
			}
			else
			{
				currentScene()->sDoAction(
					Action(
						currentScene()->getActionMap().at(event.key.code),
						actionType
					)
				);
			}
		}
		auto mousePos = sf::Mouse::getPosition(m_window);
		Vec2 mPos(mousePos.x, mousePos.y);
		if (event.type == sf::Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left: { currentScene()->sDoAction(Action("LEFT_CLICK", "START", mPos)); break; }
			case sf::Mouse::Middle: { currentScene()->sDoAction(Action("MIDDLE_CLICK", "START", mPos)); break; }
			case sf::Mouse::Right: { currentScene()->sDoAction(Action("RIGHT_CLICK", "START", mPos)); break; }
			default:
				break;
			}
			
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left: { currentScene()->sDoAction(Action("LEFT_CLICK", "END", mPos)); break; }
			case sf::Mouse::Middle: { currentScene()->sDoAction(Action("MIDDLE_CLICK", "END", mPos)); break; }
			case sf::Mouse::Right: { currentScene()->sDoAction(Action("RIGHT_CLICK", "END", mPos)); break; }
			default:
				break;
			}

		}

		if (event.type == sf::Event::MouseMoved)
		{
			currentScene()->sDoAction(Action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
		}
		
	}
	if (!m_window.pollEvent(event))
	{
	}
}

void GameEngine::load(std::string title, std::string sceneName)
{
	changeScene("LOADING", std::make_shared<SceneLoading>(this, title));

	/*
	m_assets.loadFromFile("res/config/assets.ini");
	m_assets.loadFromFile("res/images/character/Emily/Emily_Assets.ini");
	changeScene("PLAY", std::make_shared<CrimeScene>(this, "res/images/tile/Miami_Metro_Office.tmx"));*/
	//changeScene("COMPUTER", std::make_shared<SceneComputer>(this, "res/images/tile/Miami_Metro_Office.tmx"));


	//MultiThread for loading screen, while load assets
	// Pointer ke anggota fungsi

	// Membuat thread dan menjalankan loadAssets melalui pointer
	// Correctly create and manage the thread
	std::thread t(&GameEngine::loadAssets, this, sceneName); 
	t.detach(); // Ensure that the thread does not access destroyed objects 
}

bool GameEngine::isRunning() const
{
	return m_running && m_window.isOpen();
}

const Assets& GameEngine::assets() const
{
	return m_assets;
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

DateSystem& GameEngine::getDateSystem()
{
	return m_sDate;
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
	ImGui::SFML::Shutdown();
	m_window.close();
}

void GameEngine::quit()
{
	m_running = false;
	m_sDate.saveDate();
}

void GameEngine::changeScene(const std::string& sceneName,
                             std::shared_ptr<Scene> scene)
{
	m_SceneMap[sceneName] = scene;
	m_currentScene = sceneName;
	m_window.setView(m_window.getDefaultView());
}

void GameEngine::loadAssets(std::string sceneName)
{
	m_assets.loadFromFile("res/config/Assets.ini");
	changeScene(sceneName, std::make_shared<SceneMenu>(this));

	/*if (sceneName == "OFFICE_PLAY")
	{
		if (m_lastScene != "OFFICE_COMPUTER")
		{
			m_assets.clear();
			changeScene(sceneName, std::make_shared<OfficeScene>(this, "res/images/tile/Miami_Metro_Office.tmx"));
		}
		else if (m_lastScene == "OFFICE_COMPUTER")
		{
			changeScene(sceneName, m_SceneMap.at(sceneName));
		}
		m_lastScene = sceneName;
	}else if (sceneName == "CRIMESCENE_PLAY")
	{
		if (m_lastScene != "CRIMESCENE_COMPUTER")
		{
			m_assets.clear();
			m_assets.loadFromFile("res/scene/office/assets.ini");
			changeScene(sceneName, std::make_shared<CrimeScene>(this, "res/map/crime_scene/01/Dexter_Apart.tmx"));
		}
		else if (m_lastScene == "CRIMESCENE_COMPUTER")
		{
			changeScene(sceneName, m_SceneMap.at(sceneName));
		}
		m_lastScene = sceneName;
	}
	else if (sceneName == "MENU")
	{
		changeScene(sceneName, std::make_shared<SceneMenu>(this));
		m_lastScene = sceneName;
	}
	else if (sceneName == "OFFICE_COMPUTER")
	{
		m_assets.loadFromFile("res/scene/computer_office/assets.ini"); 
		changeScene(sceneName, std::make_shared<SceneComputer>(this, "Computer"));
		m_lastScene = sceneName;
	}
	else if (sceneName == "INTERVIEW_SCENE")
	{
		m_assets.clear();
		m_assets.loadFromFile("res/scene/interview/assets.ini");
		changeScene(sceneName, std::make_shared<SceneInterview>(this, "res/scene/interview/GUI.ini", "res/config/dialogue/Abigail/interview_1.json"));
		m_lastScene = sceneName;
	}*/
}

void GameEngine::setFullScreen()
{
	if (m_windowConfig.FS)
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), m_windowConfig.title,
			sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), m_windowConfig.title, sf::Style::Close);
	}

	m_window.setFramerateLimit(m_windowConfig.FL);
}

std::string GameEngine::getCurrentSceneName()
{
	return m_currentScene;
}
