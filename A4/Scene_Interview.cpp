#include "Scene_Interview.h"


SceneInterview::SceneInterview(GameEngine* gameEngine, std::string path, const std::string& dialogueScript)
	:Scene(gameEngine)
{
	init(path, dialogueScript);
}
void SceneInterview::init(std::string path, const std::string& dialogueScript)
{
	registerAction(sf::Keyboard::Escape, "EXIT");
	registerAction(sf::Keyboard::Enter, "NEXT_DIALOGUE");
	registerAction(sf::Keyboard::I, "DRAW_STAT");

	guiSetup(path, m_entityManager);
	dl::getDialogueInterviewScript(dialogueScript, m_ds, m_game->assets());
	m_paused = true;

	/*for (auto& e : m_ds.dialogue)
	{
		std::cout << "Detective :" << e.text << "\n";
		std::cout << "Bartender :" << e.npc_response << "\n\n";
	}*/
}

void SceneInterview::update()
{
	m_entityManager.update();
	if(typing)
		typeDialogueLineByLetter(m_ds);
	sDrawImGui();
	sRender();
	m_currentFrame++;
}

void SceneInterview::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void SceneInterview::sRender()
{
	auto& window = m_game->window();

	window.clear(sf::Color::Black);

	sf::Sprite bg;
	bg.setTexture(m_game->assets().getTexture("background_wall"));
	window.draw(bg);

	if (!m_paused)
	{
		for (auto& e : m_entityManager.getEntities("Gui"))
		{
			// rendering button
			if (e->hasComponent<CButton>())
			{
				auto& ebutton = e->getComponent<CButton>();
				if (!ebutton.enable) continue;
				auto& eCT = e->getComponent<CTransform>();

				// button with image
				if (e->hasComponent<CAnimation>())
				{
					auto& anim = e->getComponent<CAnimation>().animation;
					auto& sprite = anim.getSprite();
					sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
					sprite.setPosition(eCT.pos.x, eCT.pos.y);
					window.draw(sprite);
				}
				//button with text
				else {
					auto& text = ebutton.text;
					sf::FloatRect textBounds = text.getLocalBounds();
					text.setFont(m_game->assets().getFont("Ostrich_Regular"));
					text.setFillColor(sf::Color::Black);
					text.setCharacterSize(20);
					text.setOrigin(textBounds.left + textBounds.width / 2.0f,
						textBounds.top + textBounds.height / 2.0f);
					text.setPosition(eCT.pos.x, eCT.pos.y);
					window.draw(text);
				}
			}

			//rendering box dialogue
			else 
			{
				auto& anim = e->getComponent<CAnimation>().animation;
				auto& sprite = anim.getSprite();
				auto& eCT = e->getComponent<CTransform>();

				sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
				sprite.setPosition(eCT.pos.x, eCT.pos.y);

				window.draw(sprite);

				if (anim.getName() == "dialogue_box")
				{
					m_ds.text.setPosition(sprite.getGlobalBounds().left + 192, sprite.getGlobalBounds().top + 32);
					m_ds.text.setString(m_ds.typing);
					window.draw(m_ds.text);


					if (m_ds.state == 1)
					{
						// render the potrait
						sf::Sprite exp;
						auto& texture = m_game->assets().getTexture(m_ds.npc + dl::getExpression(m_ds.line)); 

						exp.setTexture(texture);
						exp.setScale(2.50f, 2.50f);
						exp.setPosition(sprite.getGlobalBounds().left + 8, sprite.getGlobalBounds().top + 8);
						window.draw(exp);
					}

				}
			}
			
		}
	}
	
	ImGui::SFML::Render(window);
	window.display();
}

void SceneInterview::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "EXIT")
		{
			onEnd();
		}
		else if (action.name() == "LEFT_CLICK")
		{
			if (m_paused) return;
			std::cout << action.pos().x << " " << action.pos().y << "\n";
		}
		else if (action.name() == "NEXT_DIALOGUE")
		{
			proceedDialogueInterview(m_ds);
		}
		else if (action.name() == "DRAW_STAT")
		{
			m_drawStat = !m_drawStat;
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT_CLICK")
		{
			if (m_paused) return;
			buttonPressedBB(action.pos(), "Gui", m_entityManager);
		}
	}
}

void SceneInterview::sDrawImGui()
{
	if (m_paused)
		ImGui::OpenPopup("Pause");
	else
		ImGui::CloseCurrentPopup();
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Pause", NULL, ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImVec2 windowSize = ImGui::GetWindowSize();
		float buttonWidth = 120.0f;
		ImGui::Text("Start Confersation?");
		if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_paused = false;
			typing = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(buttonWidth, 0)))
		{
			ImGui::CloseCurrentPopup();
			onEnd();
		}
		ImGui::EndPopup();
	}

	if (m_drawStat)
	{
		if (ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			typing ? ImGui::Text("Typing..") : ImGui::Text("Not Typing");
			ImGui::Text(std::to_string(m_ds.state).c_str());
			m_ds.dialogue[m_ds.i].hasOption ? ImGui::Text("Has Option") : ImGui::Text("No");
			ImGui::Text("Dialogue Index");
			ImGui::Text(std::to_string(m_ds.i).c_str());
			ImGui::End();
		}
	}
	if (!m_paused)
	{
		auto& dialogue = m_ds.dialogue[m_ds.i];
		if (m_ds.state == 1 && dialogue.hasOption)
		{
			ImGui::SetNextWindowPos(ImVec2(1056, 450), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::Begin("", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground);
			for (auto& opt : dialogue.option)
			{
				if (ImGui::Button(opt.second.c_str()))
				{
					if (typing)
					{
						skipTypingDialogeLine(m_ds);
						continue;
					}
					m_ds.i = opt.first - 1;
					m_ds.line = m_ds.dialogue[m_ds.i].text;
					m_ds.typing = "";
					m_ds.t = 0;
					m_ds.state = 0;
					typing = true;
				}
			}
			ImGui::End();
		}
		else {
			ImGui::SetNextWindowPos(ImVec2(1150, 635), ImGuiCond_Always);

			ImGui::Begin("", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground);

			if (ImGui::Button("NEXT"))
			{
				sDoAction(Action("NEXT_DIALOGUE", "START"));
			}
			ImGui::End();
		}
	}
}

void SceneInterview::proceedDialogueInterview(DialogueScript& ds)
{
	if (!typing)
	{
		typing = true;
		auto& dialog = ds.dialogue[ds.i];
		ds.typing = "";
		ds.t = 0;
		if (ds.line == dialog.text)
		{
			ds.line = dialog.npc_response;
			ds.state = 1;
		}
		else if (ds.line == dialog.npc_response)
		{
			if (dialog.next == NULL)
			{
				onEnd();
				return;
			}
			ds.i = dialog.next - 1;
			ds.state = 0;
			if (ds.i == ds.dialogue.size())
			{
				onEnd();
				return;
			}
			ds.line = ds.dialogue[ds.i].text;
		}
	}
	else
	{
		skipTypingDialogeLine(ds);
	}
}

void SceneInterview::typeDialogueLineByLetter(DialogueScript& ds)
{
	auto& ltd = ds.typing;
	auto line = dl::getDialogueText(ds.line);
	if (ds.t < line.length())
	{
		ds.text.setString(ds.typing + line.substr(ds.t, 1));
		if (ds.text.getGlobalBounds().width >= 932)
		{
			ds.typing += "\n";
		}
		ds.typing += line.substr(ds.t, 1);
		ds.t++;
	}
	else
	{
		typing = false;
	}
}

void SceneInterview::skipTypingDialogeLine(DialogueScript& ds)
{
	typing = false;
	ds.typing = "";
	for (auto& c : ds.line)
	{
		ds.text.setString(ds.typing + c);
		if (ds.text.getGlobalBounds().width >= 932)
		{
			ds.typing += "\n";
		}
		ds.typing += c;
	}
}


