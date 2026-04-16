#include "MapCfg.h"
#include "inicpp.hpp"
#include "Logger.h"


MapCfg::MapCfg(std::string fileName)
{
    Logger::GetLogger().Info("Reading MapCfg cfg from " + fileName);    
    inicpp::IniManager iniReader(fileName);
    name = iniReader["common"]["name"].get<std::string>();
    background = iniReader["common"]["background"].get<std::string>();
    foreground = iniReader["common"]["foreground"].get<std::string>();
    unsigned int bordersCount = iniReader["common"]["count"].get<int>();
    for(unsigned int i = 0; i < bordersCount; ++i)
    {
        auto id = std::to_string(i);
        std::cout  << "id " << id << "\n";
        auto leftX = iniReader[id]["leftX"].get<int>();
        auto topY = iniReader[id]["topY"].get<int>();
        auto rightX = iniReader[id]["rightX"].get<int>();
        auto bottomY = iniReader[id]["bottomY"].get<int>();

        std::cout  << "leftX " << leftX << " rightX " << rightX << "\n";
        std::cout  << "topY " << topY << " bottomY " << bottomY << "\n";
        assert(leftX <=  rightX);
        assert(topY <=  bottomY);
        walls.push_back({{leftX, topY}, {rightX - leftX, bottomY - topY}});
    }
}