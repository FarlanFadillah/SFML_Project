#include "Action.hpp"
#include "Vec2.hpp"

Action::Action()
{
}
// constructor for action that take name, and type only
Action::Action(const std::string& name, const std::string& type)
	: m_name(name)
	, m_type(type)
{
}
// constructor for action that take name, type, and keyboard key
Action::Action(const std::string& name, const std::string& type, sf::Keyboard::Key key)
	: m_name(name)
	, m_type(type)
	, m_key(key)
{
}

// constructor for action that take name, and type, and a vector of mouse posistion
// use for mouse click
Action::Action(const std::string& name, const std::string& type, Vec2 pos)
	:m_name(name)
	, m_type(type)
	,m_pos(pos)
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

const sf::Keyboard::Key Action::key() const
{
	return m_key;
}

const Vec2& Action::pos() const
{
	return m_pos;
}
