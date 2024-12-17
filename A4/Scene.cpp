#include "GameEngine.hpp"
#include "Scene.hpp"


Scene::Scene()
{
}

Scene::Scene(GameEngine* game)
	: m_game(game)
{
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

GameEngine* Scene::gameEngine()
{
	return m_game;
}

void Scene::setPaused(bool paused)
{
	m_paused = !m_paused;
}




char Scene::keyCodeToChar(sf::Keyboard::Key keyCode, bool shiftPressed)
{
	if (keyCode >= sf::Keyboard::A && keyCode <= sf::Keyboard::Z) {
		// Letters
		char letter = 'A' + (keyCode - sf::Keyboard::A);
		if (!shiftPressed) {
			//std::cout << "TO LOWER\n";
			letter = std::tolower(letter);
		}
		return letter;
	}
	//else if (keyCode >= sf::Keyboard::Num0 && keyCode <= sf::Keyboard::Num9) {
	//	// Numbers
	//	return '0' + (keyCode - sf::Keyboard::Num0);
	//}
	//else if (keyCode >= sf::Keyboard::Numpad0 && keyCode <= sf::Keyboard::Numpad9) {
	//	// Numpad Numbers
	//	return '0' + (keyCode - sf::Keyboard::Numpad0);
	//}
	else {
		// Handle special characters (only some examples)
		switch (keyCode) {
		case sf::Keyboard::Space: return ' ';
		case sf::Keyboard::BackSpace: return '\0';
		case sf::Keyboard::Enter: return '\n';
		case sf::Keyboard::Dash: return shiftPressed ? '_' : '-';
		case sf::Keyboard::Equal: return shiftPressed ? '+' : '=';
		case sf::Keyboard::LBracket: return shiftPressed ? '{' : '[';
		case sf::Keyboard::RBracket: return shiftPressed ? '}' : ']';
		case sf::Keyboard::SemiColon: return shiftPressed ? ':' : ';';
		case sf::Keyboard::Comma: return shiftPressed ? '<' : ',';
		case sf::Keyboard::Period: return shiftPressed ? '>' : '.';
		case sf::Keyboard::Slash: return shiftPressed ? '?' : '/';
		case sf::Keyboard::Tilde: return shiftPressed ? '~' : '`';
		case sf::Keyboard::Quote: return shiftPressed ? '"' : '\'';
		case sf::Keyboard::Tab: return '\t';
		case sf::Keyboard::BackSlash: return shiftPressed ? '|' : '\\';
			// 
				// Add more cases as needed
		default: return '\0'; // Unhandled key
		}
	}
	return '\0';
}

char Scene::keyCodeToNum(sf::Keyboard::Key keyCode)
{
	if (keyCode >= sf::Keyboard::Num0 && keyCode <= sf::Keyboard::Num9) {
		// Numbers
		return '0' + (keyCode - sf::Keyboard::Num0);
	}
	else if (keyCode >= sf::Keyboard::Numpad0 && keyCode <= sf::Keyboard::Numpad9) {
		// Numpad Numbers
		return '0' + (keyCode - sf::Keyboard::Numpad0);
	}
	else {
		// Handle special characters (only some examples)
		switch (keyCode) {
		case sf::Keyboard::Space: return ' ';
		case sf::Keyboard::BackSpace: return '\0';
		case sf::Keyboard::Enter: return '\n';
			/*
			case sf::Keyboard::Dash: return shiftPressed ? '_' : '-';
			case sf::Keyboard::Equal: return shiftPressed ? '+' : '=';
			case sf::Keyboard::LBracket: return shiftPressed ? '{' : '[';
			case sf::Keyboard::RBracket: return shiftPressed ? '}' : ']';
			case sf::Keyboard::SemiColon: return shiftPressed ? ':' : ';';
			case sf::Keyboard::Comma: return shiftPressed ? '<' : ',';
			case sf::Keyboard::Period: return shiftPressed ? '>' : '.';
			case sf::Keyboard::Slash: return shiftPressed ? '?' : '/';
			case sf::Keyboard::Tilde: return shiftPressed ? '~' : '`';
			case sf::Keyboard::Quote: return shiftPressed ? '"' : '\'';*/
			//case sf::Keyboard::BackSlash: return shiftPressed ? '|' : '\\';
			// 
				// Add more cases as needed
		default: return '\0'; // Unhandled key
		}
	}
	return '\0';
}

void Scene::typeText(sf::Keyboard::Key keyCode, EntityManager& m_entityManager, bool shiftPressed)
{
	for (auto& e : m_entityManager.getEntities("Input"))
	{
		auto& textInput = e->getComponent<CTextInput>();
		auto m_text = textInput.m_text;
		if (e->hasComponent<CTextInput>() && textInput.onFokus && textInput.canInput && textInput.text.size() <= textInput.maxRow)
		{
			//the moment user type in, if vector of string is empty, insert new string to the vectors
			if (textInput.text.empty()) textInput.text.push_back("");
			
			char letter;
			letter = textInput.numberOnly ? keyCodeToNum(keyCode) : letter = keyCodeToChar(keyCode, shiftPressed);

			m_text.setString(textInput.text[textInput.text.size() - 1] + letter);

			//checking if the key code is number or character
			if (!std::isdigit(letter) && textInput.numberOnly && keyCode != sf::Keyboard::Key::Backspace) return;


			// if char letter is a letter (ex a, b, c, d, etc)
			if (keyCode != sf::Keyboard::Key::Backspace && keyCode != sf::Keyboard::Key::Enter)
			{
				if ( m_text.getGlobalBounds().width >= textInput.maxWidth && textInput.text.size() < textInput.maxRow)
				{
					textInput.text.push_back("");
					textInput.text[textInput.text.size() - 1] += letter;
				}
				else if (m_text.getGlobalBounds().width < textInput.maxWidth)
				{
					textInput.text[textInput.text.size() - 1] += letter;
				}
				break;
			}
			// if key code is enter, then add new string to the vector, so we are in new line
			else if (keyCode == sf::Keyboard::Key::Enter && textInput.text.size() < textInput.maxRow)
			{
				textInput.text.push_back(""); 
			}
			else if(keyCode == sf::Keyboard::Key::Backspace)
			{
				if (textInput.text[textInput.text.size() - 1].empty() && textInput.text.size() > 1)
				{
					textInput.text.pop_back();
				}
				else if(!textInput.text[textInput.text.size() - 1].empty())
				{
					textInput.text[textInput.text.size() - 1] = textInput.text[textInput.text.size() - 1].substr(0, textInput.text[textInput.text.size() - 1].length() - 1);
				}
			}
			break;
		}
	}
}

bool Scene::isInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto& scale = e->getComponent<CTransform>().scale;
	auto& size = e->getComponent<CAnimation>().animation.getSize();
	auto& ePos = e->getComponent<CTransform>().pos;

	Vec2 rad(size.x * scale.x, size.y * scale.y);

	float distX = fabs(pos.x - ePos.x);
	float distY = fabs(pos.y - ePos.y);

	return (distX < rad.x / 2 && distY < rad.y / 2);

}

bool Scene::isInsideBB(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto& scale = e->getComponent<CTransform>().scale;
	auto& size = e->getComponent<CAnimation>().animation.getSize();
	auto& ePos = e->getComponent<CTransform>().pos;

	auto& cb = e->getComponent<CBoundingBox>().size;

	Vec2 rad(cb.x * scale.x, cb.y * scale.y);

	float distX = fabs(pos.x - ePos.x);
	float distY = fabs(pos.y - ePos.y);
	return (distX < cb.x / 2 && distY < cb.y / 2);
}

void Scene::buttonPressed(Vec2 pos, const std::string& name, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities(name))
	{
		if (!e->hasComponent<CButton>()) continue;
		auto& eButton = e->getComponent<CButton>();
		if (!eButton.enable) continue;
		if (isInside(pos, e))
		{
			eButton.clicked = m_mouseOnClick;
			if (!m_mouseOnClick)
			{
				sDoAction(eButton.action);
			}
		}
		else
		{
			eButton.clicked = false;
		}
	}
}

void Scene::buttonPressedBB(Vec2 pos, const std::string& name, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities(name))
	{
		if (!e->hasComponent<CButton>()) continue;
		auto& eButton = e->getComponent<CButton>();
		if (!eButton.enable) continue;
		if (isInsideBB(pos, e))
		{
			eButton.clicked = m_mouseOnClick;
			if (!m_mouseOnClick)
			{
				sDoAction(eButton.action);
			}
		}
		else
		{
			eButton.clicked = false;
		}
	}
}

void Scene::buttonHover(Vec2& pos, const std::string& name, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities(name))
	{
		auto& eButton = e->getComponent<CButton>();
		eButton.mouseOver = isInside(pos, e);
	}
}
void Scene::buttonHoverBB(Vec2& pos, const std::string& name, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities(name))
	{
		auto& eButton = e->getComponent<CButton>();
		eButton.mouseOver = isInsideBB(pos, e);
	}
}
void Scene::inputPressed(Vec2 pos, EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities("Input"))
	{
		auto& input = e->getComponent<CTextInput>();
		if (isInside(pos, e))
		{
			input.onFokus = true;
		}
		else
		{
			input.onFokus = false;
		}
	}
}

void Scene::guiSetup(std::string path, EntityManager& m_entityManager)
{
	std::ifstream in(path);

	std::string token, className, animName, actionName;
	int x, y, scale;
	bool enable;

	while (in.good())
	{
		in >> token;
		if (token == "Button")
		{
			in >> className >> animName >> x >> y >> scale >> actionName >> enable;
			//temporary button for dialogue
			auto b = m_entityManager.addEntity(className);
			if (animName != "NONE")
			{
				b->addComponent<CAnimation>(
					m_game->assets().getAnimation(animName),
					true
				);
			}
			b->addComponent<CTransform>(
				Vec2(x, y),
				Vec2(0, 0),
				Vec2(scale, scale),
				0
			);
			auto& size = b->getComponent<CAnimation>().animation.getSize(); 
			auto& scale = b->getComponent<CTransform>().scale; 
			b->addComponent<CBoundingBox>(Vec2(size.x * scale.x, size.y * scale.y)); 
			b->addComponent<CButton>(Action(actionName, "START"));
			b->getComponent<CButton>().enable = enable;
		}
		else if (token == "Box")
		{
			in >> className >> animName >> x >> y >> scale >> actionName;
			//temporary button for dialogue
			auto b = m_entityManager.addEntity(className);
			b->addComponent<CAnimation>(
				m_game->assets().getAnimation(animName),
				true
			);
			b->addComponent<CTransform>(
				Vec2(x, y),
				Vec2(0, 0),
				Vec2(scale, scale),
				0
			);
			auto& size = b->getComponent<CAnimation>().animation.getSize();
			auto& scale = b->getComponent<CTransform>().scale;
			b->addComponent<CBoundingBox>(Vec2(size.x * scale.x, size.y * scale.y));
		}
	}
}

void Scene::proceedDialogues(EntityManager& m_entityManager)
{
	for (auto& e : m_entityManager.getEntities("NPC"))
	{
		auto& cDialog = e->getComponent<CDialogue>();
		if (cDialog.isOn)
		{
			if (cDialog.dialogIndex++ >= cDialog.dialogue_map[cDialog.dialogueState].size() - 1)
			{
				currentState = GameState::Play; 
				cDialog.dialogIndex = 0; 
				cDialog.isOn = false; 
			}
			return;
		}
	}
}

void Scene::loadMapJson(const std::string& path, EntityManager& m_entityManager, Vec2& worldSize)
{
	// reading the json file
	std::ifstream in(path);

	// create a json object
	Json::Value val; 
	Json::Reader reader; 
	 
	// parse json file to json object
	reader.parse(in, val); 

	// getting layer
	auto& layer = val["layers"];

	// layer[0] is tile layer/ base layer
    auto& tileLayer = layer[0];

	//map size
	worldSize.x = tileLayer["width"].asInt();
	worldSize.y = tileLayer["height"].asInt();

	// tile size
	Vec2 gridSize(val["editorsettings"]["chunksize"]["width"].asFloat(), val["editorsettings"]["chunksize"]["height"].asFloat());
	// map size x tile size you get the world size


    int col = 0, row = 0;
    int i = 0;

	int count = 0;

	std::cout << "Size of the map is " << worldSize.x << " x " << worldSize.y << "\n";
	std::cout << "with a grid size " << gridSize.x << " x " << gridSize.y << "\n";
    while (row < worldSize.y)
    {
		
		//std::cout << tileLayer["data"][i].asString() << "\n";
		// avoid 0, because our assets doesn't have animation/texture name 0
		if (tileLayer["data"][i].asInt() != 0)
		{
			//count++;
			//std::cout << tileLayer["data"][i] << " ";
			// create entity with tag 'Tile'
			auto entity = m_entityManager.addEntity("Tile");

			// adding transform component to the tile
			// set size origin to the center of the tile
			entity->addComponent<CTransform>(Vec2((col * gridSize.x) + gridSize.x / 2, (row * gridSize.y) + gridSize.y / 2), // POSITION
				Vec2(0.0f, 0.0f), // VELOCITY
				Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
				0);

			// set the animation/ texture for the tile
			entity->addComponent<CAnimation>(
				m_game->assets().getAnimation(tileLayer["data"][i].asString()),
				true
			);
		}
		/*else
		{
			std::cout << "  ";
		}*/

        col++;
        i++;
        if (col >= worldSize.x)
        {
			//std::cout << std::endl;
            col = 0;
            row++;
        }
    }
	//std::cout << count << "\n";
}

// load object by reading json file
void Scene::loadObjectJson(const std::string& path, EntityManager& entityManager, std::shared_ptr<Entity>& m_player)
{
	//map for properties
	std::map<std::string, float> properties;

	// reading the json file
	std::ifstream in(path);

	// create a json object
	Json::Value val;
	Json::Reader reader;

	// parse json file to json object
	reader.parse(in, val);

	// getting layer
	auto& layer = val["layers"];

	//layer[1] is object layer
	auto& objectLayer = layer[1];

	// loop through all the objects
	for (auto& obj : objectLayer["objects"])
	{
		// getting x, y position of an object
		float x = obj["x"].asFloat();
		float y = obj["y"].asFloat();

		//getting the size of an object
		float width = obj["width"].asFloat();
		float height = obj["height"].asFloat();


		// adjust the position to the center of the object/image
		x += width / 2;
		y -= height / 2; 


		//getting type and name, as class entity, and animation name
		std::string type = obj["type"].asString();
		std::string name = obj["name"].asString();


		//std::cout << type << "\n";
		//check if the object has "properties" values
		if (obj["properties"])
		{

			// getting all the properties
			for (auto& prop : obj["properties"])
			{
				// save it to map of properties
				properties[prop["name"].asString()] = prop["value"].asFloat();
			}


			// spawn object with certain bounding box and horizontal/vertical offset
			spawnObjects(type, 
						name, 
						Vec2(x,y), 
						Vec2(properties["wb"], properties["hb"]),
						Vec2(properties["offx"], properties["offy"]),
						entityManager, m_player);
		}
		else
		{
			spawnObjects(type, name, Vec2(x, y), Vec2(0, 0), Vec2(0, 0), entityManager, m_player);
		}

	}
}

void Scene::spawnObjects(std::string& className, std::string& animationName, const Vec2& pos, Vec2 boundingBox, Vec2 off, EntityManager& m_entityManager, std::shared_ptr<Entity>& m_player)
{
	// Span object "temporary code"
	std::string id;
	if (className.find("NPC") != std::string::npos)
	{
		id = className.substr(4, className.length() - 4);
		className = "NPC";
	}

	auto object = m_entityManager.addEntity(className);
	object->setName(id);
	//std::cout << animationName << "\n";
	object->addComponent<CTransform>(pos * 4.0f, // POSITION
		Vec2(0.0f, 0.0f), // VELOCITY
		Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
		0);
	object->addComponent<CAnimation>(
		m_game->assets().getAnimation(animationName),
		true
	);
	auto& size = object->getComponent<CAnimation>().animation.getSize();

	Vec2 bb(size.x * 4.0f, size.y * 4.0f);
	if (boundingBox.x > 0 && boundingBox.y > 0)
	{
		object->addComponent<CBoundingBox>(boundingBox * 4, off * 4);
	}
	else
	{
		object->addComponent<CBoundingBox>(bb, Vec2(0, 0));
	}


	if (className == "Player")
	{
		object->addComponent<CState>("IdleD");
		m_player = object;
	}

	//determine whos get the cDraggable component
	if (className == "Furniture")
	{
		object->addComponent<CDraggAble>();
	}

	//npc dialogue set up
	if (className == "NPC")
	{
		object->addComponent<CDialogue>(dl::setupdialogue("res/config/dialogue/" + id + "/EN.json"));
	}

}
// adding 0 if the number is one digit
std::string Scene::formatNumber(int number)
{
	std::ostringstream oss;

	if (number < 10 && number >= 0)
	{
		oss << '0';
	}

	oss << number;
	return oss.str();
}
