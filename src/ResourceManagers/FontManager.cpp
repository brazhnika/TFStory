#include "FontManager.h"

FontManager& FontManager::Get()
{
    static FontManager manager;
    return manager;
}

const sf::Font& FontManager::GetFont(std::string name)
{
   if(m_fonts.find(name) == m_fonts.end())
    {
        m_fonts[name] = sf::Font();
        auto res = m_fonts[name].openFromFile(name);
        assert(res);
    }
    return m_fonts[name]; 
}

void FontManager::Clear()
{
    m_fonts.clear();
}