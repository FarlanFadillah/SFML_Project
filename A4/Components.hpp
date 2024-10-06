#pragma once

#include "Animation.hpp"
#include "Vec2.hpp"
#include "Action.hpp"
#include <SFML/Graphics.hpp>

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2  prevPos  = { 0.0f, 0.0f };
	Vec2  pos      = { 0.0f, 0.0f };
	Vec2  velocity = { 0.0f, 0.0f };
	Vec2  scale    = { 1.0f, 1.0f };
	Vec2  grid	   = { 0.0f, 0.0f };
	float angle    = 0;

	CTransform()
	{
	}
	CTransform(const Vec2& p)
		: pos(p)
	{
	}
	CTransform(const Vec2& _pos, const Vec2& _velocity, const Vec2& _scale,
			   float _angle)
		: pos(_pos)
		, prevPos(_pos)
		, velocity(_velocity)
		, scale(_scale)
		, angle(_angle)
	{
	}
	CTransform(const Vec2& _grid, const Vec2& _velocity, const Vec2& _scale, const Vec2& size)
		: grid(_grid) 
		, velocity(_velocity)
		, scale(_scale)
		, pos((_grid.x * size.x) + size.x/2, (_grid.y * size.y) + size.y/2)
		, prevPos((_grid.x* size.x) + size.x / 2, (_grid.y* size.y) + size.y / 2)
	{
		
	}
};

class CInput : public Component
{
public:
	bool up    = false;
	bool down  = false;
	bool left  = false;
	bool right = false;
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;

	CInput()
	{
	}
};

class CLifespan : public Component
{
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation()
	{
	}

	CAnimation(const Animation& _animation, bool _repeat)
		: animation(_animation)
		, repeat(_repeat)
	{
	}
};

class CGravity : public Component
{
public:
	float gravity = 0;

	CGravity()
	{
	}

	CGravity(float g)
		: gravity(g)
	{
	}
};

class CState : public Component
{
public:
	std::string state = "None";
	CState()
	{
	}

	CState(const std::string& _state)
		: state(_state)
	{
	}
};

//class CEmotion: public Component
//{
//public:
//	std::map<std::string, sf::Texture> emotion_map;
//	std::string currentEmotion = "%sm1";
//	CEmotion(){}
//};

class CBoundingBox : public Component
{
public:
	Vec2 size;
	Vec2 halfSize;
	sf::IntRect rectBound;
	Vec2 offRect = { 0.0f, 0.0f };
	CBoundingBox()
	{
	}

	CBoundingBox(const Vec2& _size)
		: size(_size), halfSize(_size.x / 2.0f, _size.y / 2.0f)
	{
	}
	CBoundingBox(const Vec2& _size, const Vec2& _offRect)
		: size(_size), halfSize(_size.x / 2.0f, _size.y / 2.0f)
		, offRect(_offRect)
	{
	}
};

class CDraggAble : public Component
{
public:
	bool dragging = false;
	CDraggAble(){}
};

class CButton : public Component
{
public:
	bool enable = true;
	bool clicked = false;
	bool mouseOver = false;
	std::string tileAnim;
	sf::Text text;
	Action action;
	CButton()
	{
	}
	CButton(Action _action)
		:action(_action)
	{
	}
	CButton(Action _action, std::string _name)
		:action(_action)
	{
		text.setString(_name);
	}
	CButton(Action _action, std::string _name, const sf::Font& _font, int size)
		:action(_action)
	{
		text.setString(_name);
		text.setFont(_font);
		text.setCharacterSize(size);
	}
};

class CTextInput : public Component
{
public:
	std::string name;
	bool		onFokus = false;
	sf::Text	m_text;
	std::vector<std::string> text;
	bool		canInput = false;
	bool		numberOnly = false;
	int			maxWord = 1;
	int			maxWidth = 1;
	int			maxRow = 1;
	CTextInput() 
	{
	}

	CTextInput(size_t fontSize, std::string _name)
		: name(_name)
	{
		text.push_back("");
		m_text.setCharacterSize(fontSize); 
	}
};


class CApp : public Component
{
public:
	Animation animation;
	bool	isOpen = false;
	std::string name;
	CApp() { 
	}
	CApp(const Animation& _anim, const std::string& _name)
		:animation(_anim)
		,name(_name) 
	{

	}
};

class CFingerPrint : public Component
{
public:
	bool selected = false;
	int id = 0;
	int owner_id = 0;
	sf::Texture img;
	CFingerPrint() 
	{}

	CFingerPrint(int _id, int _ownerid, sf::Texture _img) 
		:id(_id)
		, owner_id(_ownerid)
		, img(_img)
	{
	}
};


class CDialogue : public Component
{
public:
	std::map<std::string, std::vector<std::string>> dialogue_map;
	bool isOn = false;
	int dialogIndex = 0;
	std::string dialogueState = "Introduction";
	CDialogue()
	{
	}
	CDialogue(std::map<std::string, std::vector<std::string>> dialogues)
		: dialogue_map(dialogues) 
	{
	}
};

class COptionDialogue : public Component
{

	bool isOn = false;
	COptionDialogue()
	{
	}


};

