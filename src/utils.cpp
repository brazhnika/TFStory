#include "utils.h"
#include "ResourceManagers/FontManager.h"
#include <iostream>


void FillFromFont(sf::Text& text, const FontCfg& font)
{
    //text.setFont(*FontManager::Get().GetFont(font.fontFile));
    text.setCharacterSize(font.size);
    text.setFillColor(font.fillColor);
    text.setOutlineColor(font.outlineColor);
    text.setOutlineThickness(font.thickness);
    //std::cout << "Filling with " << font.thickness <<"\n";
}


sf::Text FillFromFont(const FontCfg& font)
{
    sf::Text text(FontManager::Get().GetFont(font.fontFile));
    FillFromFont(text, font);
    return text;
}