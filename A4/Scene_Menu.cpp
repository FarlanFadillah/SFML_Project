#include "Action.hpp"
#include "Scene_Menu.hpp"
#include <Scene_Play.hpp>
#include <Scene_Interview.h>

SceneMenu::SceneMenu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void SceneMenu::init()
{
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "ECS GAME ENGINE";

	m_menuText.setFont(m_game->assets().getFont("Ostrich_Regular"));
	m_menuText.setCharacterSize(64);
	m_buttonName.push_back("NEW GAME"); 
	m_buttonName.push_back("DEXTER APART"); 
	m_buttonName.push_back("INTERVIEW SCENE");
	m_buttonName.push_back("LOAD"); 
	m_buttonName.push_back("OPTIONS");
	m_buttonName.push_back("QUIT");

	auto& window = m_game->window();

	setButton(m_buttonName);
}

void SceneMenu::setButton(std::vector<std::string>& name)
{
	auto& window = m_game->window();
	for (int i = 0; i < name.size(); i++)
	{
		//set buttom IMGUI
		//m_buttons.push_back(name[i].c_str());

		//set button
		auto entity = m_entityManager.addEntity("Button");


		entity->addComponent<CButton>(Action(name[i], "START"), name[i], m_game->assets().getFont("Ostrich_Regular"), 50); 

		auto& bt = entity->getComponent<CButton>();
		auto size = bt.text.getGlobalBounds();

		entity->addComponent<CBoundingBox>(Vec2(size.width, size.height));
		
		entity->addComponent<CTransform>(Vec2(window.getSize().x / 2, (window.getSize().y / 2) - (size.height * name.size())/2 + (i * (size.height + 16))), // POSITION 
			Vec2(0.0f, 0.0f), // VELOCITY
			Vec2(1.0f, 1.0f), // SCALE  scale set to 4x16, 4x16
			0);


	}
}

void SceneMenu::renderButton(std::vector<const char*>& name)
{
	for (const auto& text : name)
	{
		if (ImGui::Button(text, sf::Vector2f(100,50)))
		{
			std::string action = text;
			sDoAction(Action(action, "START"));
		}
	}
}

void SceneMenu::update()
{
	m_entityManager.update();
	sRender();
}

void SceneMenu::onEnd()
{
	m_game->quit();
}

void SceneMenu::sDoAction(const Action& action)
{
	
	if (action.type() == "START")
	{
		
		if (action.name() == "LEFT_CLICK") 
		{
			m_mouseOnClick = true;
			buttonPressedBB(action.pos(),"Button", m_entityManager);
		}
		else if (action.name() == "NEW GAME")
		{
			m_game->changeScene("OFFICE", std::make_shared<OfficeScene>(m_game, "res/images/tile/Miami_Metro_Office.tmx"));
		}
		else if (action.name() == "DEXTER APART")
		{
			m_game->changeScene("CRIME_SCENE", std::make_shared<CrimeScene>(m_game, "res/map/crime_scene/01/Dexter_Apart.tmx"));
		}
		else if (action.name() == "LOAD")
		{
			std::cout << "Load Game\n";
		}
		else if (action.name() == "INTERVIEW SCENE")
		{
			m_game->changeScene("INTERVIEW_SCENE", std::make_shared<SceneInterview>(m_game, "res/scene/interview/GUI.ini", "res/config/dialogue/Haley/Haley_Convo.json"));
		}
		/*
		else if (action.name() == "MAP_EDITOR")
		{
			m_game->changeScene(
				"PLAY",
				std::make_shared<SceneMapEditor>(
					m_game,
					m_levelPaths[2]
				)
			);
		}*/  
		else if (action.name() == "OPTIONS")
		{
			std::cout << "Options clicked\n";
		}
		else if (action.name() == "QUIT") 
		{
			onEnd();
		}
		else if (action.name() == "MOUSE_MOVE")
		{
			mPos = action.pos();
			buttonHoverBB(mPos, "Button", m_entityManager);
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT_CLICK")
		{
			m_mouseOnClick = false;
			buttonPressedBB(action.pos(), "Button", m_entityManager);
		}
	}
}

void SceneMenu::sDrawImGui()
{
}

// System: Render
void SceneMenu::sRender()
{
	auto& window = m_game->window();

	window.clear(sf::Color(45, 50, 80)); // Blue color background

	// Render title text
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color(246, 177, 122));
	// Center the title text
	const float posX = window.getSize().x / 2.0f -
	                       m_menuText.getLocalBounds().width / 2.0f;
	const float posY = 10;
	m_menuText.setPosition(posX, posY);
	window.draw(m_menuText);


	//// Render menu text
	//for (int stringIndex = 0; stringIndex < m_menuStrings.size(); stringIndex++)
	//{
	//	const float posX = 20;
	//	const float posY = 150 + stringIndex * 100;
	//	m_menuText.setString(m_menuStrings[stringIndex]);
	//	m_menuText.setPosition(posX, posY);


	//	if (m_selectedMenuIndex == stringIndex)
	//	{
	//		auto bound = m_menuText.getGlobalBounds();
	//		m_menuText.setFillColor(sf::Color(112, 119, 161)); // Black colour current selected option in the menu
	//		sf::CircleShape circle(10, 3);
	//		circle.setOrigin(5, 5);
	//		circle.setPosition(bound.left + bound.width + 64, bound.top + bound.height/2);
	//		circle.rotate(-90);
	//		window.draw(circle);
	//	}
	//	else
	//	{
	//		m_menuText.setFillColor(sf::Color(246, 177, 122)); // White colour not selected text
	//	}


	//	

	//	window.draw(m_menuText);
	//}

	for (auto& e : m_entityManager.getEntities("Button"))
		{
			auto& button = e->getComponent<CButton>();
			int index = 0;
			if (button.mouseOver && !button.clicked)
			{
				index = 1;
			}
			index = button.clicked ? 2 : index;
			
			auto& bcTransform = e->getComponent<CTransform>();  
			auto& sprite = e->getComponent<CAnimation>().animation.setSprite(index);
			auto& size = e->getComponent<CAnimation>().animation.getSize();
			auto& text = button.text;

			sprite.setPosition(bcTransform.pos.x, bcTransform.pos.y); 

			if (!text.getString().isEmpty())
			{
				

				if (index == 0)
				{
					//nothing click nor hover
					text.setFillColor(sf::Color::White);
				}
				else if (index == 1)
				{
					//if mouse over the button
					text.setFillColor(sf::Color(246, 177, 122));

				}
				else if (index == 2)
				{
					//button get click
					text.setFillColor(sf::Color::Magenta);
				}
				sf::FloatRect textBounds = text.getLocalBounds();
				// Mengatur origin ke tengah dari teks
				text.setOrigin(textBounds.left + textBounds.width / 2.0f,
					textBounds.top + textBounds.height / 2.0f);
				text.setPosition(bcTransform.pos.x, bcTransform.pos.y);
				window.draw(text);
			}
		}
	window.display();
}