#pragma once
#include "App.h"


class NoteAPP : public App
{
	EntityManager m_entityManager;
	sf::Sprite base_app;
	sf::Sprite text_box;

	bool		m_shiftIsPressed = false;

	void init(std::string path);
	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;

	void setUpApp(const std::string path);

public:
	
	NoteAPP();
	NoteAPP(SceneComputer* sceneComputer, std::string text);
};

