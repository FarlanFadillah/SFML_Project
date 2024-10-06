#include "FingerPrintDBS_APP.h"

FingerPrintDBS_APP::FingerPrintDBS_APP(SceneComputer* sceneComputer, std::string text)
	:App(sceneComputer) 
{
	init(text);
}


void FingerPrintDBS_APP::init(std::string path) 
{
	//
	m_baseApp = m_scene->gameEngine()->assets().getTexture("base_app");
	m_baseApp2 = m_scene->gameEngine()->assets().getTexture("base_app_2");
	m_baseApp3 = m_scene->gameEngine()->assets().getTexture("base_app_3");

	setApp(path); 

	loadFingerPrint("res/config/finger_print_dbs.ini");
}

void FingerPrintDBS_APP::onEnd()
{
	sReset();
	m_scene->closeApp();
}

void FingerPrintDBS_APP::update()
{
	sFingerPrint();
	m_entityManager.update();
	m_currentFrame++;
}

void FingerPrintDBS_APP::sRender()
{
	auto m_game = m_scene->gameEngine();
	auto& window = m_game->window(); 
	//Draw main app
	window.draw(app);

	//draw left side app
	window.draw(appLeft);

	//draw right side app
	window.draw(appRight);



	//draw identification card

	if (!currentData.isEmpty)
	{
		//draw id card
		window.draw(id_card);

		sf::Sprite sprite;

		sprite.setTexture(currentData.img);
		sprite.setScale(2, 2);
		sprite.setPosition(id_card.getGlobalBounds().left + 22, id_card.getGlobalBounds().top + 70);

		//draw full name into the id card
		sf::Text name;
		name.setCharacterSize(15);
		name.setFillColor(sf::Color::Black);
		name.setStyle(sf::Text::Style::Bold);
		name.setString("Birth Name\t : " + currentData.firstName + " " + currentData.lastName);
		name.setFont(m_game->assets().getFont("Ostrich_Regular"));
		name.setPosition(id_card.getGlobalBounds().left + 84, id_card.getGlobalBounds().top + 74);


		sf::Text age;
		age.setCharacterSize(15);
		age.setFillColor(sf::Color::Black);
		age.setStyle(sf::Text::Style::Bold);
		age.setString("Age\t\t\t\t: " + std::to_string(currentData.age));
		age.setFont(m_game->assets().getFont("Ostrich_Regular"));
		age.setPosition(id_card.getGlobalBounds().left + 84, id_card.getGlobalBounds().top + 100);


		sf::Text city;
		city.setCharacterSize(20);
		city.setFillColor(sf::Color::Black);
		city.setStyle(sf::Text::Style::Bold);
		city.setString(currentData.city);
		city.setFont(m_game->assets().getFont("Ostrich_Regular"));
		city.setPosition(id_card.getGlobalBounds().left + 168, id_card.getGlobalBounds().top + 40);

		window.draw(name);
		window.draw(age);
		window.draw(city);
		window.draw(sprite);
	}


	// draw finger print data base

	if (m_searchingDBS)
	{
		auto& fp = m_fingerPrints[fpCount];
		auto& img = fp.img;
		sf::Sprite fpSprite;
		fpSprite.setTexture(img);
		fpSprite.setOrigin(img.getSize().x / 2, img.getSize().y / 2);
		fpSprite.setScale(8, 8);
		fpSprite.setPosition(app.getPosition().x + fpSprite.getGlobalBounds().width / 2 + 5, app.getPosition().y);

		window.draw(fpSprite);
	}
	else if (!m_searchingDBS && m_findMatch)
	{
		auto& fp = m_fingerPrints[fpCount];
		auto& img = fp.img;
		sf::Sprite fpSprite;
		fpSprite.setTexture(img);
		fpSprite.setOrigin(img.getSize().x / 2, img.getSize().y / 2);
		fpSprite.setScale(8, 8);
		fpSprite.setPosition(app.getPosition().x + fpSprite.getGlobalBounds().width / 2 + 5, app.getPosition().y);

		window.draw(fpSprite);
	}



	//drawing new fingerprints object 

	int col = 0, row = 0;
	sf::RectangleShape rect;
	for (auto& e : m_entityManager.getEntities("FingerPrints"))
	{
		//draw fingerprint img
		auto& fp = e->getComponent<CFingerPrint>();
		auto& img = fp.img;

		sf::Sprite fpSprite;
		fpSprite.setTexture(img);
		fpSprite.setOrigin(fpSprite.getGlobalBounds().width / 2, fpSprite.getGlobalBounds().height / 2);
		fpSprite.setScale(2, 2);

		Vec2 pos(
			((appLeft.getPosition().x - appLeft.getGlobalBounds().width / 2) + 32) + fpSprite.getGlobalBounds().width / 2 + (col * (fpSprite.getGlobalBounds().width + 5)),
			((appLeft.getPosition().y - appLeft.getGlobalBounds().height / 2) + 80) + fpSprite.getGlobalBounds().height / 2 + (row * (fpSprite.getGlobalBounds().height + 5))
		);
		if (!e->hasComponent<CTransform>())
		{
			e->addComponent<CTransform>(pos);
		}
		fpSprite.setPosition(pos.x, pos.y);

		window.draw(fpSprite);
		//draw rect if selected

		if (fp.selected)
		{
			rect.setSize(sf::Vector2f(fpSprite.getGlobalBounds().width + 2, fpSprite.getGlobalBounds().height + 2));
			rect.setOrigin(fpSprite.getGlobalBounds().width / 2, fpSprite.getGlobalBounds().height / 2);
			rect.setPosition(pos.x - 1, pos.y - 1);
			rect.setFillColor(sf::Color(0, 0, 0, 0));
			rect.setOutlineColor(sf::Color::Blue);
			rect.setOutlineThickness(2.0f);

			window.draw(rect);
		}

		col++;
		if (col >= 3)
		{
			col = 0;
			row++;
		}
	}


	if (m_selectredFP.id != 0)
	{

		sf::Sprite fpSprite;
		fpSprite.setTexture(m_selectredFP.img);
		fpSprite.setOrigin(m_selectredFP.img.getSize().x / 2, m_selectredFP.img.getSize().y / 2);
		fpSprite.setScale(8, 8);
		fpSprite.setPosition(app.getPosition().x - fpSprite.getGlobalBounds().width / 2 - 5, app.getPosition().y);

		window.draw(fpSprite);


	}


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

	if ((m_currentFrame / 15) % 4 >= 2)
	{
		sf::Text text;

		m_searchingDBS ? text.setString("Searching...") : text.setString(m_findMatch ? "Found Match" : "Not Found");
		text.setStyle(sf::Text::Style::Bold);
		text.setFont(m_game->assets().getFont("Ostrich_Regular"));

		m_findMatch ? text.setFillColor(sf::Color::Blue) : text.setFillColor(sf::Color::Red);

		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);

		text.setPosition(window.getSize().x / 2, app.getPosition().y + app.getGlobalBounds().height / 2 - text.getGlobalBounds().height * 3);

		window.draw(text);
	}
}

void FingerPrintDBS_APP::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "LEFT_CLICK")
		{
			m_mouseOnClick = true;
			buttonPressed(action.pos(), "Button", m_entityManager);
		}
		else if (action.name() == "EXIT")
		{
			onEnd();
		}
		else if (action.name() == "MOUSE_MOVE")
		{
			buttonHover(m_scene->m_mousePos, "Button", m_entityManager);
		}
		else if (action.name() == "TOOGLE_COLLISION")
		{
			m_drawCollision = !m_drawCollision;
		}
		else if (action.name() == "SEARCHING")
		{
			if(m_selectredFP.id != -1) m_searchingDBS = true;
		}
		else if (action.name() == "RESET_DBS")
		{
			sReset();
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT_CLICK")
		{
			m_mouseOnClick = false;
			//std::cout << action.pos().x << " : " << action.pos().y << "\n";
			buttonPressed(action.pos(), "Button", m_entityManager);
			fingerPrintSelect(action.pos());
		}
	}
}

void FingerPrintDBS_APP::sFingerPrint()
{
	if (!m_searchingDBS) return;
	fpCount = (m_searchingCount / 15) % m_fingerPrints.size();

	if (m_selectredFP.id == m_fingerPrints[fpCount].id)
	{
		m_searchingDBS = false;
		m_findMatch = true;

		getData(m_selectredFP.owner_id);
	}
	else if (m_selectredFP.id != m_fingerPrints[fpCount].id && fpCount == m_fingerPrints.size() - 1)
	{
		m_searchingDBS = false;
		m_findMatch = false;
	}
	m_searchingCount++;
}

void FingerPrintDBS_APP::setApp(const std::string& path)
{
	std::ifstream indbs("res/config/criminal_record_text.ini");

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

			button->addComponent<CTransform>(Vec2(x, y), // POSITION 
				Vec2(0.0f, 0.0f), // VELOCITY
				Vec2(scale, scale), // SCALE  scale set to 4x16, 4x16
				0);

			auto& bsize = button->getComponent<CAnimation>().animation.getSize();

			button->addComponent<CBoundingBox>(Vec2(bsize.x * scale, bsize.y * scale));
			button->addComponent<CButton>(Action(action, "START"));
		}
		else if (token == "Input")
		{
			std::string name, anim;
			int x, y, fSize, maxWords;
			bool canInput, numberOnly;
			in >> name >> anim >> x >> y >> fSize >> canInput >> numberOnly >> maxWords;


			auto input = m_entityManager.addEntity("Input");

			input->addComponent<CAnimation>(
				m_scene->gameEngine()->assets().getAnimation(anim),
				true
			);

			input->addComponent<CTransform>(Vec2(x, y), // POSITION 
				Vec2(0.0f, 0.0f), // VELOCITY
				Vec2(1.0f, 1.0f), // SCALE  scale set to 4x16, 4x16
				0);

			auto& bsize = input->getComponent<CAnimation>().animation.getSize();

			input->addComponent<CBoundingBox>(Vec2(bsize.x, bsize.y));
			input->addComponent<CTextInput>(fSize, name);


			auto& textIn = input->getComponent<CTextInput>();
			textIn.canInput = canInput;
			textIn.numberOnly = numberOnly;
			textIn.m_text.setFillColor(sf::Color::Black);
			textIn.maxWord = maxWords;


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

	auto& window = m_scene->gameEngine()->window();

	//Draw main app
	app.setTexture(m_baseApp);
	app.setOrigin(m_baseApp.getSize().x / 2, m_baseApp.getSize().y / 2);
	app.setPosition(window.getSize().x / 2, window.getSize().y / 2); 

	//draw left side app
	appLeft.setTexture(m_baseApp2);
	appLeft.setOrigin(m_baseApp2.getSize().x / 2, m_baseApp2.getSize().y / 2);
	appLeft.setPosition(app.getPosition().x - app.getGlobalBounds().width / 2 - m_baseApp2.getSize().x / 2, window.getSize().y / 2);


	//draw right side app
	appRight.setTexture(m_baseApp3);
	appRight.setOrigin(m_baseApp3.getSize().x / 2, m_baseApp3.getSize().y / 2);
	appRight.setPosition(app.getPosition().x + app.getGlobalBounds().width / 2 + m_baseApp3.getSize().x / 2, window.getSize().y / 2);


	//id card
	auto& id_card_tex = m_scene->gameEngine()->assets().getTexture("id_card_2"); 
	id_card.setTexture(id_card_tex);
	id_card.setOrigin(id_card_tex.getSize().x / 2, id_card_tex.getSize().y / 2);
	id_card.setScale(sf::Vector2f(2.0f, 2.0f));
	id_card.setPosition(appRight.getPosition().x, appRight.getPosition().y);

}

void FingerPrintDBS_APP::loadFingerPrint(std::string path)
{
	std::ifstream in(path);

	int index = 0;
	while (in.good())
	{
		int id, owner_id;
		std::string textureName;
		in >> id >> textureName >> owner_id;
		FingerPrint temp;
		temp.id = id;
		temp.owner_id = owner_id;
		temp.img = m_scene->gameEngine()->assets().getTexture(textureName); 
		m_fingerPrints.push_back(temp);


		// temp new fingerprint
		// change this when gameplay is done
		auto fp = m_entityManager.addEntity("FingerPrints");
		fp->addComponent<CFingerPrint>(id, owner_id, temp.img);
		fp->addComponent<CBoundingBox>(Vec2(58, 58));


		index++;

	}

}

void FingerPrintDBS_APP::fingerPrintSelect(const Vec2 pos)
{
	for (auto& e : m_entityManager.getEntities("FingerPrints"))
	{
		auto& fp = e->getComponent<CFingerPrint>();
		if (appLeft.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
		{
			if (isInside(pos, e))
			{
				sReset();
				m_selectredFP.id = fp.id;
				m_selectredFP.owner_id = fp.owner_id;
				m_selectredFP.img = fp.img;
				fp.selected = true;
			}
			else
			{
				fp.selected = false;
			}
		}
	}
}

void FingerPrintDBS_APP::sReset()
{
	m_searchingDBS = false;
	m_findMatch = false;
	m_selectredFP = FingerPrint();
	currentData = Data();
	fpCount = 0;
	m_searchingCount = 0;

}

void FingerPrintDBS_APP::getData(int id)
{
	std::ifstream in("res/config/citizen_data.ini");

	while (in.good())
	{
		std::string token;
		int m_id;
		in >> token;
		std::string img_name;

		if (token == "Data")
		{

			in >> m_id;
			if ((int)m_id == id)
			{
				in >> currentData.firstName >> currentData.lastName >> currentData.age >> currentData.height >> currentData.bloodType >> currentData.city >> img_name;
				currentData.img = m_scene->gameEngine()->assets().getTexture(img_name);
				currentData.isEmpty = false;
			}
		}
	}
}

