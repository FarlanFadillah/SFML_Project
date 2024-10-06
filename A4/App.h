#pragma once
#include "GameEngine.hpp"
#include "Action.hpp"
#include <SFML/Graphics.hpp>
#include "EntityManager.hpp"
#include <fstream>

#include <map>
#include <memory>
#include <Scene_Computer.h>

class SceneComputer; // Resolve circular dependency

using ActionMap = std::map<int, std::string>; // input key, action name

class App
{
public:
	bool isInside(Vec2 pos, std::shared_ptr<Entity> e);

	void buttonPressed(Vec2 pos, const std::string& name, EntityManager& m_entityManager);
	void buttonHover(Vec2& pos, const std::string& name, EntityManager& m_entityManager);
	bool m_mouseOnClick = false;
	SceneComputer* m_scene;

	uint64_t      m_currentFrame = 0;
	bool          m_hasEnded = false;
	bool          m_paused = false;
	ActionMap     m_actionMap;

	virtual void update() = 0;
	virtual void onEnd() = 0;
	virtual void sRender() = 0;
	virtual void sDoAction(const Action& action) = 0;



	void registerAction(int inputKey, const std::string& actionName);

	std::string formatNumber(int number);



	App();
	App(SceneComputer* sceneComputer);
};

