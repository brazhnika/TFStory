#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "inicpp.hpp"
#include "ParseUtils.h"

struct FontCfg
{
    std::string fontFile;
    sf::Color fillColor;
    sf::Color outlineColor;
    float thickness;
    int size;
};

struct FontsCfg
{
    FontCfg normal;
    FontCfg selected;
    FontCfg hovered;
};


inline FontCfg ReadFont(inicpp::IniManager& iniReader, std::string fontSection)
{
    FontCfg cfg;
    cfg.fontFile = iniReader[fontSection]["file"].get<std::string>();
    cfg.fillColor = ParseColor(iniReader[fontSection]["fillColor"].get<std::string>());
    cfg.outlineColor = ParseColor(iniReader[fontSection]["fillColor"].get<std::string>());
    cfg.thickness = iniReader[fontSection]["thikness"].get<int>();
    cfg.size = iniReader[fontSection]["size"].get<int>();
    return cfg;
}

inline FontsCfg ReadFontConfig(inicpp::IniManager& iniReader)
{
    FontsCfg cfg;
    cfg.normal = ReadFont(iniReader, "normalFont");
    cfg.selected = ReadFont(iniReader, "selectedFont");
    cfg.hovered = ReadFont(iniReader, "hoveredFont");    
    return cfg;
}

inline FontsCfg ReadFonts(std::string file)
{
    inicpp::IniManager iniReader(file); 
    return ReadFontConfig(iniReader);
}