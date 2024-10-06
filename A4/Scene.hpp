#pragma once

#include "Action.hpp"
#include "Entity.hpp"
#include "GameEngine.hpp"
#include "Physics.h"
#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <Assets.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "TXT.hpp"
#include <map>
#include <memory>

class GameEngine; // Resolve circular dependency

using ActionMap = std::map<int, std::string>; // input key, action name

enum GameState {
	Play,
	Dialogue,
	Gui,
};
class Scene
{
protected:
	enum SceneId
	{
		Play = 0,
		MapEditor = 1,
	};

	GameEngine*   m_game          = nullptr;
	uint64_t      m_currentFrame  = 0;
	bool          m_hasEnded      = false;
	bool          m_paused        = false;
	ActionMap     m_actionMap;
	GameState currentState = GameState::Play;
public:

	std::map<GameState, std::string> GameStateName =
	{
		{GameState::Play, "Play State"},
		{GameState::Dialogue, "Dialogue State"}
	};
	
	bool		  m_mouseOnClick = false;
	Scene();
	Scene(GameEngine* game);

	virtual void update()                        = 0;
	virtual void onEnd()                         = 0;
	virtual void sRender()                       = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sDrawImGui()					 = 0;

	const ActionMap& getActionMap() const;

	GameEngine* gameEngine();

	void registerAction(int inputKey, const std::string& actionName);

	char keyCodeToChar(sf::Keyboard::Key keyCode, bool shiftPressed);
	char keyCodeToNum(sf::Keyboard::Key keyCode);
	void typeText(sf::Keyboard::Key keyCode, EntityManager& m_entityManager, bool shiftPressed);
	bool isInside(Vec2 pos, std::shared_ptr<Entity> e);
	bool isInsideBB(Vec2 pos, std::shared_ptr<Entity> e);

	void buttonPressed(Vec2 pos, const std::string& name, EntityManager& m_entityManager);
	void buttonPressedBB(Vec2 pos, const std::string& name, EntityManager& m_entityManager);
	void buttonHover(Vec2& pos, const std::string& name, EntityManager& m_entityManager);
	void buttonHoverBB(Vec2& pos, const std::string& name, EntityManager& m_entityManager);
	void inputPressed(Vec2 pos, EntityManager& m_entityManager);

	void guiSetup(std::string path, EntityManager& m_entityManager);
	void proceedDialogues(EntityManager& m_entityManager);

	std::string formatNumber(int number);


	void setPaused(bool paused);
};
