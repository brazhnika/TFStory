#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "inicpp.hpp"
#include "Types.h"


std::vector<int> ParseListOfInts(std::string str);

sf::Color ParseColor(std::string str);

Rect readRect(inicpp::IniManager& iniReader, std::string sectionName, std::string rectName);