#pragma once

#include "Vec2.hpp"
#include <string>
#include <SFML/Graphics.hpp>

class Action
{
private:
	std::string m_name = "NONE"; // Ex: "SHOOT", "JUMP", "RIGHT"
	std::string m_type = "NONE"; // "START" or "END"
	std::string m_tile = "NONE";
	Vec2		m_pos  = Vec2(0,0);
	sf::Keyboard::Key		m_key;

public:
	Action();
	Action(const std::string& name, const std::string& type);
	Action(const std::string& name, const std::string& type, sf::Keyboard::Key key);
	Action(const std::string& name, const std::string& type, Vec2 pos);
	Action(const std::string& name, const std::string& type, std::string tile);

	const std::string& name() const;
	const std::string& type() const;
	const std::string& tile() const;
	const sf::Keyboard::Key key() const;
	const Vec2& pos() const;
};