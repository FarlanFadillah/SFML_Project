#include "Note_APP.h"


NoteAPP::NoteAPP()
{
}

NoteAPP::NoteAPP(SceneComputer* sceneComputer, std::string text)
	:App(sceneComputer)
{
	init(text); 
}
void NoteAPP::init(std::string path)
{
	setUpApp(path); 
}

void NoteAPP::onEnd()
{
	m_scene->closeApp();
}

void NoteAPP::update()
{
	m_entityManager.update();
	m_currentFrame++;
}

void NoteAPP::sRender()
{
	auto& window = m_scene->gameEngine()->window();

	//draw base app
	window.draw(base_app);

	window.draw(text_box); 


	for (auto& e : m_entityManager.getEntities("Button")) 
	{
		if (e->hasComponent<CButton>())
		{
			auto& button = e->getComponent<CButton>();
			auto& box = e->getComponent<CBoundingBox>();
			auto& transform = e->getComponent<CTransform>();
			auto& anim = e->getComponent<CAnimation>();

			int index = 0;
			if (button.mouseOver && !button.clicked)
			{
				index = 1;
			}
			index = button.clicked ? 2 : index;

			auto sprite = anim.animation.setSprite(index);

			sprite.setPosition(transform.pos.x, transform.pos.y);
			sprite.setScale(transform.scale.x, transform.scale.y);

			window.draw(sprite);
		}
	}


	for (auto& e : m_entityManager.getEntities("Input"))
	{
		auto& cInput = e->getComponent<CTextInput>(); 
		auto& box = e->getComponent<CBoundingBox>(); 
		auto& transform = e->getComponent<CTransform>(); 
		auto& anim = e->getComponent<CAnimation>(); 

		auto sprite = anim.animation.getSprite();

		sprite.setPosition(transform.pos.x, transform.pos.y);
		sprite.setScale(transform.scale.x, transform.scale.y);

		window.draw(sprite);

		auto& text = cInput.m_text;
		//m_scene->m_stats.push_back(std::to_string(cInput.text.size()));
		int row = 0;
		for (auto txt : cInput.text)
		{
			if ((m_currentFrame / 20) % 2 == 0 && cInput.onFokus && row >= cInput.text.size() - 1)
			{
				text.setString(txt + "_");
			}
			else
			{
				text.setString(txt);
			}
			/*m_scene->m_stats.push_back(std::to_string(txt.length()));
			m_scene->m_stats.push_back(std::to_string(text.getGlobalBounds().width));*/
			text.setFillColor(sf::Color::Black);
			text.setStyle(sf::Text::Style::Bold);
			text.setPosition(sprite.getGlobalBounds().left + 16, sprite.getGlobalBounds().top + 16 + (row * (16)));
			window.draw(text);
			row++;
		}
	}
	m_scene->m_stats.push_back(m_shiftIsPressed ? "Shift Pressed on" : "Shift Pressed off");
	
}

void NoteAPP::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "EXIT")
		{
			onEnd();
		}
		else if (action.name() == "LEFT_CLICK")
		{
			m_mouseOnClick = true;
			buttonPressed(action.pos(), "Button", m_entityManager);
		}
		else if (action.name() == "MOUSE_MOVE")
		{
			buttonHover(m_scene->m_mousePos, "Button", m_entityManager);
		}
		else if (action.name() == "TYPE")
		{
			m_scene->typeText(action.key(), m_entityManager, m_shiftIsPressed); 
		}
		else if (action.name() == "LSHIFT")
		{
			m_shiftIsPressed = true;
		}
		else if (action.name() == "CAPS_LOCK")
		{
			m_shiftIsPressed = !m_shiftIsPressed;
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT_CLICK")
		{
			m_mouseOnClick = false;
			buttonPressed(action.pos(), "Button", m_entityManager); 
			m_scene->inputPressed(action.pos(), m_entityManager);
		}
		else if (action.name() == "TYPE")
		{
			//m_scene->typeText(action.key(), m_entityManager);
		}
		else if (action.name() == "LSHIFT")
		{
			m_shiftIsPressed = false;
		}
	}
}

void NoteAPP::setUpApp(const std::string path)
{
	auto& window = m_scene->gameEngine()->window(); 
	base_app.setTexture(m_scene->gameEngine()->assets().getTexture("base_app")); 
	base_app.setOrigin(base_app.getGlobalBounds().width / 2, base_app.getGlobalBounds().height / 2); 
	base_app.setPosition(window.getSize().x / 2, window.getSize().y/2);

	std::ifstream in(path);

	std::string token;
	while (in.good())
	{
		in >> token;
		if (token.find("//") != std::string::npos) continue;
		if (token == "Button")
		{
			std::string name, action;
			int x, y, scale;
			in >> name >> action >> x >> y >> scale;
			auto button = m_entityManager.addEntity("Button");

			button->addComponent<CAnimation>(
				m_scene->gameEngine()->assets().getAnimation(name),
				true
			);


			auto& bsize = button->getComponent<CAnimation>().animation.getSize();

			button->addComponent<CTransform>(Vec2(base_app.getGlobalBounds().left + x + bsize.x/2, base_app.getGlobalBounds().top + y + bsize.y/2), // POSITION 
				Vec2(0.0f, 0.0f), // VELOCITY 
				Vec2(scale, scale), // SCALE  scale set to 4x16, 4x16 
				0); 

			button->addComponent<CBoundingBox>(Vec2(bsize.x * scale, bsize.y * scale));
			button->addComponent<CButton>(Action(action, "START"));
		}
		else if (token == "Input")
		{
			std::string name, anim;
			int x, y, fSize, scale, maxWord, maxCol, maxRow;
			bool canInput, numberOnly;
			in >> name >> anim >> x >> y >> scale >> fSize >> canInput >> numberOnly >> maxWord >> maxCol >> maxRow;


			auto input = m_entityManager.addEntity("Input");

			input->addComponent<CAnimation>(
				m_scene->gameEngine()->assets().getAnimation(anim),
				true
			);

			auto& bsize = input->getComponent<CAnimation>().animation.getSize(); 
			input->addComponent<CTransform>(Vec2(base_app.getGlobalBounds().left + x + bsize.x / 2, base_app.getGlobalBounds().top + y + bsize.y / 2), // POSITION  
				Vec2(0.0f, 0.0f), // VELOCITY 
				Vec2(scale, scale), // SCALE  scale set to 4x16, 4x16 
				0);


			input->addComponent<CBoundingBox>(Vec2(bsize.x, bsize.y));
			input->addComponent<CTextInput>(fSize, name);

			auto& textIn = input->getComponent<CTextInput>();
			textIn.canInput = canInput;
			textIn.numberOnly = numberOnly;
			textIn.maxWord = maxWord; 
			textIn.maxWidth = maxCol;
			textIn.maxRow = maxRow;
			textIn.m_text.setFont(m_scene->gameEngine()->assets().getFont("Times_New_Roman"));
			textIn.m_text.setFillColor(sf::Color::Black);


		}
		//else if (token == "ss")
		//{
		//	std::string firstName, lastName, homeTown, blood; 
		//	int age, height; 

		//	in >> firstName >> lastName >> age >> height >> blood >> homeTown; 
		//	//std::cout << firstName << " " << lastName << " " << age << " yrs old " << homeTown << "\n"; 

		//	auto input = m_entityManager.addEntity("List");
		//	auto& textIn = input->getComponent<CTextInput>(); 

		//	input->addComponent<CTextInput>(18);
		//	textIn.m_text.setFillColor(sf::Color::Black);

		//	textIn.text = firstName + " " + lastName + " " + std::to_string(age) + " yrs old " + homeTown + "\n";
		//}

	}


	


	
}






