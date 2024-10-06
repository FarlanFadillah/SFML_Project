#pragma once
#include "App.h"


struct Data {
	std::string firstName, lastName, city, bloodType;
	int age, height;
	sf::Texture img;
	bool isEmpty = true;
};

struct FingerPrint {
	int id = -1, owner_id; sf::Texture img;
};

class FingerPrintDBS_APP :
    public App
{

public:

	EntityManager		m_entityManager;

	//Data
	Data currentData;

	//Texture App
	sf::Texture			m_baseApp;
	sf::Texture			m_baseApp2;
	sf::Texture			m_baseApp3;
	sf::Sprite app;
	sf::Sprite appLeft;
	sf::Sprite appRight;
	sf::Sprite id_card;

	//Fingerprint app related
	std::vector<FingerPrint> m_fingerPrints;
	FingerPrint				m_selectredFP;

	//count var
	int fpCount = 0;
	uint64_t			m_searchingCount = 0;



	//booleans
	bool				m_searchingDBS = false;
	bool				m_findMatch = false;
	bool				m_drawCollision = false;



	void init(std::string path);
	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;


	//FingerPrint related function
	void sFingerPrint();
	void setApp(const std::string& path);

	void loadFingerPrint(std::string path);
	void fingerPrintSelect(const Vec2 pos);
	void sReset();


	void getData(int id);

	FingerPrintDBS_APP(SceneComputer* sceneComputer, std::string text);

};

