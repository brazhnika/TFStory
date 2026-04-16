#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class FontManager
{
public:
   static FontManager& Get();
   const sf::Font& GetFont(std::string str);
   void Clear();
private:
    FontManager() = default;
    std::unordered_map<std::string, sf::Font> m_fonts;
};