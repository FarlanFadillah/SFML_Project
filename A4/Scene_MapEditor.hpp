#pragma once
#include "GameEngine.hpp"
#include "Scene.hpp"
#include "EntityManager.hpp"

class SceneMapEditor :
    public Scene
{
private:
	std::shared_ptr<Entity> m_camera;
	sf::IntRect				m_rect;
	Vec2					m_worldSize;
	sf::View				CameraView;
	EntityManager           m_entityManager;
	int currentScenePlayFrame = 0;

	bool                    m_drawTextures = true;
	bool                    m_drawCollision = false;
	bool                    m_drawGrid = true;
	bool					m_showButton = false;
	bool					m_mouseOnClick = false;
	bool					m_drawStats = true;
	bool					m_dragging = false;

	sf::Text				m_text;

	std::vector<std::string>m_stats;

	const Vec2              m_gridSize = { 64, 64 };

	Vec2					mPos;
	Vec2					m_lastDragPos;



	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;

	void sCamera();

	void init(const std::string& levelPath);
	void loadLevel(const std::string& levelPath);
	void saveLevel();
	void sMovement();
	void spawnCamera(Vec2 pos);
	bool buttonPressed(Vec2 pos);
	Vec2 windowToWorldPos(Vec2 pos);
	bool isInside(Vec2 pos, std::shared_ptr<Entity> e);
	void createTileButton();

	void buttonHover(Vec2& pos);

	void spawnTile(std::string tile);

public:
	SceneMapEditor(GameEngine* gameEngine, const std::string& levelPath);
};

