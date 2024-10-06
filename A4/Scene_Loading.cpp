#include "Scene_Loading.hpp"

SceneLoading::SceneLoading(GameEngine* gameEngine, std::string text)
	:Scene(gameEngine)
	,m_text(text)
{
	m_currentFrame = 0;
}

void SceneLoading::init()
{
}

void SceneLoading::onEnd()
{
}

void SceneLoading::update()
{
	sRender();
	m_currentFrame++;
}

void SceneLoading::sRender()
{
	auto& window = m_game->window();
	window.clear(sf::Color(122, 28, 172));

	m_LoadText.setString(m_text);
	m_LoadText.setFont(m_game->ostrich_regular);
	m_LoadText.setStyle(sf::Text::Style::Bold);
	m_LoadText.setFillColor(sf::Color(235, 211, 248)); 
	m_LoadText.setCharacterSize(50);
	m_LoadText.setPosition(window.getSize().x / 2 - m_LoadText.getGlobalBounds().width / 2, window.getSize().y / 2 - m_LoadText.getGlobalBounds().height / 2);
	
	//Draw loading status
	m_stat.setString(m_game->m_assets.m_statString);
	m_stat.setFont(m_game->ostrich_regular);
	m_stat.setFillColor(sf::Color::Black); 
	m_stat.setCharacterSize(20); 
	m_stat.setPosition(window.getSize().x - m_stat.getGlobalBounds().width - 5, window.getSize().y - m_LoadText.getGlobalBounds().height); 
	window.draw(m_stat);


	window.draw(m_LoadText);
	window.display();
}

void SceneLoading::sDoAction(const Action& action)
{
}

void SceneLoading::sDrawImGui()
{
}


