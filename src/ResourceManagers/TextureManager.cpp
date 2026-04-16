#include <SFML/Graphics.hpp>
#include <string>
#include "ResourceManagers/TextureManager.h"
#include "Logger.h"
#include <iostream>

TextureManager& TextureManager::Get()
{
    static TextureManager s_instance = TextureManager();
    return s_instance;
}

std::shared_ptr<sf::Texture> TextureManager::GetTexture(std::string str)
{
    Logger::GetLogger().Info("Getting texture " + str);
    if(m_textures.find(str) == m_textures.end())
    {
        Logger::GetLogger().Info("Creating texture " + str + " from file.");
        m_textures[str] = std::make_shared<sf::Texture>(str);
        return m_textures[str];
    }
    else
       return m_textures[str];
}

void TextureManager::Clear()
{
    m_textures.clear();
}
