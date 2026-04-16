#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>



struct MapCfg
{
    std::string name;
    std::string background;
    std::string foreground;
    std::vector<sf::IntRect> walls;

    MapCfg() = default;
    MapCfg(const MapCfg&) = default;
    MapCfg(std::string fileName);
};

std::string ToString(const MapCfg& cfg);
