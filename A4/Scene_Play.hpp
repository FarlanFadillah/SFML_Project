#pragma once
#include "Action.hpp"
#include "Entity.hpp"
#include "Scene.hpp"

struct GUI {
	Vec2 pos = { 0,0 };
	sf::Sprite img;
};

struct PlayerConfig
{
	float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
	std::string WEAPON;
};

/*
░█████╗░██████╗░██╗███╗░░░███╗███████╗  ░██████╗░█████╗░███████╗███╗░░██╗███████╗
██╔══██╗██╔══██╗██║████╗░████║██╔════╝  ██╔════╝██╔══██╗██╔════╝████╗░██║██╔════╝
██║░░╚═╝██████╔╝██║██╔████╔██║█████╗░░  ╚█████╗░██║░░╚═╝█████╗░░██╔██╗██║█████╗░░
██║░░██╗██╔══██╗██║██║╚██╔╝██║██╔══╝░░  ░╚═══██╗██║░░██╗██╔══╝░░██║╚████║██╔══╝░░
╚█████╔╝██║░░██║██║██║░╚═╝░██║███████╗  ██████╔╝╚█████╔╝███████╗██║░╚███║███████╗
░╚════╝░╚═╝░░╚═╝╚═╝╚═╝░░░░░╚═╝╚══════╝  ╚═════╝░░╚════╝░╚══════╝╚═╝░░╚══╝╚══════╝
*/

class CrimeScene : public Scene
{
private:
	std::shared_ptr<Entity> m_player;
	EntityManager           m_entityManager;
	std::string             m_levelPath;
	bool                    m_drawTextures = true;
	bool                    m_drawCollision = false;
	bool                    m_drawGrid = false;
	bool					m_drawStats = true;
	bool					m_dragging = false;
	bool					m_debugMode = false;
	sf::IntRect				m_rect; 
	Vec2					m_worldSize;
	Vec2					m_pos;
	Vec2					m_lastDragPos = { 0,0 };
	Vec2					m_gridSize = { 64,64 };
	sf::CircleShape			m_circleMouse;
	sf::View				CameraView;
	std::vector<std::string>m_stats;
	sf::Text				m_text;

	Physics					m_physics;

	//Dialogue Gui
	std::map<std::string, GUI> m_guiObjectMap;

	void init(std::string path);
	void update()                        override;
	void onEnd()                         override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;

	void sCamera();


	void loadLevel(const std::string& levelPath);
	void sMovement();
	void sAnimation();
	void sCollision();
	void setAnimation(std::shared_ptr<Entity> entity,
		const std::string& animationName, bool repeat); // helper function
	void drawLine(const Vec2& p1, const Vec2& p2);
	void spawnPlayer();
	void spawnObjects(std::string& className, std::string& animationName, Vec2& pos, Vec2 boundingBox, Vec2 off);

	void loadMap(std::istream& fin, std::string word, std::string prefix1, std::string prefix2);
	void loadObject(std::istream& fin, std::string start, std::string end);
	void sDragAndDrop();

	bool checkCollision(Vec2 ca, Vec2 cb, float ra, float rb);
	Vec2 windowToWorldPos(Vec2 pos);

	void screenShot();
	void checkCollisionMouse(Vec2 pos, const std::string& name, Action action);
	void dragAndDrop(Vec2 pos);


public:
	CrimeScene(GameEngine* gameEngine, const std::string& path);
};



/*
░█████╗░███████╗███████╗██╗░█████╗░███████╗  ░██████╗░█████╗░███████╗███╗░░██╗███████╗
██╔══██╗██╔════╝██╔════╝██║██╔══██╗██╔════╝  ██╔════╝██╔══██╗██╔════╝████╗░██║██╔════╝
██║░░██║█████╗░░█████╗░░██║██║░░╚═╝█████╗░░  ╚█████╗░██║░░╚═╝█████╗░░██╔██╗██║█████╗░░
██║░░██║██╔══╝░░██╔══╝░░██║██║░░██╗██╔══╝░░  ░╚═══██╗██║░░██╗██╔══╝░░██║╚████║██╔══╝░░
╚█████╔╝██║░░░░░██║░░░░░██║╚█████╔╝███████╗  ██████╔╝╚█████╔╝███████╗██║░╚███║███████╗
░╚════╝░╚═╝░░░░░╚═╝░░░░░╚═╝░╚════╝░╚══════╝  ╚═════╝░░╚════╝░╚══════╝╚═╝░░╚══╝╚══════╝
*/



class OfficeScene : public Scene
{
private:
	std::shared_ptr<Entity> m_player;
	PlayerConfig            m_playerConfig;
	EntityManager           m_entityManager;
	std::string             m_levelPath;
	bool                    m_drawTextures  = true;
	bool                    m_drawCollision = false;
	bool                    m_drawGrid      = false;
	bool					m_record			= false;
	bool					m_playRecord		= false;
	bool					smoothCam		= true;
	bool					m_cameraLimit	= true;
	bool					m_drawStats		= false;
	bool					m_dragging		= false;
	bool					m_debugMode		= false;
	const Vec2              m_gridSize      = { 64, 64 };
	sf::IntRect				m_rect;
	Vec2					m_worldSize;
	Vec2					m_pos;
	Vec2					m_lastDragPos = { 0,0 };
	sf::CircleShape			m_circleMouse;
	sf::View				CameraView;
	float					m_zoomScale = 1;

	Physics					m_physics;

	sf::Text                m_gridText;
	sf::Text				m_text;

	//int currentOfficeSceneFrame = 0;

	std::vector<std::string>m_stats;

	//Dialogue Gui
	std::map<std::string, GUI> m_guiObjectMap; 

	//Record
	int r_frame;
	std::string r_name, r_type;

	std::string actionTextRecord;

	void update()                        override;
	void onEnd()                         override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;
	void sCamera();

	void init(const std::string& levelPath);
	void loadLevel(const std::string& levelPath);
	void sMovement();
	void sAnimation();
	void sCollision();
	void setAnimation(std::shared_ptr<Entity> entity,
					  const std::string& animationName, bool repeat); // helper function
	void drawLine(const Vec2& p1, const Vec2& p2);
	void spawnPlayer();
	void spawnObjects(std::string& className, std::string& animationName, Vec2& pos, Vec2 boundingBox, Vec2 off);

	void playRecord(int& frame);
	void record(const Action& action);
	void loadMap(std::istream& fin, std::string word, std::string prefix1, std::string prefix2);
	void loadObject(std::istream& fin, std::string start, std::string end); 
	void sDragAndDrop();

	bool checkCollision(Vec2 ca, Vec2 cb, float ra, float rb);
	Vec2 windowToWorldPos(Vec2 pos);

	void screenShot();
	void checkCollisionMouse(Vec2 pos, const std::string& name, Action action);
	void dragAndDrop(Vec2 pos);


public:
	int index = 1;
	OfficeScene(GameEngine* gameEngine, const std::string& levelPath);
};