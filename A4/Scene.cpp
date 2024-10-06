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

		std::cout << animName << "\n";
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
