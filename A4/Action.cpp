#include "Action.hpp"
#include "Vec2.hpp"

Action::Action()
{
}

Action::Action(const std::string& name, const std::string& type)
	: m_name(name)
	, m_type(type)
{
}
Action::Action(const std::string& name, const std::string& type, sf::Keyboard::Key key)
	: m_name(name)
	, m_type(type)
	, m_key(key)
{
}


Action::Action(const std::string& name, const std::string& type, Vec2 pos)
	:m_name(name)
	, m_type(type)
	,m_pos(pos)
{

}

Action::Action(const std::string& name, const std::string& type, std::string tile)
	:m_name(name)
	, m_type(type)
	,m_tile(tile)
{
}

const std::string& Action::name() const
{
	return m_name;
}

const std::string& Action::type() const
{
	return m_type;
}

const std::string& Action::tile() const
{
	return m_tile;
}

const sf::Keyboard::Key Action::key() const
{
	return m_key;
}

const Vec2& Action::pos() const
{
	return m_pos;
}
