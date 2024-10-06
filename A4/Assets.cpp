#include "Assets.hpp"

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <json/json.h>

Assets::Assets()
{
}

bool Assets::addTexture(const std::string& textureName, const std::string& path,
                        bool smooth)
{
    sf::Texture texture;

    if (!texture.loadFromFile(path))
    {
        std::cerr << "Failed to load texture: " << textureName << " " <<
            path << std::endl;
        return false;
    }

    texture.setSmooth(smooth);
    m_textureMap[textureName] = texture;
    return true;
}

bool Assets::addTexture(const std::string& textureName, const std::string& path,
                        int posX, int posY, int width, int height, bool smooth)
{
    sf::Texture texture;

    if (!texture.loadFromFile(path, sf::IntRect(posX, posY, width, height)))
    {
        std::cerr << "Failed to load texture: " << textureName << " " <<
            path << std::endl;
        return false;
    }

    texture.setSmooth(smooth);
    m_textureMap[textureName] = texture;

    return true;
}

bool Assets::addFont(const std::string& fontName, const std::string& path)
{
    ImGuiIO& io = ImGui::GetIO();
    sf::Font font;

    

    if (!font.loadFromFile(path))
    {
        std::cerr << "Failed to load font: " << fontName << " " << path <<
            std::endl;
        return false;
    }
    else
    {
        /*std::cout << path.c_str() << "\n";
        ImFont* imguiFont = io.Fonts->AddFontFromFileTTF(path.c_str(), 24.0f);
        if (imguiFont == nullptr) 
        {
            std::cerr << "Failed to load ImGui font: " << fontName << " " << path << std::endl;
        }else
            m_imguiFonts[fontName] = imguiFont;*/
    }

    m_fontMap[fontName] = font;
    return true;
}

bool Assets::addAnimation(const std::string& animationName,
                          const std::string& textureName,
                          const size_t keyframesCount, const size_t duration)
{
    const auto& texture = getTexture(textureName);
    m_animationMap[animationName] = Animation(
										animationName, texture, keyframesCount,
										duration
									);
    return true;
}

bool Assets::addTileSet(const std::string& path, int x, int y, int w, int h)
{

    int col = 0, row = 0, index = 1, tileSize = 16;
    while (col < w / x && row < h / y)
    {
        
        m_statString = path + " (" + std::to_string(col) + ", " + std::to_string(row) + ") No. " + std::to_string(index);
        sf::Texture temp;
        if (!temp.loadFromFile(path, sf::IntRect(tileSize * col, tileSize * row, tileSize, tileSize))) {
            std::cout << "fail to load texture\n";
        }
        m_textureMap[std::to_string(index)] = temp;
        addAnimation(std::to_string(index), std::to_string(index), 1, 0);
        col++;
        index++;
        if (col >= w / x)
        {
            row++;
            col = 0;
        }
    }
    return true;
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const
{
    // TODO: handle not found case
    return m_textureMap.at(textureName);
}

const Animation& Assets::getAnimation(const std::string& animationName) const
{
    // TODO: handle not found case
    return m_animationMap.at(animationName);
}

const sf::Font& Assets::getFont(const std::string& fontName) const
{
    // TODO: handle not found case
    return m_fontMap.at(fontName);
}

ImFont* Assets::getImGuiFont(const std::string& fontName) const
{
    auto font = m_imguiFonts.find(fontName);
    if(font != m_imguiFonts.end())
    {
        return font->second;
    }
    return nullptr;
}

const std::map<int, sf::Texture> Assets::getTileSet() const
{
    return m_tileSet;
}

void Assets::loadFromFile(const std::string& filePath)
{
    
    std::ifstream fin(filePath);
    std::string token;

    while (fin.good())
    {
        fin >> token;
        if (token == "Texture")
        {
            std::string name, path;
            int X, Y, width, height;
            bool smooth;
            fin >> name >> path >> X >> Y >> width >> height >> smooth;

            m_statString = "load " + name + " at " + path;
            if (!addTexture(name, path, X, Y, width, height, smooth))
            {
                // TODO: handle error
                std::cout << name << " fail\n";
            }
        }
        else if (token == "Animation")
        {
            std::string name, textureName;
            int keyframesCount, duration;
            fin >> name >> textureName >> keyframesCount >> duration;
            if (!addAnimation(name, textureName, keyframesCount, duration))
            {
                std::cout << name << " fail\n";
            }
            else {
                m_statString = "Add animation " + name;
            }
        }
        else if (token == "Font")
        {
            std::string name, path;
            fin >> name >> path;
            if (!addFont(name, path))
            {
                // TODO: handle error
            }
        }
        else if (token == "Tile")
        {

            std::string path;
            int gridX, gridY, width, height;
            fin >> path >> gridX >> gridY >> width >> height;
            if (addTileSet(path, gridX, gridY, width, height))
            {
                // TODO: handle error
                m_statString = "Tile Set " + path;
            }
            else
            {
            }

        }
        else if (token == "Object")
        {
            std::string path, name;  
            int gridX, gridY, width, height, total; 
            fin >> name >> path >> gridX >> gridY >> width >> height >> total; 

            loadObjectTexture(name, gridX, gridY, width, height, path, total);
        }
        else if (token == "Assets")
        {
            std::string path;
            fin >> path;
            loadFromFile(path);

        }
        
    }

    
}

void Assets::loadObjectTexture(std::string name, int gridx, int gridy, int width, int height, std::string path, int total)
{
    int col = 0, row = 0, index = 1, tileSize = 16; 
    while (col < width / gridx && row < height / gridy) 
    {

        m_statString = path + " (" + std::to_string(col) + ", " + std::to_string(row) + ") No. " + std::to_string(index); 
        sf::Texture temp; 
        if (!temp.loadFromFile(path, sf::IntRect(tileSize * col, tileSize * row, tileSize, tileSize))) { 
            std::cout << "fail to load texture\n";
        }
        m_textureMap[name + std::to_string(index)] = temp; 
        addAnimation(name + std::to_string(index), name + std::to_string(index), 1, 0); 
        col++; 
        index++; 
        if (col >= width / gridx) 
        {
            row++;
            col = 0;
        }

        if (index > total) return;
    }
}

void Assets::clear()
{
    m_animationMap.clear();
    m_textureMap.clear();
    m_tileSet.clear();

    //std::cout << "Clearing Assets\n";
}
