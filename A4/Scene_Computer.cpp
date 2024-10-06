#include "Scene_Computer.h"
#include "SFML/Graphics.hpp"
#include <fstream>
#include "FingerPrintDBS_APP.h"
#include <Note_APP.h>
#include "Windows.h"

SceneComputer::SceneComputer(GameEngine* gameEngine, std::string path)
	:Scene(gameEngine)
{ 
	init(path);
}
void SceneComputer::closeApp()
{
	m_appOpen = false;
}
void SceneComputer::init(std::string path)
{
	m_actionMap.clear();
	registerAction(sf::Keyboard::Q, "TYPE");
	registerAction(sf::Keyboard::W, "TYPE");
	registerAction(sf::Keyboard::E, "TYPE");
	registerAction(sf::Keyboard::R, "TYPE");
	registerAction(sf::Keyboard::T, "TYPE");
	registerAction(sf::Keyboard::Y, "TYPE");
	registerAction(sf::Keyboard::U, "TYPE");
	registerAction(sf::Keyboard::I, "TYPE");
	registerAction(sf::Keyboard::O, "TYPE"); 
	registerAction(sf::Keyboard::P, "TYPE");
	registerAction(sf::Keyboard::A, "TYPE");
	registerAction(sf::Keyboard::S, "TYPE");
	registerAction(sf::Keyboard::D, "TYPE");
	registerAction(sf::Keyboard::F, "TYPE");
	registerAction(sf::Keyboard::G, "TYPE");
	registerAction(sf::Keyboard::H, "TYPE");
	registerAction(sf::Keyboard::J, "TYPE");
	registerAction(sf::Keyboard::K, "TYPE"); 
	registerAction(sf::Keyboard::L, "TYPE");
	registerAction(sf::Keyboard::Z, "TYPE");
	registerAction(sf::Keyboard::X, "TYPE");
	registerAction(sf::Keyboard::C, "TYPE");
	registerAction(sf::Keyboard::V, "TYPE");
	registerAction(sf::Keyboard::B, "TYPE");
	registerAction(sf::Keyboard::N, "TYPE");
	registerAction(sf::Keyboard::M, "TYPE");
	registerAction(sf::Keyboard::Num0, "TYPE");
	registerAction(sf::Keyboard::Num1, "TYPE");
	registerAction(sf::Keyboard::Num2, "TYPE");
	registerAction(sf::Keyboard::Num3, "TYPE");
	registerAction(sf::Keyboard::Num4, "TYPE");
	registerAction(sf::Keyboard::Num5, "TYPE");
	registerAction(sf::Keyboard::Num6, "TYPE");
	registerAction(sf::Keyboard::Num7, "TYPE");
	registerAction(sf::Keyboard::Num8, "TYPE");
	registerAction(sf::Keyboard::Num9, "TYPE");

	registerAction(sf::Keyboard::LShift, "LSHIFT");
	registerAction(sf::Keyboard::RShift, "LSHIFT");
	registerAction(sf::Keyboard::Comma, "TYPE");
	registerAction(sf::Keyboard::Period, "TYPE");
	registerAction(sf::Keyboard::Quote, "TYPE");
	registerAction(sf::Keyboard::Tab, "TYPE"); 
	registerAction(sf::Keyboard::Dash, "TYPE"); 
	registerAction(sf::Keyboard::Slash, "TYPE"); 
	registerAction(sf::Keyboard::SemiColon, "TYPE"); 


	registerAction(sf::Keyboard::BackSpace, "TYPE");
	registerAction(sf::Keyboard::Enter, "TYPE");
	registerAction(sf::Keyboard::Space, "TYPE");
	registerAction(sf::Keyboard::Escape, "EXIT");
	registerAction(sf::Keyboard::F1, "TOOGLE_COLLISION");
	m_backGround = m_game->assets().getTexture("window_screen");
	//Draw Background
	bg.setTexture(m_backGround); 
	bg.setPosition(0, 0); 


	m_apps["FINGERPRINT_APP"] = std::make_shared<FingerPrintDBS_APP>(this, "res/config/cComp.ini");
	m_apps["NOTE_APP"] = std::make_shared<NoteAPP>(this, "res/config/Note_App.ini");

	setUpComputer("res/config/cComp.ini");
	

	//auto entity = m_entityManager.addEntity("TextInput");

	//entity->addComponent<CTransform>(Vec2(150, 200), // POSITION 
	//	Vec2(0.0f, 0.0f), // VELOCITY
	//	Vec2(1.0f, 1.0f), // SCALE  scale set to 4x16, 4x16
	//	0
	//);

	//entity->addComponent<CTextInput>();
	//entity->addComponent<CBoundingBox>(Vec2(200,400));


	////Create app
	//auto entity = m_entityManager.addEntity("App");

	//entity->addComponent<CAnimation>(
	//	m_game->assets().getAnimation("crime_record_dbs_app"),
	//	true
	//);
	//auto& size = entity->getComponent<CAnimation>().animation.getSize();

	//entity->addComponent<CBoundingBox>(Vec2(size.x, size.y));

	//entity->addComponent<CTransform>(Vec2(100, 100), // POSITION 
	//	Vec2(0.0f, 0.0f), // VELOCITY
	//	Vec2(1.0f, 1.0f), // SCALE  scale set to 4x16, 4x16
	//	0);

	//entity->addComponent<CButton>(Action("OPEN_CRIME_RECORD_DBS", "START"));

	//entity->addComponent<CApp>(m_game->assets().getAnimation("base_app"));



	////create button
	//auto button = m_entityManager.addEntity("Button");

	//button->addComponent<CAnimation>(
	//	m_game->assets().getAnimation("close_window_button"),
	//	true
	//);

	//button->addComponent<CTransform>(Vec2(0, 0), // POSITION 
	//	Vec2(0.0f, 0.0f), // VELOCITY
	//	Vec2(2.0f, 2.0f), // SCALE  scale set to 4x16, 4x16
	//	0);

	//auto& scale = button->getComponent<CTransform>().scale;
	//auto& bsize = button->getComponent<CAnimation>().animation.getSize();

	//button->addComponent<CBoundingBox>(Vec2(bsize.x * scale.x, bsize.y * scale.y));
	//button->addComponent<CButton>(Action("CLOSE_ALL_APP", "START"));

	//m_closeButton = button;
}

void SceneComputer::onEnd()
{
	//m_game->changeScene("PLAY", m_game->m_SceneMap.at("PLAY"));
	auto csName = m_game->getCurrentSceneName();
	auto name = csName.substr(0, csName.find('_'));
	m_game->load( name + "_PLAY", "Loading..."); 

}

void SceneComputer::update()
{
	m_entityManager.update();
	//update current app
	if(m_appOpen) m_apps[m_currentAppName]->update(); 
	sRender();
	m_currentFrame++;
}

void SceneComputer::sRender()
{
	m_stats.clear();
	auto& window = m_game->window();
	window.clear(sf::Color::Blue);


	//Draw Background
	window.draw(bg);


	//draw current APP
	if(m_appOpen) m_apps[m_currentAppName]->sRender();
	

	//draw list database
	/*for (auto& e : m_entityManager.getEntities("List"))
	{
		auto& textInput = e->getComponent<CTextInput>();  
		if (e->hasComponent<CTextInput>()) 
		{
			auto& box = e->getComponent<CBoundingBox>();  
			auto& pos = e->getComponent<CTransform>().pos; 

			auto& text = textInput.m_text;
			text.setPosition(boxPos.x + 10, boxPos.y + 10);
			text.setStyle(sf::Text::Style::Bold);
			text.setFont(m_game->assets().getFont("Ostrich_Regular"));

			window.draw(text);
		}
	}*/

	////draw app
	for (auto& e : m_entityManager.getEntities("App"))
	{
		auto& button = e->getComponent<CButton>();


		auto& bcTransform = e->getComponent<CTransform>();
		auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
		auto& size = e->getComponent<CAnimation>().animation.getSize();

		sprite.setOrigin(size.x / 2, size.y / 2);
		sprite.setScale(bcTransform.scale.x, bcTransform.scale.y);
		sprite.setPosition(bcTransform.pos.x, bcTransform.pos.y);
		window.draw(sprite); 
	}


	//	auto& app = e->getComponent<CApp>();
	//	if (app.isOpen && m_openDbs) 
	//	{
	//		auto& dbs = app.animation.getSprite();
	//		dbs.setPosition(window.getSize().x / 2, window.getSize().y / 2); 

	//		//Close Button setup

	//		auto& button = m_closeButton->getComponent<CButton>(); 
	//		int index = 0;
	//		if (button.mouseOver && !button.clicked) 
	//		{
	//			index = 1; 
	//		} 
	//		index = button.clicked ? 2 : index;

	//		auto& bcTransform = m_closeButton->getComponent<CTransform>();
	//		auto& bsprite = m_closeButton->getComponent<CAnimation>().animation.setSprite(index);
	//		auto& bBox = m_closeButton->getComponent<CBoundingBox>();

	//		m_closeButton->getComponent<CTransform>().pos = Vec2(dbs.getPosition().x + app.animation.getSize().x/2 - bBox.size.x,
	//															dbs.getPosition().y - app.animation.getSize().y/2  + bBox.size.y); 

	//		bsprite.setPosition(bcTransform.pos.x, bcTransform.pos.y);
	//		bsprite.setScale(sf::Vector2f(bcTransform.scale.x, bcTransform.scale.y));

	//		window.draw(dbs); 
	//		window.draw(bsprite);  
	//	}
	//	else {
	//		app.isOpen = false;
	//	}


	//}
	if (m_drawCollision)
	{
		for (auto e : m_entityManager.getEntities())
		{
			if (e->hasComponent<CBoundingBox>())
			{
				auto& box = e->getComponent<CBoundingBox>();
				auto& anim = e->getComponent<CAnimation>().animation;
				auto& transform = e->getComponent<CTransform>();
				auto animSize = Vec2(anim.getSize().x * abs(transform.scale.x), anim.getSize().y * abs(transform.scale.y));
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x, box.size.y));
				rect.setOrigin(box.size.x / 2, box.size.y / 2);
				rect.setPosition(transform.pos.x + box.offRect.x, transform.pos.y + box.offRect.y);  
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color::Red);
				rect.setOutlineThickness(1);
				window.draw(rect);
			}
		}
	}

	//stats
	auto date = m_game->getDateSystem();

	if (m_drawStats)
	{

		//Mouse stat
		m_stats.push_back(m_mouseOnClick ? "Mouse on Click True" : "Mouse on Click False");
		m_stats.push_back(formatNumber(date.getDate().day) + " / " + formatNumber(date.getDate().month) + " / " + formatNumber(date.getDate().year));
		m_stats.push_back(formatNumber(date.getDate().minute) + " : " + formatNumber(date.getDate().hour));

		int row = 0;

		for (auto e : m_stats)
		{
			m_text.setCharacterSize(30);
			m_text.setFont(m_game->assets().getFont("Ostrich_Regular"));
			m_text.setString(e);
			m_text.setFillColor(sf::Color::Red);
			m_text.setOutlineColor(sf::Color::Black);
			m_text.setOutlineThickness(2.0f);
			m_text.setStyle(sf::Text::Style::Bold);

			m_text.setPosition(50, 50 + (row * m_text.getGlobalBounds().height));
			window.draw(m_text);
			row++;
		}
	}
	
	
	
	window.display();
}

void SceneComputer::sDoAction(const Action& action)
{

	if(m_appOpen) m_apps[m_currentAppName]->sDoAction(action);
	if (action.type() == "START")
	{
		if (action.name() == "LEFT_CLICK")
		{
			m_mouseOnClick = true; 
		}
		else if (action.name() == "EXIT")
		{
			onEnd(); 
		}
		else if (action.name() == "MOUSE_MOVE")
		{
			m_mousePos = action.pos(); 
		}
		else if (action.name() == "TOOGLE_COLLISION")
		{
			m_drawCollision = !m_drawCollision;
		}
		else if (action.name() == "FINGERPRINT_APP")
		{
			m_appOpen = true;
			m_currentAppName = action.name();
		}
		else if (action.name() == "NOTE_APP")
		{
			m_appOpen = true;
			m_currentAppName = action.name();
		}
		
	}
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT_CLICK")
		{
			std::cout << action.pos().x << " : " << action.pos().y << "\n";
			m_mouseOnClick = false;
			buttonPressed(action.pos(), "App", m_entityManager); 
		}
	}
}

void SceneComputer::sDrawImGui()
{
}

void SceneComputer::setUpComputer(const std::string path) 
{
	std::ifstream in(path);
	std::string token, name, actionName;
	float x, y, scale;
	while (in.good())
	{
		in >> token;
		if (token == "App")
		{
			in >> name >> actionName >> x >> y >> scale;
			auto app = m_entityManager.addEntity("App");

			app->addComponent<CTransform>(
				Vec2(x,y),
				Vec2(0,0),
				Vec2(scale, scale),
				0
			);
			app->addComponent<CAnimation>(
				m_game->assets().getAnimation(name), 
				true 
			);
			auto& size = app->getComponent<CAnimation>().animation.getSize();
			app->addComponent<CBoundingBox>(Vec2(size.x * scale, size.y * scale)); 

			app->addComponent<CButton>(Action(actionName, "START")); 
		}
	}




}










