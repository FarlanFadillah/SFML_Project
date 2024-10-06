#pragma once

#include "Animation.hpp"
#include "SFML/Graphics.hpp"
#include <map>

#include <imgui.h>
#include <imgui-SFML.h>

class Assets
{
private:
	std::map<std::string, sf::Texture>	m_textureMap;
	std::map<std::string, Animation>	m_animationMap;
	std::map<std::string, sf::Font>		m_fontMap;
	std::map<int, sf::Texture>			m_tileSet;
	std::map<std::string, ImFont*>		m_imguiFonts;

	bool addTexture(const std::string& textureName, const std::string& path,
	                bool smooth = true);
	bool addTexture(const std::string& textureName, const std::string& path,
	                int posX, int posY, int width, int height,
					bool smooth = true);                                        // (posX, posY) is (rectLeft, rectTop)
	bool addAnimation(const std::string& animationName,
	                  const std::string& textureName, const size_t frameCount,
					  const size_t duration);
	bool addTileSet(const std::string& path, int x, int y, int w, int h);

public:
	std::string							m_statString;
	Assets();

	bool addFont(const std::string& fontName, const std::string& path);
	const sf::Texture& getTexture(const std::string& textureName)     const;
	const Animation&   getAnimation(const std::string& animationName) const;
	const sf::Font&    getFont(const std::string& fontName)           const;
	ImFont* getImGuiFont(const std::string& fontName) const;
	const std::map<int, sf::Texture> getTileSet() const;

	void loadFromFile(const std::string& filePath);

	void loadObjectTexture(std::string name, int gridx, int gridy, int width, int height, std::string path, int total);

	void clear();
};